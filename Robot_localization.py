from pyibex import *
from vibes import *

Marks = [[6, 12], [-2, -5], [-3, 10], [3, 4]]
D = [Interval(10, 13), Interval(8, 10), Interval(5, 7), Interval(6, 8)]
Alpha = [Interval(0.5, 1), Interval(-3, -1.5), Interval(1, 2), Interval(2, 3)]

vibes.beginDrawing()
vibes.newFigure("loc_pie")
vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})
for m in Marks:
    vibes.drawCircle(m[0], m[1], 0.1, "red[black]")

vibes.axisEqual()
