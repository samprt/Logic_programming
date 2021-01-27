import math
import numpy
from myinterval import *


class Box:

    def __init__(self, union):
        if len(union) == 0 or len(union) == 1:
            raise ValueError("A box must have at least 2 intervals")
        self.intervals = list(union)
        self.axis = 0
        self.width = 0
        lb, ub = [], []
        for i in range(len(union)):
            if union[i].width > self.width:
                self.width = union[i].width
                self.axis = i
            lb.append(float(union[i].lb))
            ub.append(float(union[i].ub))
        self.lb = numpy.array(lb)  # .reshape((len(lb), 1))
        self.ub = numpy.array(ub)  # .reshape((len(ub), 1))
        if Interval(1, 0) in union:
            self.ub = nan
            self.lb = nan

    def __repr__(self):
        if self.is_empty():
            return "Ø"
        else:
            return "["+str(self.lb)+","+str(self.ub)+"]"

    def __eq__(self, other):
        if self.is_empty() and other.is_empty():
            if math.isnan(self.lb) and math.isnan(other.lb):
                return True
        elif self.lb.all() == other.lb.all() and self.ub.all() == other.ub.all():
            return True
        return False

    def __contains__(self, item):
        if type(item) == int or type(item) == float:
            raise ValueError("The dimension of the box is bigger than the item")
        if type(item) == numpy.ndarray:
            if len(self.intervals) != item.shape[0]:
                raise ValueError("The dimension of the box is different than the item")
            else:
                g = len(self.intervals)
                for i in range(g):
                    if float(item[i]) not in self.intervals[i]:
                        return False
                return True
        if type(item) == Box:
            if len(self.intervals) != len(item.intervals):
                raise ValueError("The dimension of the box is different than the item")
            else:
                g = len(self.intervals)
                for i in range(g):
                    if item.intervals[i] not in self.intervals[i]:
                        return False
                return True

    def is_empty(self):
        if type(self.lb) == float:
            if math.isnan(self.lb):
                return True
        return False

    def split(self):
        t = self.intervals[self.axis]
        intervals = self.intervals.copy()
        del(intervals[self.axis])
        t1 = left(t)
        t2 = right(t)
        intervals1 = intervals.copy()
        intervals1.insert(self.axis, t1)
        intervals2 = intervals.copy()
        intervals2.insert(self.axis, t2)
        return Box(intervals1), Box(intervals2)