from myinterval import *
from vibes import *


def minimize(f, tmin, tmax, dt, draw=False, color="[blue]"):
    t = tmin
    m = Interval(float('inf'), float('inf'))
    while t < tmax:
        x = Interval(t, t + dt)
        y = f(x)
        if draw:
            vibes.drawBox(x.lb, x.ub, y.lb, y.ub, color)
        t = t + dt
        m = min_interval(m, y)
    return m


if __name__ == '__main__':
    vibes.beginDrawing()
    vibes.newFigure("my minimizer")
    vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})

    def f(x):
        return sqr(x) + 2*x - exp(x)

    m = minimize(f, -2, 2, 0.5, True, '[blue]')
    print("m =", m)
    m = minimize(f, -2, 2, 0.05, True, '[yellow]')
    print("m =", m)
    m = minimize(f, -2, 2, 0.005, True, '[green]')
    print("m =", m)
    m = minimize(f, -2, 2, 0.0005, True, '[red]')
    print("m =", m)
    vibes.endDrawing()
