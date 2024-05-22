import sys

mod = pow(10, 9) + 7
n, q = int(input()), int(input())
arr = [0 for _ in range(n)]
fib = [0 for _ in range(n + 2)]
fib[0] = fib[1] = 1
for i in range(2, n + 2):
    fib[i] = (fib[i - 1] + fib[i - 2]) % mod

idx = 0
res = [0] * n
goto = [-1] * n
for (left, right) in reversed([tuple(map(int, sys.stdin.readline().split())) for _ in range(q)]):
    idx = left - 1
    j = 0
    while j < right - left + 1:
        if res[idx] == 0:
            res[idx] = fib[j + 1]
            goto[idx] = right
            idx += 1
            j += 1
        else:
            j += goto[idx] - idx
            idx = goto[idx]
print(*res)