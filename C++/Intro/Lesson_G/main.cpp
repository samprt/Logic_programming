#include <tubex.h>

using namespace std;
using namespace tubex;

bool intro = false;

int main() {
    //--------------- Init
    // Actual trajectory of the state vector
    double dt = 0.05;
    Interval tdomain(0, 6); // temporal domain [t0,tf]
    TrajectoryVector actual_x(tdomain, TFunction(
            "(10*cos(t) + t ; 5*sin(2*t) + t ; atan2((10*cos(2*t) + 1), -10*sin(t) + 1) ; sqrt((-10*sin(t) + 1)^2 + (10*cos(2*t) + 1)^2))"),
                              dt);
    IntervalVector u({{-oo, oo},
                      {-oo, oo}});

    // Creating random map of landmarks
    int nb_landmarks = 150;
    IntervalVector map_area(actual_x.codomain().subvector(0, 1));
    map_area.inflate(2);
    vector<IntervalVector> v_map =
            DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

    // Generating observations obs=(t,range,bearing) of these landmarks
    int max_nb_obs = 20;
    Interval visi_range(0, 4); // [0m,75m]
    Interval visi_angle(-M_PI / 4, M_PI / 4); // frontal sonar
    vector<IntervalVector> v_obs =
            DataLoader::generate_observations(actual_x, v_map, max_nb_obs,
                                              true, visi_range, visi_angle);

    for (auto &obs : v_obs) {
        obs[1].inflate(0.1);
        obs[2].inflate(0.04);
    }

    //Tube
    TubeVector x(tdomain, dt, 4);
    x[2] = Tube(actual_x[2], dt).inflate(0.01);
    x[3] = Tube(actual_x[3], dt).inflate(0.01);

    //-------------- Contraction
    ContractorNetwork cn;
    TubeVector v(tdomain, dt, 4); // Derivative of x
    CtcFunction ctc_add(Function("x1", "x2", "x3", "x1 + x2 - x3")); //Add contractor function
    CtcFunction ctc_sub(Function("x", "y", "z", "x - y - z")); //Sub contractor function
    CtcConstell ctc_constell(v_map); //Constellation contractor on map v_map
    for (auto &obs : v_obs) // For each observation
    {
        // State vector at the time of the observation
        IntervalVector &p = cn.create_dom(
                {Interval::ALL_REALS, Interval::ALL_REALS, Interval::ALL_REALS, Interval::ALL_REALS});
        Interval &Mx = cn.create_dom(Interval::ALL_REALS); // x of landmark
        Interval &My = cn.create_dom(Interval::ALL_REALS); // y of landmark
        Interval &d1 = cn.create_dom(Interval::ALL_REALS); // x distance of landmark
        Interval &d2 = cn.create_dom(Interval::ALL_REALS); // y distance of landmark
        Interval &theta = cn.create_dom(Interval::ALL_REALS); // Bearing of the measurement
        cn.add(ctc::eval, {obs[0], p, x, v}); // Evaluation of the state vector x at time obs[0]
        cn.add(ctc_sub, {Mx, p[0], d1}); // d1 = mx - x
        cn.add(ctc_sub, {My, p[1], d2}); // d2 = my - y
        cn.add(ctc_add, {p[2], obs[2], theta}); // theta = x3 + y1
        cn.add(ctc::polar, {d1, d2, obs[1], theta}); // Range and bearing contractor
        cn.add(ctc_constell, {Mx, My}); // Contract the position of the landmark
    }
    // Derivative contractor
    CtcFunction f(Function("x[4]", "u[2]", "a[4]",
                           "(x[3]*cos(x[2]) - a[0] ; x[3]*sin(x[2]) - a[1] ; u[0] - a[2] ; u[1] - a[3])"));
    cn.add(ctc::deriv, {x, v});
    cn.add(f, {x, u, v});

    //----------------- Graphics
    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100, 100, 600, 300);
    fig_map.smooth_tube_drawing(true);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&actual_x, "x*", 0, 1);
    fig_map.add_observations(v_obs, &actual_x); // drawing obs
    for (const auto &b : v_map)
        fig_map.add_beacon(b.mid(), 0.1); // drawing beacons


    cn.contract(true);

    fig_map.show(0.5);
}