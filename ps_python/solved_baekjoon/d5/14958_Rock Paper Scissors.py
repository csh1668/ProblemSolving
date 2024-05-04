mod = 998244353
w = 3


def ntt(a, inverse=False):
    n = len(a)
    j = 0
    for i in range(1, n):
        reverse = n // 2
        while j >= reverse:
            j -= reverse
            reverse //= 2
        j += reverse
        if i < j:
            a[i], a[j] = a[j], a[i]
    step = 2
    while step <= n:
        half = step // 2
        u = pow(w, mod // step, mod)
        if inverse:
            u = pow(u, mod - 2, mod)
        for i in range(0, n, step):
            wi = 1
            for j in range(i, i + half):
                v = a[j + half] * wi
                a[j + half] = (a[j] - v) % mod
                a[j] = (a[j] + v) % mod
                wi = (u * wi) % mod
        step *= 2

    if inverse:
        num = mod - (mod - 1) // n
        for i in range(n):
            a[i] = (a[i] * num) % mod


def multiply_ntt(a, b):
    n = 1
    while n < len(a):
        n <<= 1
    while n < len(b):
        n <<= 1
    n <<= 1
    while len(a) < n:
        a.append(0)
    while len(b) < n:
        b.append(0)
    ntt(a)
    ntt(b)
    c = []
    for i in range(n):
        c.append(a[i] * b[i])
    ntt(c, True)
    return c


n, m = map(int, input().split())
rps = input()
my = input()

r_win, p_win, s_win = [], [], []
r_my, p_my, s_my = [], [], []
for c in rps:
    if c == 'R':
        r_win.append(0)
        p_win.append(1)
        s_win.append(0)
    elif c == 'P':
        r_win.append(0)
        p_win.append(0)
        s_win.append(1)
    else:
        r_win.append(1)
        p_win.append(0)
        s_win.append(0)
for c in my:
    if c == 'R':
        r_my.append(1)
        p_my.append(0)
        s_my.append(0)
    elif c == 'P':
        r_my.append(0)
        p_my.append(1)
        s_my.append(0)
    else:
        r_my.append(0)
        p_my.append(0)
        s_my.append(1)


# print(r_win)
# print(p_win)
# print(s_win)
# print(r_my)
# print(p_my)
# print(s_my)
r_my.reverse()
p_my.reverse()
s_my.reverse()

r_res = multiply_ntt(r_win, r_my)
p_res = multiply_ntt(p_win, p_my)
s_res = multiply_ntt(s_win, s_my)

# print(r_res)
# print(p_res)
# print(s_res)

res = 0
for i in range(m - 1, n + m - 1):
    res = max(res, r_res[i] + p_res[i] + s_res[i])
print(res)