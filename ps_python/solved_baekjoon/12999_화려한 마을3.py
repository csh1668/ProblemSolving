import sys

n, m = map(int, sys.stdin.readline().split())
arr = list(map(int, sys.stdin.readline().split()))
queries = [list(map(lambda x: int(x) - 1, sys.stdin.readline().split())) for _ in range(m)]

sqrt_n = 600
for i in range(n):
    arr[i] += 100001
b = [0] * 200002    # 색깔의 개수
cnt = [0] * 100002  # 개수의 개수

x = y = 0
ans = 0
for idx, q in enumerate(sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1]))):
    l, r = q
    if idx == 0:
        x = l
        y = l - 1
    while y < r:
        y += 1
        b[arr[y]] += 1
        cnt[b[arr[y]] - 1] -= 1
        cnt[b[arr[y]]] += 1
        ans = max(b[arr[y]], ans)
    while y > r:
        b[arr[y]] -= 1
        cnt[b[arr[y]] + 1] -= 1
        cnt[b[arr[y]]] += 1
        if cnt[b[arr[y]] + 1] == 0 and ans == b[arr[y]] + 1:
            ans -= 1
        y -= 1
    while x > l:
        x -= 1
        b[arr[x]] += 1
        cnt[b[arr[x]] - 1] -= 1
        cnt[b[arr[x]]] += 1
        ans = max(b[arr[x]], ans)
    while x < l:
        b[arr[x]] -= 1
        cnt[b[arr[x]] + 1] -= 1
        cnt[b[arr[x]]] += 1
        if cnt[b[arr[x]] + 1] == 0 and ans == b[arr[x]] + 1:
            ans -= 1
        x += 1

    q.append(ans)

print(*map(lambda x: x[-1], queries), sep='\n')
