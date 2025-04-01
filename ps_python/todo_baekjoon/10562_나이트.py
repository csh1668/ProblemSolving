mod = 1000000000 + 9

def berlekamp_massey(x: list[int]):
    ls, cur = [], []
    lf = ld = 0
    for i in range(len(x)):
        t = 0
        for j in range(len(cur)):
            t = (t + x[i - j - 1] * cur[j]) % mod
        if (t - x[i]) % mod == 0:
            continue
        if len(cur) == 0:
            cur = [0] * (i + 1)
            lf = i
            ld = ((t - x[i]) % mod) - mod
            continue
        k = -(x[i] - t) * pow(ld, mod - 2, mod) % mod
        c = [0] * (i - lf - 1)
        c.append(k)
        for j in ls:
            c.append(-j * k % mod)
        while len(c) < len(cur):
            c.append(0)
        for j in range(len(cur)):
            c[j] = (c[j] + cur[j]) % mod
        if i - lf + len(ls) >= len(cur):
            ls, lf, ld = cur, i, t - x[i] % mod
        cur = c
    for i in cur:
        i %= mod
    return cur

def get_nth(rec, dp, n):
    m = len(rec)
    s, t = [0] * m, [0] * m
    s[0] = 1
    if m != 1:
        t[1] = 1
    else:
        t[0] = rec[0]

    def mul(v: list[int], w: list[int]):
        m = len(v)
        t = [0] * (2 * m)
        for j in range(m):
            for k in range(m):
                t[j + k] += v[j] * w[k] % mod
                if t[j + k] >= mod:
                    t[j + k] -= mod
        for j in range(2 * m - 1, m - 1, -1):
            for k in range(1, m + 1):
                t[j - k] += t[j] * rec[k - 1] % mod
                if t[j - k] >= mod:
                    t[j - k] -= mod
        t = [t[i] for i in range(m)]
        return t

    while n:
        if n & 1:
            s = mul(s, t)
        t = mul(t, t)
        n >>= 1
    res = 0
    for i in range(m):
        res += s[i] * dp[i] % mod
    return res % mod

def guess_nth_term(x: list[int], n: int):
    if n < len(x):
        return x[n]
    v = berlekamp_massey(x)
    # print(v)
    if len(v) == 0:
        return 0
    return get_nth(v, x, n)

dir = ((-1, 2), (-2, 1), (-2, -1), (-1, -2), (1, -2), (2, -1), (2, 1), (1, 2))

res = 0
def backtracking(arr, vis):
    global res
    for x in range(len(arr)):
        for y in range(len(arr[0])):
            if arr[x][y] == 0 and not vis[x][y]:
                can_place = True
                for dx, dy in dir:
                    nx, ny = x + dx, y + dy
                    if 0 <= nx < len(arr) and 0 <= ny < len(arr[0]) and arr[nx][ny] == 1:
                        can_place = False
                        break
                if can_place:
                    vis[x][y] = True
                    arr[x][y] = 1
                    res += 1
                    backtracking(arr, [[a for a in b] for b in vis])
                    arr[x][y] = 0

def calc(n, m):
    global res
    res = 1
    backtracking([[0] * m for _ in range(n)], [[False] * m for _ in range(n)])
    return res

print()

# # 1일때는 2의 N제곱, 2일때부터는 계산 필요
# cases = [None, None, dict(), dict(), dict()]
# for N in range(3, 5):
#     for M in range(1, 10):
#         cases[N][M] = calc(N, M) % mod
#         print(N, M, cases[N][M])

berlekamp = [None, None, None, None, None]
case2 = [1, 2, 4, 6, 9, 15, 25, 40, 64, 104, 169, 273, 441, 714, 1156]
berlekamp[2] = berlekamp_massey(case2)
print(berlekamp[2], berlekamp[3])
# for N in range(2, 3):
#     berlekamp[N] = berlekamp_massey(list(cases[N].values()))

print(get_nth(berlekamp[2], case2, 50) ** 2 % mod)

dp3 = [[0] * 3 for _ in range(10)]
dp3[0] = 

# print(cases)
# print(berlekamp)
#
# dps = [None, None, None, None, None]
# for N in range(2, 5):
#     dps[N] = [0] * len(berlekamp[N])

# for _ in range(int(input())):
#     N, M = map(int, input().split())
#