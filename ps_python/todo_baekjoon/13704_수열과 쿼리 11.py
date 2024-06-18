import sys

n, k = map(int, input().split())
sqrt_n = int(n ** 0.5)
arr = [0] + list(map(int, input().split()))
m = int(input())
queries = [list(map(int, sys.stdin.readline().split())) for _ in range(m)]

for i in range(n):
    arr[i + 1] ^= arr[i]
# print(arr)

x = y = 0
ans = 0
for idx, q in enumerate(sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1]))):
    left, right = q[0] - 1, q[1]
    if idx == 1:
        x = y = left
    while y < right:
        y += 1
        # 연산
    while y > right:
        # 연산
        y -= 1
    while x > left:
        x -= 1
        # 연산
    while x < left:
        # 연산
        x += 1
    q.append(ans)

print(*map(lambda x: x[-1], queries), sep='\n')