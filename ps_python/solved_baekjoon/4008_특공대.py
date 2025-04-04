lines = list[tuple[int, int]]()

def inter(a, b):
    return (lines[a][1] - lines[b][1]) / (lines[b][0] - lines[a][0])

def add(a, b):
    lines.append((a, b))
    while len(lines) > 2 and inter(-1, -2) < inter(-2, -3): lines.pop(-2)

def get(x):
    s, e = 0, len(lines) - 1
    while s < e:
        m = s + e >> 1
        if inter(m, m + 1) < x: s = m + 1
        else: e = m
    return lines[e][0] * x + lines[e][1]

N = int(input())
a, b, c = map(int, input().split())
X = [0] + list(map(int, input().split()))
for i in range(1, N + 1): X[i] += X[i - 1]

ev = lambda x: a * x ** 2 + b * x + c
dp = [0] * (N + 1)
dp[1] = ev(X[1]) # base case
# add(0, 0) # y = 0
for i in range(2, N + 1):
    slope = -2 * a * X[i - 1] # X[i]와 관련이 있는 항에서 X[i]를 제거한 것 -> 기울기
    intercept = dp[i - 1] + a * X[i - 1] ** 2 - b * X[i - 1] # X[i]와 관련이 없는 항들 -> y절편
    add(slope, intercept)
    dp[i] = ev(X[i]) + get(X[i])
    # 나이브 풀이
    # dp[i] = max(dp[j] + ev(X[i] - X[j]) for j in range(i))

print(dp[N])