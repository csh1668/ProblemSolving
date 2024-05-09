import math
import sys


def seg_rec(left, right, idx):
    if left == right:
        seg[idx] = arr[left]
        return seg[idx]
    mid = (left + right) // 2
    seg[idx] = min(seg_rec(left, mid, idx * 2), seg_rec(mid + 1, right, idx * 2 + 1))
    return seg[idx]


def query_rec(start, end, idx, left, right):
    if left > end or right < start:
        return 1000000000
    if left <= start and right >= end:
        return seg[idx]
    mid = (start + end) // 2
    return min(query_rec(start, mid, idx * 2, left, right), query_rec(mid + 1, end, idx * 2 + 1, left, right))


n, m = map(int, input().split())
arr = [int(sys.stdin.readline()) for _ in range(n)]
height = math.ceil(math.log(len(arr), 2))
size = pow(2, height + 1) - 1
seg = [0] + [1000000000] * size
seg_rec(0, len(arr) - 1, 1)
for _ in range(m):
    a, b = map(int, sys.stdin.readline().split())
    print(query_rec(0, len(arr) - 1, 1, a - 1, b-1))
