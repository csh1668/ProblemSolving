import sys

lines = list[tuple[int, int]]()

def inter(a, b):
    return (lines[a][1] - lines[b][1]) / (lines[b][0] - lines[a][0])

def add(a, b):
    lines.append((a, b))
    while len(lines) > 2 and inter(-1, -2) <= inter(-2, -3): lines.pop(-2)

def get(x):
    s, e = 0, len(lines) - 1
    while s < e:
        m = s + e >> 1
        if inter(m, m + 1) < x: s = m + 1
        else: e = m
    return e

N = int(sys.stdin.readline())
A = []
for (w, h) in sorted([tuple(map(int, sys.stdin.readline().split())) for _ in range(N)], reverse=True):
    if not A or A[-1][1] < h: A.append((w, h))

dp = [0] * len(A)
dp[0] = A[0][0] * A[0][1]
add(A[1][0], dp[0])
for i in range(1, len(A)):
    j = get(A[i][1])
    dp[i] = lines[j][0] * A[i][1] + lines[j][1]
    if i < len(A) - 1: add(A[i + 1][0], dp[i])
    # naive
    # dp[i] = min(dp[j] + A[j + 1][0] * A[i][1] for j in range(i))
print(dp[-1])