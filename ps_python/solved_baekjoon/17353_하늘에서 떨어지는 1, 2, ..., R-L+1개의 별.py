import sys

n = int(sys.stdin.readline())
arr = [0] + list(map(int, sys.stdin.readline().split()))
for i in range(n, 0, -1):
    arr[i] -= arr[i - 1]
q = int(sys.stdin.readline())

tree = [0] * 262144
lazy = [0] * 262144

def init(s, e, i):
    if s == e:
        tree[i] = arr[s + 1]
        return tree[i]
    m = (s + e) >> 1
    tree[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1)
    return tree[i]

def propagate(s, e, i):
    if lazy[i] != 0:
        tree[i] += (e - s + 1) * lazy[i]
        if s != e:
            lazy[i << 1] += lazy[i]
            lazy[i << 1 | 1] += lazy[i]
        lazy[i] = 0

def update(s, e, l, r, v, i=1):
    propagate(s, e, i)
    if l > e or r < s:
        return
    if l <= s and e <= r:
        lazy[i] += v
        propagate(s, e, i)
        return
    m = (s + e) >> 1
    update(s, m, l, r, v, i << 1)
    update(m + 1, e, l, r, v, i << 1 | 1)
    tree[i] = tree[i << 1] + tree[i << 1 | 1]

def get(s, e, l, r, i=1):
    propagate(s, e, i)
    if l > e or r < s:
        return 0
    if l <= s and e <= r:
        return tree[i]
    m = (s + e) >> 1
    return get(s, m, l, r, i << 1) + get(m + 1, e, l, r, i << 1 | 1)


init(0, n - 1, 1)
for _ in range(q):
    mode, *args = map(int, sys.stdin.readline().split())
    if mode == 1:
        update(0, n - 1, args[0] - 1, args[1] - 1, 1)
        if args[1] != n:
            update(0, n - 1, args[1], args[1], -(args[1] - args[0] + 1))
    else:
        print(get(0, n - 1, 0, args[0] - 1))