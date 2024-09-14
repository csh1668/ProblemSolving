import sys

n = int(sys.stdin.readline())
g = [[] for _ in range(n)]
for i in range(n):
    s = 0
    for j, x in enumerate(map(int, sys.stdin.readline().split())):
        if x: g[i].append((j, x))
        s += x
    if s & 1:
        print(-1)
        exit()

st = [0] * (pow(10, 6) * 5); top = 1
while top:
    cur = st[top - 1]
    if not g[cur]:
        sys.stdout.write(str(cur + 1) + ' ')
        top -= 1
    else:
        nxt, x = g[cur][-1]
        if x == 1:
            del g[cur][nxt]
            del g[nxt][cur]
        else:
            g[cur][nxt] -= 1
            g[nxt][cur] -= 1
        st[top] = nxt
        top += 1