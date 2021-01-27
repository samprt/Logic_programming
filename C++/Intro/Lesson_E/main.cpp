#include <tubex.h>

using namespace std;
using namespace tubex;


int main()
{
  //--------------- Init
  // trajectory of the robot
  double dt = 0.01;
  Interval tdomain(0,5); // temporal domain [t0,tf]
  TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);
  
  //Speed
  TrajectoryVector actual_v(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt);

  //Landmark
  Vector b({0.5, 1});

  //Distance from the landmark
  Trajectory actual_y = sqrt(sqr(actual_x[0] - b[0]) + sqr(actual_x[1] - b[1]));
  RandTrajectory n(tdomain, dt, Interval(-0.2, 0.2));
  //Trajectory display_y;
  //display_y = actual_y + n;
  actual_y += n;

  //---------------- Tubes
  TubeVector x(actual_x, dt);
  x.inflate(0.2);

  Tube y = Tube(actual_y, dt);
  y.inflate(0.2);

  TubeVector v = TubeVector(actual_v, dt);
  v.inflate(0.01);

  //-------------- Contraction
  ContractorNetwork cn;
  IntervalVector bI = IntervalVector(b);

  cn.add(ctc::dist, {x, bI, y});
  cn.add(ctc::deriv, {x, v});


  //----------------- Graphics
  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.add_trajectory(&actual_x, "x*", 0, 1);
  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
  fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
  fig_map.show(0.5);

  VIBesFigTube fig_dist("Distance to the landmark");
  fig_dist.set_properties(100, 100, 600, 300);
  fig_dist.add_trajectory(&actual_y, "y*");
  fig_dist.add_tube(&y, "y");
  fig_dist.show();

  cn.contract();

  fig_map.show(0.5);
  fig_dist.show();
}