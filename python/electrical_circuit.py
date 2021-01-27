from myinterval import *
from mybox import *
from mycontractor import *

if __name__ == '__main__':
    R1 = Interval(0, inf)
    R2 = Interval(0, inf)
    R = Interval(-inf, inf)
    E = Interval(23, 26)
    I = Interval(4, 8)
    I2 = Interval(-inf, inf)
    U1 = Interval(10, 11)
    U2 = Interval(14, 17)
    P = Interval(124, 130)

    for i in range(10):
        R, R1, R2 = Cadd(R, R1, R2)
        I2, I = Csqr(I2, I)
        P, E, I = Cmul(P, E, I)
        P, R, I2 = Cmul(P, R, I2)
        E, R, I = Cmul(E, R, I)
        U1, R1, I = Cmul(U1, R1, I)
        U2, R2, I = Cmul(U2, R2, I)
        E, U1, U2 = Cadd(E, U1, U2)

    print("R1 =", R1)
    print("R2 =", R2)

