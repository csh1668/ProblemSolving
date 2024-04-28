n, r, m = map(int, input().split())
cache = [[0] * m for _ in range(m)]


def convert(number):
    res = []
    while number:
        res.append(number % m)
        number //= m
    return res


def binomial_coefficient(n, k, mod):
    cache[n][0] = 1
    for i in range(1, n + 1):
        j = min(i, k)
        while j:
            cache[n][j] += cache[n][j - 1]
            j -= 1
    return cache[n][k]


a, b = convert(n), convert(r)
while len(a) != len(b):
    b.append(0)
answer = 1
for i in range(len(a)):
    x, y = a[i], b[i]
    if x < y:
        answer = 0
        break
    if x == 0:
        continue
    answer = answer * binomial_coefficient(x, y, m) % m
print(answer)