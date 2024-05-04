import math
import sys


def seg_rec(left, right, idx):
    if left == right:
        seg[idx] = arr[left]
        return seg[idx]
    mid = (left + right) // 2
    seg[idx] = seg_rec(left, mid, idx * 2) + seg_rec(mid + 1, right, idx * 2 + 1)
    return seg[idx]


def query_rec(start, end, idx, left, right):
    if left > end or right < start:
        return 0
    if left <= start and right >= end:
        return seg[idx]
    mid = (start + end) // 2
    return query_rec(start, mid, idx * 2, left, right) + query_rec(mid + 1, end, idx * 2 + 1, left, right)


def update(start, end, node, idx, val):
    if start > idx or idx > end:
        return seg[node]
    seg[node] += val
    if start != end:
        mid = (start + end) // 2
        update(start, mid, node * 2, idx, val)
        update(mid + 1, end, node * 2 + 1, idx, val)


n, m, k = map(int, input().split())
arr = [int(sys.stdin.readline()) for _ in range(n)]
height = math.ceil(math.log(len(arr), 2))
size = pow(2, height + 1) - 1
seg = [0] + [0] * size
seg_rec(0, len(arr) - 1, 1)
for _ in range(m + k):
    a, b, c = map(int, sys.stdin.readline().split())
    if a == 1:
        update(0, len(arr) - 1, 1, b - 1, c - arr[b - 1])
        arr[b - 1] = c
    else:
        print(query_rec(0, len(arr) - 1, 1, b - 1, c - 1))
