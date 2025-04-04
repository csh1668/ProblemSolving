import sys
r = sys.stdin.readline

def egcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, x, y = egcd(b % a, a)
        return g, y - (b // a) * x, x

def crt(a, b, a_mod, b_mod):
    g, x, y = egcd(a_mod, b_mod)
    if (b - a) % g != 0:
        return -1, -1
    x = x * (b - a) // g
    y = y * (b - a) // g
    lcm = a_mod * b_mod // g
    return (a + a_mod * x % lcm) % lcm, lcm

for _ in range((int(r()))):
    A, B, C, a, b, c = map(int, r().split())
    x, y = crt(a, b, A, B)
    if x == -1:
        print(-1)
        continue
    x, y = crt(x, c, y, C)
    print(x)