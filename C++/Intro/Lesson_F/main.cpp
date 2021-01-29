#include <tubex.h>

using namespace std;
using namespace tubex;

bool intro = false;

int main()
{
  if(intro)
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

    //Measurement
    IntervalVector yi({{-0.84, -0.83}, {-0.76, -0.75}});

    //---------------- Tubes
    TubeVector x(tdomain, dt, 2);
    

    TubeVector v = TubeVector(actual_v, dt);
    v.inflate(0.03);

    //-------------- Contraction
    ContractorNetwork cn;
    double ti = 2;
    cn.add(ctc::deriv, {x, v});
    cn.add(ctc::eval, {ti, yi, x, v});


    //----------------- Graphics
    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100, 100, 600, 300);
    fig_map.add_trajectory(&actual_x, "x*", 0, 1);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
    fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);

    cn.contract();

    fig_map.show(0.5);
  }

  //--------------- Init
  // Actual trajectory of the state vector
  double dt = 0.01;
  Interval tdomain(0,3); // temporal domain [t0,tf]
  TrajectoryVector actual_x(tdomain, TFunction("(10*cos(t) + t ; 5*sin(2*t) + t ; atan2((10*cos(2*t) + 1), -10*sin(t) + 1) ; sqrt((-10*sin(t) + 1)^2 + (10*cos(2*t) + 1)^2))"), dt);
  IntervalVector u({{-oo, oo}, {-oo, oo}});

  //Landmarks
  IntervalVector b1({{8}, {3}});
  IntervalVector b2({{0}, {5}});
  IntervalVector b3({{-2}, {1}});

  // Measurements of psi and v
  Tube psi = Tube(actual_x[2], dt);
  psi.inflate(0.01);

  Tube v = Tube(actual_x[3], dt);
  psi.inflate(0.01);

  // Measurements of distance to landmarks at certain times
  double t1 = 0.3;
  Interval y1(1.9);
  y1.inflate(0.1);
  
  double t2 = 1.5;
  Interval y2(3.6);
  y2.inflate(0.1);
  
  double t3 = 2.0;
  Interval y3(2.8);
  y3.inflate(0.1);

  //---------------- Tubes
  TubeVector x(tdomain, dt, 4);
  x[2] = psi;
  x[3] = v;

  //-------------- Contraction
  ContractorNetwork cn;
  TubeVector a(tdomain, dt, 4);
  IntervalVector p1(4);
  IntervalVector p2(4);
  IntervalVector p3(4);
  CtcFunction f(Function("x[4]", "u[2]", "a[4]", "(x[3]*cos(x[2]) - a[0] ; x[3]*sin(x[2]) - a[1] ; u[0] - a[2] ; u[1] - a[3])"));
  cn.add(ctc::deriv, {x, a});
  cn.add(f, {x, u, a});
  cn.add(ctc::dist, {p1[0], p1[1], b1, y1});
  cn.add(ctc::dist, {p2[0], p2[1], b2, y2});
  cn.add(ctc::dist, {p3[0], p3[1], b3, y3});
  cn.add(ctc::eval, {t1, p1, x, a});
  cn.add(ctc::eval, {t2, p2, x, a});
  cn.add(ctc::eval, {t3, p3, x, a});

  //----------------- Graphics
  vibes::beginDrawing();

  VIBesFigMap fig_map("Map");
  fig_map.set_properties(100, 100, 600, 300);
  fig_map.add_trajectory(&actual_x, "x*", 0, 1);
  fig_map.add_tube(&x, "x", 0, 1);
  fig_map.add_beacon(b1, 0.1);
  fig_map.add_beacon(b2, 0.1);
  fig_map.add_beacon(b3, 0.1);
  fig_map.draw_ring(b1[0].mid(), b1[1].mid(), y1, "gray");
  fig_map.draw_ring(b2[0].mid(), b2[1].mid(), y2, "gray");
  fig_map.draw_ring(b3[0].mid(), b3[1].mid(), y3, "gray");
  fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);

  cn.contract();

  fig_map.show(0.5);
}