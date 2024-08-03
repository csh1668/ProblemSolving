import sys

n, c = map(int, sys.stdin.readline().split())
m = int(sys.stdin.readline())
arr = sorted([tuple(map(int, sys.stdin.readline().split())) for _ in range(m)], key=lambda x: (x[1], x[0]))

cap = [0] * (n + 1)
res = 0
for s, e, b in arr:
    load = b
    for j in range(s, e):
        load = min(load, c - cap[j])
        if load == 0: break
    else:
        for j in range(s, e):
            cap[j] += load
        res += load
print(res)
