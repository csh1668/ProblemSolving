n, k = map(int, input().split())
arr = [list(map(int, input().split())) for _ in range(n)]
mod = pow(10, 9) + 7


def matrix_mul(a, b):
    res = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                res[i][j] += a[i][k] * b[k][j]
            res[i][j] %= mod
    return res


def matrix_pow(a, b):
    res = [[1 if i == j else 0 for j in range(n)] for i in range(n)]
    while b:
        if b & 1:
            res = matrix_mul(res, a)
        a = matrix_mul(a, a)
        b >>= 1
    return res


arr = matrix_pow(arr, k)
cnt = 0
for i in range(n):
    for j in range(n):
        cnt = cnt + arr[i][j] % mod
print(cnt)