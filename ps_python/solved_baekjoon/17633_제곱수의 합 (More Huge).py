import math


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
    if is_prime(n): return [n]
    d = pollard_rho(n)
    return factorize(d) + factorize(n // d)

N = int(input())
# N이 제곱수인 경우
if math.isqrt(N) ** 2 == N:
    print(1); exit()

while N % 4 == 0: N //= 4

# 르장드르의 세 제곱수 정리
# N이 4^a(8b+7) 형태인 경우
if N % 8 == 7:
    print(4); exit()

factors = factorize(N)
cnt = dict()
for factor in factors:
    cnt[factor] = cnt.get(factor, 0) + 1

for factor in factors:
    # 4n+3 형태의 소수가 홀수개 존재하는 경우
    if factor % 4 == 3 and N % factor == 0 and cnt[factor] & 1:
        print(3); exit()

print(2)