import math
import numpy

nan = float('nan')


class Interval:

    def __init__(self, a, b):
        if a > b:
            self.lb = nan
            self.ub = nan
            self.width = 0
        else:
            self.lb = a
            self.ub = b
            self.width = self.ub - self.lb

    def __repr__(self):
        if self.is_empty():
            return "Ø"
        if self.lb == -float('inf') and self.ub == float('inf'):
            return "R"
        if self.lb == 0 and self.ub == float('inf'):
            return "R+"
        if self.lb == -float('inf') and self.ub == 0:
            return "R-"
        return "[" + str(self.lb) + "," + str(self.ub) + "]"

    def __len__(self):
        return 1

    def is_empty(self):
        return math.isnan(self.lb)

    def __add__(self, other):
        if other.is_empty():
            return self
        return Interval(self.lb + other.lb, self.ub + other.ub)

    def __sub__(self, other):
        other = Interval(-other.ub, -other.lb)
        return self + other

    def __mul__(self, other):
        if type(other) == int:
            return self.__mul__(Interval(other, other))
        if type(other) == float:
            return self.__mul__((Interval(other, other)))
        x1 = self.lb * other.lb
        x2 = self.lb * other.ub
        x3 = self.ub * other.lb
        x4 = self.ub * other.ub
        return Interval(min(x1, x2, x3, x4), max(x1, x2, x3, x4))

    def __rmul__(self, other):
        return self.__mul__(other)

    def __truediv__(self, other):
        if self.is_empty():
            return Interval(0, -1)
        if 0 in other:
            return Interval(-float('inf'), float('inf'))
        return self * Interval(1 / other.ub, 1 / other.lb)

    def __contains__(self, item):
        if type(item) == float:
            return self.lb <= item <= self.ub
        if type(item) == int:
            return self.lb <= item <= self.ub
        # if type(item) == numpy.int64:
        #     return self.lb <= item <= self.ub
        if type(item) == Interval:
            if item.is_empty():
                return True
            if self.is_empty():
                return False
            if item.ub < self.lb or item.lb > self.ub:
                return False
            if self.lb <= item.lb and item.ub <= self.ub:
                return True

    def __eq__(self, other):
        if self.is_empty() and other.is_empty():
            return True
        else:
            return self.lb == other.lb and self.ub == other.ub

    def __ne__(self, other):
        return not self.__eq__(other)

    def __and__(self, other):
        return intersection(self, other)


def sqrt(i):
    if 0 in i:
        return Interval(0, math.sqrt(i.ub))
    return Interval(math.sqrt(i.lb), math.sqrt(i.ub))


def sqr(i):
    t = [i.lb ** 2, i.ub ** 2]
    if 0 in i:
        return Interval(0, max(t))
    return Interval(min(t), max(t))


def exp(i):
    return Interval(math.exp(i.lb), math.exp(i.ub))


def log(i):
    if i.ub < 0:
        return Interval(0, -1)
    if 0 in i and i.ub != 0:
        return Interval(-float('inf'), math.log(i.ub))
    if 0 in i and i.ub == 0:
        return Interval(0, -1)
    return Interval(math.log(i.lb), math.log(i.ub))


def min_interval(i1, i2):
    if i1.is_empty():
        return i2
    if i2.is_empty():
        return i1
    return Interval(min(i1.lb, i2.lb), min(i1.ub, i2.ub))


def max_interval(i1, i2):
    if i1.is_empty():
        return i2
    if i2.is_empty():
        return i1
    return Interval(max(i1.lb, i2.lb), max(i1.ub, i2.ub))


def hull_union(i1, i2):
    if i1.is_empty():
        return i2
    if i2.is_empty():
        return i1
    t = [i1.lb, i2.lb, i1.ub, i2.ub]
    return Interval(min(t), max(t))


def intersection(i1, i2):
    if i1.is_empty():  # No inclusion because of empty set
        return Interval(1, 0)
    if i2.is_empty():  # No inclusion because of empty set
        return Interval(1, 0)
    if i1 in i2:  # First inclusion
        return i1
    if i2 in i1:  # Second inclusion
        return i2
    if i1.ub < i2.lb or i2.ub < i1.lb:  # No inclusion
        return Interval(1, 0)
    if i1.lb < i2.lb < i1.ub < i2.ub:
        return Interval(i2.lb, i1.ub)
    if i2.lb < i1.lb < i2.ub < i1.ub:
        return Interval(i1.lb, i2.ub)
    if i1.ub == i2.lb:
        return Interval(i1.ub, i1.ub)
    if i1.lb == i2.ub:
        return Interval(i1.lb, i1.lb)


def left(i):
    return Interval(i.lb, (i.lb + i.ub)/2)


def right(i):
    return Interval((i.lb + i.ub)/2, i.ub)


if __name__ == "__main__":
    I1 = Interval(1, 2)
    I2 = Interval(-1, 4)
    I3 = Interval(1, 0)
    B = Box((I1, I2))
    B1, B2 = B.split()
    print(B1.width)
    print(B2)
