#include <tubex.h>

using namespace std;
using namespace tubex;

int main()
{
  // Truth (unknown pose)
  Vector x_truth({2.,1.,M_PI/6.}); // (x,y,heading)
  Vector y_truth({6, M_PI/6.});
  Vector m_truth({5., 6.2});

  //Create the bounded sets [x], [y], [m]
  IntervalVector x({Interval::ALL_REALS, 
                   Interval::ALL_REALS,
                   Interval(x_truth[2])});
  IntervalVector y({Interval(y_truth[0]).inflate(0.3),
                   Interval(y_truth[1]).inflate(0.1)});
  IntervalVector m({Interval(m_truth[0]), Interval(m_truth[1])});
  m.inflate(0.2);

  CtcFunction ctc_sub_1(Function("m1", "x1", "d1", "m1 - x1 - d1"));
  CtcFunction ctc_sub_2(Function("m2", "x2", "d2", "m2 - x2 - d2"));
  CtcFunction ctc_add(Function("x3", "y2", "theta", "x3 + y2 - theta"));
  
  Interval d1, d2;
  Interval theta;

  //Contractor network
  ContractorNetwork cn;
  cn.add(ctc_sub_1, {m[0], x[0], d1});
  cn.add(ctc_sub_2, {m[1], x[1], d2});
  cn.add(ctc_add, {x[2], y[1], theta});
  cn.add(ctc::polar, {d1, d2, y[0], theta});
  

	// Graphics
	vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100,100,500,500);
  fig_map.axis_limits(0,7,0,7);
  fig_map.draw_vehicle(x_truth, 1);
  fig_map.draw_box(m, "red");
  fig_map.draw_pie(x_truth[0], x_truth[1], y[0], x[2] + y[1]);

  cn.contract();
  
  fig_map.draw_box(x.subvector(0,1)); // does not display anything if unbounded
  fig_map.show();

  vibes::endDrawing();
}