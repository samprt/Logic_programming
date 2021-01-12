from pyibex import *
from vibes import *


vibes.beginDrawing()
vibes.newFigure('Ring with Pyibex')
vibes.setFigureProperties({'x': 200, 'y': 100, 'width': 800, 'height': 800})
f1 = Function("x1", "x2", "(x1-1)^2 + (x2-2)^2")
f2 = Function("x1", "x2", "(x1-2)^2 + (x2-5)^2")
r1 = Interval(4, 5)
r2 = Interval(5, 6)
X = IntervalVector(2, [-10, 10])
sep1 = SepFwdBwd(f1, sqr(r1))
sep2 = SepFwdBwd(f2, sqr(r2))
sep = sep1 & sep2
pySIVIA(X, sep, 0.1)
vibes.axisEqual()



