N = int(input())
arr = [[] for _ in range(N)]
for _ in range(3):
    for i, e in enumerate(map(int, input().split())):
        arr[e - 1].append(i)
arr.sort(key=lambda x: x[0])

sz = 1048576; offset = 524288
tree = [N + 1] * sz

def update(i, v):
    i |= offset; tree[i] = v
    while i > 1:
        i >>= 1
        tree[i] = min(tree[i << 1], tree[i << 1 | 1])

def query(l, r):
    l |= offset; r |= offset
    ret = N + 1
    while l <= r:
        if l & 1: ret = min(ret, tree[l]); l += 1
        if r & 1 == 0: ret = min(ret, tree[r]); r -= 1
        l >>= 1; r >>= 1
    return ret

ans = 0
for i in range(N):
    if query(0, arr[i][1]) > arr[i][2]: ans += 1
    update(arr[i][1], arr[i][2])
print(ans)