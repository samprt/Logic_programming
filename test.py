import unittest
from myinterval import *
from mybox import *
import numpy as np

inf = float('inf')
nan = float('nan')


class TestInterval(unittest.TestCase):
    """
        Tests for file myinterval.py
    """

    def setUp(self):
        """
            Variables used for the tests
        """
        self.empty = Interval(1, 0)
        self.R = Interval(-inf, inf)
        self.Rplus = Interval(0, inf)
        self.Rminus = Interval(-inf, 0)
        self.I1 = Interval(-1, 3)
        self.I2 = Interval(2, 5)

    def testRepr(self):
        """
            Test of the representation of intervals
        """
        self.assertEqual(self.I1.__repr__(), "[-1,3]")  # Normal interval
        self.assertEqual(self.R.__repr__(), "R")  # R
        self.assertEqual(self.Rplus.__repr__(), "R+")  # R+
        self.assertEqual(self.Rminus.__repr__(), "R-")  # R-
        self.assertEqual(self.empty.__repr__(), "Ø")  # Empty interval

    def testInstance(self):
        """
            Check if intervals are stable with operators
        """
        self.assertIsInstance(self.I1, Interval)
        self.assertIsInstance(self.empty, Interval)
        self.assertIsInstance(self.R, Interval)
        self.assertIsInstance(self.I1 + self.I2, Interval)
        self.assertIsInstance(self.I1 - self.I2, Interval)
        self.assertIsInstance(self.I1 * self.I2, Interval)
        self.assertIsInstance(self.I1 / self.I2, Interval)
        self.assertIsInstance(max_interval(self.I1, self.I2), Interval)
        self.assertIsInstance(min_interval(self.I1, self.I2), Interval)

    def testArithmetics(self):
        """
            Tests the results of various operators
        """
        self.assertEqual(self.I1 + self.empty, self.I1)  # Addition with empty set
        self.assertEqual(self.I1 + self.I2, Interval(1, 8))  # Addition between 2 intervals
        self.assertEqual(self.I1 - self.I2, Interval(-6, 1))  # Subtraction of 2 intervals
        self.assertEqual(self.I1 * self.I2, Interval(-5, 15))  # Multiplication of 2 intervals
        self.assertEqual(self.I1 * 2, Interval(-2, 6))  # Multiplication of an interval with an int
        self.assertEqual(2 * self.I1, Interval(-2, 6))  # R-Multiplication of an interval with an int
        self.assertEqual(self.I1 / self.I2, Interval(-1/2, 3/2))  # Division of 2 intervals
        self.assertEqual(max_interval(self.I1, self.I2), Interval(2, 5))  # Maximum of 2 intervals
        self.assertEqual(max_interval(self.empty, self.I2), self.I2)  # Maximum of interval and empty set
        self.assertEqual(max_interval(self.R, self.I1), Interval(-1, inf))  # Maximum of interval and R
        self.assertEqual(max_interval(self.Rplus, self.I1), Interval(0, inf))  # Maximum of interval and R+
        self.assertEqual(max_interval(self.Rminus, self.I1), Interval(-1, 3))  # Maximum of interval and R-
        self.assertEqual(min_interval(self.I1, self.I2), Interval(-1, 3))  # Minimum of 2 intervals
        self.assertEqual(min_interval(self.empty, self.I2), self.I2)  # Minimum of interval and empty set
        self.assertEqual(min_interval(self.R, self.I1), Interval(-inf, 3))  # Minimum of interval and R
        self.assertEqual(min_interval(self.Rplus, self.I1), Interval(-1, 3))  # Minimum of interval and R+
        self.assertEqual(min_interval(self.Rminus, self.I1), Interval(-inf, 0))  # Minimum of interval and R-
        self.assertEqual(hull_union(self.I1, self.I2), Interval(-1, 5))  # Hull union of 2 intervals
        self.assertEqual(intersection(self.empty, self.I1), self.empty)  # Intersection with empty set
        self.assertEqual(intersection(self.I1, self.R), self.I1)  # Intersection of I1 and R
        self.assertEqual(intersection(self.I1, self.I2), Interval(2, 3))  # Intersection of I1 and I2
        self.assertEqual(self.I1 & self.I2, Interval(2, 3))  # Intersection of I1 and I2
        self.assertEqual(self.I1 == self.I1, True)  # Equality of same interval
        self.assertEqual(self.empty == self.empty, True)  # Equality of empty set
        self.assertEqual(Interval(1, 0) == Interval(0, -1), True)  # Equality of empty set
        self.assertEqual(self.I1 == self.I2, False)  # Equality of different intervals
        self.assertEqual(3 in self.I2, True)  # 3 is in I2
        self.assertEqual(self.I1 in self.I2, False)  # I1 is not in I2
        self.assertEqual(self.empty in self.I2, True)  # Ø is in every interval
        self.assertEqual(self.I1 in self.R, True)  # every interval is in R

    def testFonctions(self):
        self.assertEqual(sqr(self.I1), Interval(0, 9))  # Square
        self.assertEqual(sqrt(self.I1), Interval(0, math.sqrt(3)))  # Square root
        self.assertEqual(log(self.Rminus), self.empty)
        self.assertEqual(log(self.I2), Interval(math.log(self.I2.lb), math.log(self.I2.ub)))


class TestBox(unittest.TestCase):

    def setUp(self):
        self.empty = Interval(1, 0)
        self.R = Interval(-inf, inf)
        self.I1 = Interval(-1, 3)
        self.I2 = Interval(2, 5)
        self.I3 = Interval(-1, 1.5)
        self.empty_box = Box((self.empty, self.R))
        self.B2d = Box((self.I1, self.I2))
        self.B3d = Box((self.I1, self.I2, self.I3))
        self.LargeBox = Box((Interval(-10, 10), Interval(-15, 10), Interval(0, 20)))
        self.SmallBox = Box((Interval(-3, 2), Interval(-15, -13), Interval(1, 5)))

    def testBox(self):
        self.assertEqual(self.B2d.__repr__(), "[[-1.  2.],[3. 5.]]")
        self.assertEqual(self.B3d.__repr__(), "[[-1.  2. -1.],[3.  5.  1.5]]")
        self.assertEqual(self.B2d.width, 4)
        self.assertEqual(self.B3d.width, 4)
        self.assertEqual(self.B2d.axis, 0)
        self.assertEqual(self.B3d.axis, 0)
        self.assertEqual(self.empty_box.is_empty(), True)
        self.assertEqual(self.empty_box == self.empty_box, True)
        self.assertEqual(self.B2d == self.B2d, True)
        self.assertEqual(self.B2d == Box((self.I1, self.I2)), True)
        B2d1, B2d2 = self.B2d.split()
        self.assertEqual(B2d1, Box((Interval(-1, 1), Interval(2, 5))))
        self.assertEqual(B2d2, Box((Interval(1, 3), Interval(2, 5))))
        self.assertEqual(np.array([0, 0, 0]) in self.LargeBox, True)
        self.assertEqual(np.array([[0], [0], [0]]) in self.LargeBox, True)
        self.assertEqual(self.SmallBox in self.LargeBox, True)
        self.assertEqual(self.LargeBox in self.SmallBox, False)


if __name__ == "__main__":
    unittest.main()
