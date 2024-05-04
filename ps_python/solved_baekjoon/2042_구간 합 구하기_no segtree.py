import sys

n, m, k = map(int, input().split())
arr = [int(sys.stdin.readline()) for _ in range(n)]
prefix_sum = arr[:]
for i in range(1, n):
    prefix_sum[i] += prefix_sum[i - 1]
prefix_sum.append(0)

changes = dict()
for _ in range(m + k):
    a, b, c = map(int, sys.stdin.readline().split())
    if a == 1:
        changes[b - 1] = c
    else:
        res = prefix_sum[c - 1] - prefix_sum[b - 2]
        for change in changes.items():
            if (b - 1) <= change[0] < c:
                res += change[1] - arr[change[0]]
        print(res)