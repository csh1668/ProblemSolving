n, m, k = map(int, input().split())
arr = list(map(int, input().split()))


def count(cur):
    cnt = 1
    prev = arr[0]
    for i in range(1, k):
        if arr[i] - prev >= cur:
            cnt += 1
            prev = arr[i]
    return cnt


s, e, res = 0, arr[-1] - arr[0], -1
while s <= e:
    mid = s + e >> 1
    c = count(mid)
    if c < m:
        e = mid - 1
    else:
        res = mid
        s = mid + 1
    # print(mid)

ans = '1'
c = 1
p = arr[0]
for i in range(1, k):
    if arr[i] - p >= res and c < m:
        ans += '1'
        c += 1
        p = arr[i]
    else:
        ans += '0'

print(ans)