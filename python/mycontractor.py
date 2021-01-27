from myinterval import *
from mybox import *

inf = float('inf')


def Cadd(z, x, y):  # z = x + y
    z = z & x + y
    x = x & z - y
    y = y & z - x
    return z, x, y


def Cmul(z, x, y):
    z = z & x*y
    x = x & z/y
    y = y & z/x
    return z, x, y


def Cexp(y, x):
    y = y & exp(x)
    x = x & log(y)
    return y, x


def Csqr(y, x):
    x1 = x & Interval(-inf, 0)
    x2 = x & Interval(0, inf)
    y = hull_union(y & sqr(x1), y & sqr(x2))
    if y.is_empty():
        return y, y
    else:
        x1 = x1 & (-1 * sqrt(y))
        x2 = x2 & sqrt(y)
        return y, hull_union(x1, x2)


if __name__ == '__main__':
    x = Interval(0, 6)
    y = Interval(1.5, 6)
    print(Cexp(y, x))
