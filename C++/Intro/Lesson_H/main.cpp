#include <tubex.h>
#include <unistd.h>

using namespace std;
using namespace tubex;

bool offline = true;

int main() {
    //--------------- Init

    // Actual trajectory of the state vector
    double dt = 0.05;
    Interval tdomain(0, 15); // temporal domain [t0,tf]

    // Initial pose x0=(0,0,2)
    Vector x0({0, 0, 2});

    // System input
    Trajectory u(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt);

    // Actual trajectories (state + derivative)
    TrajectoryVector x_truth(3); // Actual state vector trajectory
    TrajectoryVector v_truth(3); // Actual derivative of the state vector trajectory
    RandTrajectory n_u(tdomain, 0.05, Interval(-0.03, 0.03));
    v_truth[2] = u + n_u;
    x_truth[2] = v_truth[2].primitive() + x0[2];
    v_truth[0] = 10 * cos(x_truth[2]);
    v_truth[1] = 10 * sin(x_truth[2]);
    x_truth[0] = v_truth[0].primitive() + x0[0];
    x_truth[1] = v_truth[1].primitive() + x0[1];

    //----------------- Tubes
    Tube u_tube(u, dt);
    TubeVector x(tdomain, dt, 3);
    x.set(IntervalVector(x0), 0);
    TubeVector v(tdomain, dt, 3);
    v[2] = u_tube + Tube(tdomain, dt, {-0.03, 0.03});
    x[2] = v[2].primitive() + x0[2];
    x[2] &= Tube(x_truth[2], dt).inflate(0.03);
    v[0] = 10 * cos(x[2]);
    v[1] = 10 * sin(x[2]);
    x[0] = v[0].primitive() + x0[0];
    x[1] = v[1].primitive() + x0[1];

    // Landmarks (unknown)
    vector<Vector> b({{6,  12},
                      {-2, -5},
                      {-3, 20},
                      {3,  4}});

    //----------------- Contractor
    ContractorNetwork cn;

    //----------------- Map boxes for SLAM
    vector<IntervalVector> map;
    for (int i = 0; i < 4; i++) {
        IntervalVector box({Interval::ALL_REALS, Interval::ALL_REALS});
        map.push_back(box);
    }

    //----------------- Graphics
    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(80, 100, 900, 500);
    fig_map.smooth_tube_drawing(true);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1);
    for (auto &beacon : b)
        fig_map.add_beacon(beacon, 0.5);

    if (offline) {
        // Generate observations : obs = [t, distance, beacon]
        vector<IntervalVector> v_obs;
        double t = 0.0;
        while (t < tdomain.ub()) {
            int beacon = rand() % 4;
            double dx = b[beacon][0] - x_truth(t)[0];
            double dy = b[beacon][1] - x_truth(t)[1];
            Interval distance = sqrt(sqr(dx) + sqr(dy));
            IntervalVector temp({Interval(t), distance.inflate(0.03), Interval(beacon)});
            v_obs.push_back(temp);
            t += 2 * dt;
        }

        //-------------- Contraction
        for (auto &obs : v_obs) // For each observation
        {
            // State vector at the time of the observation
            IntervalVector &p = cn.create_dom({Interval::ALL_REALS, Interval::ALL_REALS, Interval::ALL_REALS});
            cn.add(ctc::eval, {obs[0], p, x, v}); // Evaluation of the state vector x at time obs[0]
            cn.add(ctc::dist, {map[obs[2].mid()], p[0], p[1], obs[1]});
        }

        // ------------ Display
        fig_map.show(0.5);

        cn.contract(true);
        for (auto &box : map)
            fig_map.draw_box(box, "green[]");

        fig_map.show(0.5);
    } else {
        double dt = 0.05;
        double iteration_dt = 0.2; // elapsed animation time between each dt
        Interval tdomain(0, 15);   // [t0,tf]

        // Create tubes defined over [t0,tf]
        // Add already known constraints, such as motion equations

        double prev_t_obs = tdomain.lb();
        for (double t = tdomain.lb(); t < tdomain.ub(); t += dt) {
            if (t - prev_t_obs > 2 * dt) // new observation each 2*dt
            {
                // Creating new observation to a random landmark
                int beacon = rand() % 4;
                double dx = b[beacon][0] - x_truth(t)[0];
                double dy = b[beacon][1] - x_truth(t)[1];
                Interval &distance = cn.create_dom({Interval::ALL_REALS});
                distance = sqrt(sqr(dx) + sqr(dy));

                // Adding related observation constraints to the network
                IntervalVector &p = cn.create_dom({Interval::ALL_REALS, Interval::ALL_REALS, Interval::ALL_REALS});
                Interval &It = cn.create_dom(t);
                cn.add(ctc::eval, {It, p, x, v}); // Evaluation of the state vector x at time obs[0]
                cn.add(ctc::dist, {map[beacon], p[0], p[1], distance});

                // Updated last iteration time
                prev_t_obs = t;
            }

            double contraction_dt = cn.contract_during(iteration_dt);
            usleep(max(0., iteration_dt - contraction_dt) * 1e6); // pause for the animation

            // Display the current slice [x](t)
            fig_map.draw_box(x(max(0., ibex::previous_float(t))).subvector(0, 1));
        }
    }
}