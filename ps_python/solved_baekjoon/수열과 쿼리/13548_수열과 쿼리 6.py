import math
import sys

n = int(input())
sqrt_n = int(math.sqrt(n))
arr = list(map(int, input().split()))
m = int(input())
queries = [list(map(lambda x: int(x) - 1, sys.stdin.readline().split())) for i in range(m)]

pi = 0
pj = -1
dic = [0] * 100001
count_of = [0] * 100002
ans = 0
for q in sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1])):
    i, j = q
    while pj < j:
        pj += 1
        dic[arr[pj]] += 1
        count_of[dic[arr[pj]] - 1] -= 1
        count_of[dic[arr[pj]]] += 1
        ans = max(ans, dic[arr[pj]])
    while pj > j:
        if dic[arr[pj]] > 0:
            dic[arr[pj]] -= 1
            count_of[dic[arr[pj]] + 1] -= 1
            count_of[dic[arr[pj]]] += 1
            if ans == dic[arr[pj]] + 1 and count_of[dic[arr[pj]] + 1] == 0:
                ans -= 1
        pj -= 1
    while pi > i:
        pi -= 1
        dic[arr[pi]] += 1
        count_of[dic[arr[pi]] - 1] -= 1
        count_of[dic[arr[pi]]] += 1
        ans = max(ans, dic[arr[pi]])
    while pi < i:
        if dic[arr[pi]] > 0:
            dic[arr[pi]] -= 1
            count_of[dic[arr[pi]] + 1] -= 1
            count_of[dic[arr[pi]]] += 1
            if ans == dic[arr[pi]] + 1 and count_of[dic[arr[pi]] + 1] == 0:
                ans -= 1
        pi += 1
    q.append(ans)

print(*map(lambda x: x[-1], queries), sep='\n')