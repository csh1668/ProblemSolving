N = int(input())
A = list(map(int, input().split()))
money = 0

for i in range(N - 2):
    if A[i] > A[i + 1]:
        diff = A[i] - A[i + 1]
        money += diff * 3
        A[i] -= diff
    if A[i] <= A[i + 1] > A[i + 2]:
        diff = min(A[i], A[i + 1] - A[i + 2])
        money += diff * 5
        A[i + 1] -= diff
        A[i] -= diff
    if A[i] <= A[i + 1] <= A[i + 2]:
        A[i + 2] -= A[i]
        A[i + 1] -= A[i]
        money += A[i] * 7
        A[i] = 0

mn, mx = min(A[N - 2], A[N - 1]), max(A[N - 2], A[N - 1])
money += mn * 5
money += (mx - mn) * 3
A[N - 2] = A[N - 1] = 0

print(money)
# print(A)