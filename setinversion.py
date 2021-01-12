from myinterval import *
from mybox import *
from vibes import *


if __name__ == '__main__':
    x0 = Box((Interval(-3, 3), Interval(-3, 3)))
    y = [Interval(1.5, 2),
             Interval(0.7, 0.8),
             Interval(0.1, 0.3),
             Interval(-0.1, 0.03)]
    epsilon = 0.1
    T = [0.2, 1, 2, 4]
    L = [x0]
    Xminus = []
    Xplus = []

    for i in range(len(T)):
        while len(L):
            x = L.pop()
            f = x.intervals[0] * exp(x.intervals[1] * T[i])
            if f in y[i]:
                Xminus.append(x)
                Xplus.append(x)
            elif intersection(f, y[i]).is_empty():
                pass
            elif x.width < epsilon:
                Xplus.append(x)
            else:
                x1, x2 = x.split()
                L.append(x1)
                L.append(x2)

    vibes.beginDrawing()
    vibes.newFigure("set inversion")
    vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})
    for x in Xplus:
        vibes.drawBox(x.lb[0], x.ub[0], x.lb[1], x.ub[1], '[red]')
    for x in Xminus:
        vibes.drawBox(x.lb[0], x.ub[0], x.lb[1], x.ub[1], '[blue]')
    vibes.endDrawing()