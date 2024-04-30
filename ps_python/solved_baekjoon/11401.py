mod = 1000000000 + 7

n, r = map(int, input().split())
if r > n - r:
    r = n - r
a = b = 1
for i in range(r):
    a = (a * (n - i)) % mod
    b = (b * (i + 1)) % mod
res = (a * pow(b, mod - 2, mod)) % mod
print(res)