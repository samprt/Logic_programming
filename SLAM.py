from pyibex import *
from vibes import *
import numpy as np
import random


def rand(I):
    return I.lb() + random.random() * (I.ub() - I.lb())


def cstate(x1, y1, x, y, th):
    fx = Function("x1", "x", "th", "x1 - x - %f*10*cos(th)" % dt)
    fy = Function("y1", "y", "th", "y1 - y - %f*10*sin(th)" % dt)
    Cx = CtcFwdBwd(fx, Interval(0, 0))
    Cy = CtcFwdBwd(fy, Interval(0, 0))
    Bx = IntervalVector(3)
    Bx[0], Bx[1], Bx[2] = x1, x, th
    Cx.contract(Bx)
    x1, x, th = Bx[0], Bx[1], Bx[2]
    By = IntervalVector(3)
    By[0], By[1], By[2] = y1, y, th
    Cy.contract(By)
    y1, y, th = By[0], By[1], By[2]
    return x1, y1, x, y


def cmarks(x, y, mx, my, d):
    f = Function("x", "y", "mx", "my", "(x - mx)^2 + (y - my)^2")
    C = CtcFwdBwd(f, sqr(d))
    X = IntervalVector(4)
    X[0], X[1], X[2], X[3] = x, y, mx, my
    C.contract(X)
    x, y, mx, my = X[0], X[1], X[2], X[3]
    return x, y, mx, my


vibes.beginDrawing()
vibes.newFigure("SLAM")
vibes.setFigureProperties({'x': 0, 'y': 0, 'width': 1000, 'height': 1000})

_x, _y, _th, dt = 0, 0, 1, 0.1
noise = 0.03 * Interval(-1, 1)
_X, _Y, Th, J, D = [], [], [], [], []
_mx, _my = [6, -2, -3, 3], [1, -5, 4, 4]
kmax = 100
for k in range(1, kmax + 1):
    j = random.randint(0, 3)
    J.append(j)
    _d = np.sqrt((_mx[j] - _x) ** 2 + (_my[j] - _y) ** 2)
    D.append(_d + rand(noise) + noise)
    _X.append(_x)
    _Y.append(_y)
    Th.append(_th + rand(noise) + noise)
    _x += dt * 10 * np.cos(_th)
    _y += dt * 10 * np.sin(_th)
    _th += dt * (3 * np.sin(k * dt) ** 2 + rand(noise))

X = [Interval(-20, 20)] * (kmax + 1)
X[0] = Interval(0)
Y = X.copy()
Mx = [Interval(-20, 20)] * 4
My = [Interval(-20, 20)] * 4

# Compute the box estimations for the position
for k in range(1, kmax):
    X[k], Y[k], X[k - 1], Y[k - 1] = cstate(X[k], Y[k], X[k - 1], Y[k - 1], Th[k - 1])

# Compute the box estimations for the landmarks
for k in range(kmax):
    j = J[k]
    X[k], Y[k], Mx[j], My[j] = cmarks(X[k], Y[k], Mx[j], My[j], D[k])

# Draw the positions
for k in range(kmax):
    vibes.drawCircle(_X[k], _Y[k], 0.1, "black[]")

# Draw the boxes of the estimated positions
for Xk, Yk in zip(X, Y):
    vibes.drawBox(Xk[0], Xk[1], Yk[0], Yk[1], "blue[]")

# Draw the landmarks positions
for j in range(0, 4):
    vibes.drawCircle(_mx[j], _my[j], 0.1, "red[red]")

# Draw estimated landmarks positions using SLAM method
for j in range(0, 4):
    vibes.drawBox(Mx[j][0], Mx[j][1], My[j][0], My[j][1], "red[]")
