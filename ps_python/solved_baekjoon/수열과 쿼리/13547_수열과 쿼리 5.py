import math
import sys

n = int(input())
sqrt_n = int(math.sqrt(n))
arr = list(map(int, input().split()))
m = int(input())
queries = [list(map(lambda x: int(x) - 1, sys.stdin.readline().split())) for i in range(m)]

pi = 0
pj = -1
dic = [0] * 1000001
cnt = 0
for q in sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1])):
    i, j = q
    while pj < j:
        pj += 1
        if dic[arr[pj]] == 0:
            cnt += 1
        dic[arr[pj]] += 1
    while pj > j:
        if dic[arr[pj]] > 0:
            dic[arr[pj]] -= 1
        if dic[arr[pj]] == 0:
            cnt -= 1
        pj -= 1
    while pi >= i:
        pi -= 1
        if dic[arr[pi]] == 0:
            cnt += 1
        dic[arr[pi]] += 1
    while pi < i:
        if dic[arr[pi]] > 0:
            dic[arr[pi]] -= 1
        if dic[arr[pi]] == 0:
            cnt -= 1
        pi += 1
    q.append(cnt)

print(*map(lambda x: x[-1], queries), sep='\n')