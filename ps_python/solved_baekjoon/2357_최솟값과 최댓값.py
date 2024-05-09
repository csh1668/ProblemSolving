import math
import sys

n, m = map(int, sys.stdin.readline().split())
arr = [int(sys.stdin.readline()) for _ in range(n)]
tree = [[0, 0] for _ in range(pow(2, math.ceil(math.log(n, 2) + 1)))]
location = [0] * n


def seg(left=0, right=n - 1, i=1):
    if left == right:
        # 리프 노드라면 자기 자신 저장
        tree[i][0], tree[i][1] = arr[left], arr[left]
        # 리프 노드의 인덱스 저장
        location[left] = i
        return tree[i]
    mid = (left + right) // 2
    # segment에 대한 연산
    left_seg, right_seg = seg(left, mid, i * 2), seg(mid + 1, right, i * 2 + 1)
    tree[i][0] = min(left_seg[0], right_seg[0])
    tree[i][1] = max(left_seg[1], right_seg[1])
    return tree[i]


def search(start, end, left, right, i=1):
    # 범위를 벗어나면 0 리턴
    if end < left or start > right:
        return [1000000001, 0]
    # 찾는 범위 왼쪽이 구간 왼쪽보다 작거나 같고,
    # 찾는 범위 오른쪽이 구간 오른쪽보다 크거나 같으면 구간값 리턴
    if left <= start and end <= right:
        return tree[i]
    mid = (start + end) // 2
    left_seg, right_seg = search(start, mid, left, right, i * 2), search(mid + 1, end, left, right, i * 2 + 1)
    return [min(left_seg[0], right_seg[0]), max(left_seg[1], right_seg[1])]


def update(i, val):
    idx = location[i]
    tree[idx][0], tree[idx][1] = val, val
    while idx > 1:
        idx //= 2
        left_child, right_child = tree[idx * 2], tree[idx * 2 + 1]
        tree[idx][0], tree[idx][1] = min(left_child[0], right_child[0]), max(left_child[1], right_child[1])


seg()
# print(tree)
for _ in range(m):
    # print(tree)
    a, b = map(int, sys.stdin.readline().split())
    print(*search(0, n - 1, a - 1, b - 1))