mod = 1000000007


def multiply_matrix_mod(a: list[list[int]], b: list[list[int]]):
    c = [[0 for _ in range(2)] for _ in range(2)]
    for i in range(2):
        for j in range(2):
            for k in range(2):
                c[i][j] += a[i][k] * b[k][j]
            c[i][j] %= mod
    return c


def power_matrix_mod(a: list[list[int]], mul: int):
    result = [[1 if i == j else 0 for j in range(2)] for i in range(2)]
    while mul:
        if mul & 1:
            result = multiply_matrix_mod(result, a)
        a = multiply_matrix_mod(a, a)
        mul >>= 1
    return result


def fib(n):
    fib_matrix = [[1, 1], [1, 0]]
    return power_matrix_mod(fib_matrix, n)[0][0]


n = int(input())
print(fib(n-1) * fib(n) % mod)