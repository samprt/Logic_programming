from pyibex import *
from pyibex.geometry import SepPolarXY
from vibes import *

Marks = [[6, 12], [-2, -5], [-3, 10], [3, 4]]
D = [Interval(10, 13), Interval(8, 10), Interval(5, 7), Interval(6, 8)]
Alpha = [Interval(0.5, 1), Interval(-3, -1.5), Interval(1, 2), Interval(2, 3)]

seps = []
for m, d, alpha in zip(Marks, D, Alpha):
    sep1 = SepPolarXY(d, alpha)
    fforward = Function("v1", "v2", "(%f - v1 ; %f - v2)" % (m[0], m[1]))
    fbackward = Function("p1", "p2", "(%f - p1 ; %f - p2)" % (m[0], m[1]))
    sep = SepTransform(sep1, fforward, fbackward)
    seps.append(sep)

Sep = SepQInterProjF(seps)
Sep.q = 1

sepU = SepUnion(seps)

P = IntervalVector(2, [-20, 20])

vibes.beginDrawing()
vibes.newFigure("loc_pie")
vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})
pySIVIA(P, Sep, 0.01)
# pySIVIA(P, sepU, 0.01)

for m in Marks:
    vibes.drawCircle(m[0], m[1], 0.3, "yellow[black]")

vibes.axisEqual()
