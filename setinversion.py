from vibes import *

using_pyibex = True

if __name__ == '__main__':
    if using_pyibex:
        from pyibex import *

        T = [0.2, 1, 2, 4, 5]
        y = [Interval(1.5, 2),
             Interval(0.7, 0.8),
             Interval(0.1, 0.3),
             Interval(-0.1, 0.03),
             Interval(6, 7)]

        seps = []
        for i in range(len(T)):
            f = Function("p1", "p2", "p1 * exp(p2*%f)" % T[i])
            seps.append(SepFwdBwd(f, y[i]))

        sep = SepQInterProjF(seps)
        sep.q = 0

        vibes.beginDrawing()
        vibes.newFigure("set inversion")
        vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})
        P = IntervalVector(2, [-3, 3])
        pySIVIA(P, sep, 0.02)
        vibes.axisEqual()

    if not using_pyibex:
        from myinterval import *
        from mybox import *

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
