mod = 1000000000 + 7
m = int(input())
cache = [0] * 4000001
cache[0] = 1
for i in range(1, 4000001):
    cache[i] = (cache[i - 1] * i) % mod

for _ in range(m):
    n, r = map(int, input().split())
    res = (cache[n] * pow(cache[r] * cache[n - r], mod - 2, mod)) % mod
    print(res)