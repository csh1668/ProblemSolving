import math
import decimal

a, b, c = map(decimal.Decimal, input().split())
decimal.getcontext().prec = 100
decimal.getcontext().rounding = decimal.ROUND_HALF_UP
pi = decimal.Decimal("3.14159265358979323846264338327950288419716939937510582097494459230781640628")
precision = decimal.Decimal("0." + ("0" * 50) + "1")


def sin_precision(i: decimal.Decimal):
    i = i % (2 * pi)
    n = decimal.Decimal(1)
    tmp = decimal.Decimal(i)
    p = decimal.Decimal(i)
    while abs(tmp) >= precision:
        n += 2
        fac = n * (n - 1)
        tmp = (tmp * i * i) / -fac
        p += tmp
    return p


def f(i):
    return a * i + b * sin_precision(i)


x = c / a
step = decimal.Decimal("0.5")

while True:
    fx = f(x)
    if abs(c - fx) < precision:
        break
    if fx < c:
        x += step
    else:
        x -= step
    step /= 2

print(round(x, 6))
# print(x)