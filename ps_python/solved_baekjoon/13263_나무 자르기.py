N = int(input())
A, B = [list(map(int, input().split())) for _ in range(2)]

lines = list[tuple[int, int]]()

def inter(a, b):
    return (lines[a][1] - lines[b][1]) / (lines[b][0] - lines[a][0])

def add(a, b):
    lines.append((a, b))
    while len(lines) > 2 and inter(-1, -2) < inter(-2, -3):
        lines.pop(-2)

def get(x):
    s, e = 0, len(lines) - 1
    while s < e:
        m = s + e >> 1
        if inter(m, m + 1) < x:
            s = m + 1
        else:
            e = m
    return e

dp = [0] * N
add(B[0], dp[0])
for i in range(1, N):
    j = get(A[i])
    dp[i] = lines[j][0] * A[i] + lines[j][1]
    add(B[i], dp[i])

print(dp[-1])