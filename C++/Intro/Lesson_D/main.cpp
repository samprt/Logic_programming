#include <tubex.h>

using namespace std;
using namespace tubex;

class MyCtc : public ibex::Ctc
{
  public:

    MyCtc(const std::vector<ibex::IntervalVector>& M_)
      : ibex::Ctc(2), // the contractor acts on 2d boxes
        M(M_)         // attribute needed later on for the contraction
    {

    }

    void contract(ibex::IntervalVector& a)
    {
      // Insert contraction formula here (question D.2)
      std::vector<ibex::IntervalVector> sets;
      for(auto&Mi : M)
        sets.push_back(a & Mi);
      IntervalVector temp(2, Interval::EMPTY_SET);
      for(auto& set : sets)
      {
        temp |= set;
      }
      a = temp;
    }

  protected:

    const std::vector<ibex::IntervalVector> M;
};

int main()
{
  //test the constellation contractor
  vector<IntervalVector> M;
  M.push_back(IntervalVector({{1.5}, {2.5}}));
  M.push_back(IntervalVector({{3}, {1}}));
  M.push_back(IntervalVector({{2}, {2}}));
  M.push_back(IntervalVector({{2.5}, {3}}));
  M.push_back(IntervalVector({{3.5}, {2}}));
  M.push_back(IntervalVector({{4}, {1}}));
  M.push_back(IntervalVector({{1.5}, {0.5}}));
  for(auto& Mi : M)
    Mi.inflate(0.05);

  IntervalVector a1({{1.25, 3}, {1.6, 2.75}});
  IntervalVector a2({{2, 3.5}, {0.6, 1.2}});
  IntervalVector a3({{1.1, 3.25}, {0.2, 1.4}});

  MyCtc ctc_constell(M);
  ContractorNetwork cn_test;

  cn_test.add(ctc_constell, {a1});
  cn_test.add(ctc_constell, {a2});
  cn_test.add(ctc_constell, {a3});

	/*// Graphics
	vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.axis_limits(0,7,0,7);
  for (auto& Mi :M)
    fig_map.draw_box(Mi, "red[red]");
  fig_map.draw_box(a1, "blue[]");
  fig_map.draw_box(a2, "red[]");
  fig_map.draw_box(a3, "green[]");

  cn_test.contract();
  cout << "a1 = " << a1 << endl;
  cout << "a2 = " << a2 << endl;
  cout << "a3 = " << a3 << endl;

  fig_map.draw_box(a1, "blue[]");
  fig_map.draw_box(a2, "red[]");
  fig_map.draw_box(a3, "green[]");  

  vibes::endDrawing();*/

  //Localization
  Vector x_truth({2, 1, M_PI/6});
  vector<IntervalVector> v_obs =
    DataLoader::generate_static_observations(x_truth, M, false);

  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.draw_vehicle(x_truth,0.5);
  for(const auto& Mi : M)
    fig_map.draw_box(Mi, "red[orange]");

  // Adding uncertainties on the measurements
  for(auto& obs : v_obs)
  {
    obs[0].inflate(0.02); // range
    obs[1].inflate(0.02); // bearing
  }
  for(auto& obs : v_obs)
    fig_map.draw_pie(x_truth[0], x_truth[1], obs[0], x_truth[2] + obs[1]);

  IntervalVector x({Interval::ALL_REALS, Interval::ALL_REALS, Interval(x_truth[2])});
  
  // Define contractors
  CtcFunction ctc_sub_1(Function("m1", "x1", "d1", "m1 - x1 - d1"));
  CtcFunction ctc_sub_2(Function("m2", "x2", "d2", "m2 - x2 - d2"));
  CtcFunction ctc_add(Function("x3", "y2", "theta", "x3 + y2 - theta"));
  

  ContractorNetwork cn;

  /*cout << "M[i][0]" << M[0][0] << endl;
  cout << "M[i][1]" << M[0][1] << endl;
  cout << "v_obs[i][0]" << v_obs[0][0] << endl;
  cout << "v_obs[i][1]" << v_obs[0][1] << endl;*/

  for(int i = 0 ; i < v_obs.size() ; i++) // for each measurement
  {
    // Define intermediate variables
    Interval& d1 = cn.create_dom(Interval::ALL_REALS);
    Interval& d2 = cn.create_dom(Interval::ALL_REALS);
    Interval& theta = cn.create_dom(Interval::ALL_REALS);

    // Add contractors and related domains
    cn.add(ctc_sub_1, {M[i][0], x[0], d1});
    cn.add(ctc_sub_2, {M[i][1], x[1], d2});
    cn.add(ctc_add, {x[2], v_obs[i][1], theta});
    cn.add(ctc::polar, {d1, d2, v_obs[i][0], theta});

    // Note: v_obs[i] is a 2d vector [y^i]
    // Note: M[i] is a 2d box of M
  }

  cn.contract();

  fig_map.draw_box(x.subvector(0, 1), "red[]");

  fig_map.axis_limits(fig_map.view_box(), true, 0.1);
}