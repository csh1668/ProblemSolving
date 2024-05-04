import math

a, b, c = map(int, input().split())


def f(i):
    return a * i + b * math.sin(i)


x = c / a
step = 0.5

while True:
    fx = f(x)
    if abs(c - fx) < 0.0000000001:
        break
    if fx < c:
        x += step
    else:
        x -= step
    step /= 2

print(x)