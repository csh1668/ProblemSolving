import sys
from collections import deque

n = int(input())
arr = [int(sys.stdin.readline()) for _ in range(n)] + [0]
stack = deque()
ans = 0
for i, e in enumerate(arr):
    k = i
    while stack and stack[-1][1] > e:
        k, prev = stack.pop()
        ans = max(ans, (i - k) * prev)
    stack.append((k, e))
# print(stack)
print(ans)