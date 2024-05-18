import math
import sys

n, m = map(int, input().split())
arr = list(map(int, input().split()))
queries = [list(map(lambda x: int(x) - 1, sys.stdin.readline().split())) for _ in range(m)]

# 좌표 압축
sorted_arr = sorted(arr)
compressed = dict()
for i, e in enumerate(sorted_arr):
    if e not in compressed.keys():
        compressed[e] = i
for i in range(n):
    arr[i] = compressed[arr[i]]
# print(arr)

sqrt_n = int(math.sqrt(n))
counter = [0] * n
sorted_queries = sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1]))
pi = pj = cnt = idx = 0

for q in sorted_queries:
    i, j = q
    if idx == 0:
        pi = pj = i
        counter[arr[pi]] = 1

    while pi < i:
        

    q.append(cnt)
    idx += 1