import functools


def is_prime(n):
    if n <= 5 or n % 2 == 0 or n % 3 == 0:
        return n == 2 or n == 3 or n == 5
    s = ((n - 1) & (1 - n)).bit_length() - 1
    d = n >> s

    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        p = pow(a, d, n)
        if p == 1 or p == n - 1 or a % n == 0: continue
        for _ in range(s):
            p = (p * p) % n
            if p == n - 1: break
        else: return False
    return True

def pollard_rho(n):
    from math import gcd
    from random import randrange
    if is_prime(n): return n
    if n == 1: return 1
    if n % 2 == 0: return 2
    x = randrange(2, n)
    y = x; d = 1
    c = randrange(1, n)

    while d == 1:
        x = (x * x + c) % n
        y = (y * y + c) % n
        y = (y * y + c) % n
        d = gcd(abs(x - y), n)
        if d == n: return pollard_rho(n)
    return pollard_rho(d)

def factorize(n):
    if n == 1: return []
    ret = []
    while n > 1:
        p = pollard_rho(n)
        ret.append(p)
        n //= p
    return ret

N = int(input())

factors = factorize(N)

cnt = dict()
for f in factors:
    cnt[f] = cnt.get(f, 0) + 1

ans = functools.reduce(lambda x, y: x * (y + 1), cnt.values(), 1)
print(ans)