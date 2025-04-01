N, B, C = map(int, input().split())
A = list(map(int, input().split()))

if B <= C:
    print(sum(A) * B)
    exit()

money = 0
for i in range(N - 2):
    if A[i] > A[i + 1]:
        diff = A[i] - A[i + 1]
        money += diff * B
        A[i] -= diff
    if A[i] <= A[i + 1] > A[i + 2]:
        diff = min(A[i], A[i + 1] - A[i + 2])
        money += diff * (B + C)
        A[i + 1] -= diff
        A[i] -= diff
    if A[i] <= A[i + 1] <= A[i + 2]:
        A[i + 2] -= A[i]
        A[i + 1] -= A[i]
        money += A[i] * (B + C * 2)
        A[i] = 0

mn, mx = min(A[N - 2], A[N - 1]), max(A[N - 2], A[N - 1])
money += mn * (B + C)
money += (mx - mn) * B
A[N - 2] = A[N - 1] = 0

print(money)
# print(A)