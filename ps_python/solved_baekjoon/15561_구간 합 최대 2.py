import math
import sys


class ArraySegmentTree:
    def __init__(self, init_arr, combine_func, init_val):
        n = len(init_arr)
        self.n = n
        self.init_arr = init_arr
        self.init_val = init_val
        self.tree = [init_val for _ in range(pow(2, math.ceil(math.log(n, 2) + 1)))]
        self.location = [0] * n
        self.combine_func = combine_func
        self.make_segment_tree(0, n - 1, 1)

    def make_segment_tree(self, left, right, i=1):
        if left == right:
            # 리프 노드라면 자기 자신 저장
            self.tree[i] = self.init_arr[left]
            # 리프 노드의 인덱스 저장
            self.location[left] = i
            return self.tree[i]
        mid = (left + right) // 2
        # segment에 대한 연산
        self.tree[i] = self.combine_func(self.make_segment_tree(left, mid, i * 2), self.make_segment_tree(mid + 1, right, i * 2 + 1))
        return self.tree[i]

    def search(self, start, end, left, right, i=1):
        # 범위를 벗어나면 기본값 리턴
        if end < left or start > right:
            return self.init_val
        # 찾는 범위 왼쪽이 구간 왼쪽보다 작거나 같고,
        # 찾는 범위 오른쪽이 구간 오른쪽보다 크거나 같으면 구간값 리턴
        if left <= start and end <= right:
            return self.tree[i]
        mid = (start + end) // 2
        return self.combine_func(self.search(start, mid, left, right, i * 2), self.search(mid + 1, end, left, right, i * 2 + 1))

    def update_from_leaf(self, i, val):
        idx = self.location[i]
        self.tree[idx] = val
        while idx > 1:
            idx //= 2
            self.tree[idx] = self.combine_func(self.tree[idx * 2], self.tree[idx * 2 + 1])


class Node:
    def __init__(self, l_val, r_val, mid_val, p_sum, start_idx, end_idx, l_idx, r_idx, mid_l_idx, mid_r_idx):
        self.mid_r_idx = mid_r_idx
        self.mid_l_idx = mid_l_idx
        self.r_idx = r_idx
        self.l_idx = l_idx
        self.end_idx = end_idx
        self.start_idx = start_idx
        self.l_val = l_val
        self.r_val = r_val
        self.mid_val = mid_val
        self.p_sum = p_sum

    def __str__(self):
        return f'l_val: {self.l_val}, r_val: {self.r_val}, mid_val: {self.mid_val}, p_sum: {self.p_sum}, start_idx: {self.start_idx}, end_idx: {self.end_idx}, l_idx: {self.l_idx}, r_idx: {self.r_idx}, mid_l_idx: {self.mid_l_idx}, mid_r_idx: {self.mid_r_idx}'


def combine(n1: Node, n2: Node):
    start_idx = n1.start_idx
    end_idx = n2.end_idx
    if n1.l_val + (n1.l_idx - n1.start_idx) * v > n1.p_sum + n2.l_val + (n2.l_idx - n1.start_idx) * v:
        l_val = n1.l_val
        l_idx = n1.l_idx
    else:
        l_val = n1.p_sum + n2.l_val
        l_idx = n2.l_idx
    if n2.r_val + (n2.end_idx - n2.r_idx) * v > n1.r_val + n2.p_sum + (n2.end_idx - n1.r_idx) * v:
        r_val = n2.r_val
        r_idx = n2.r_idx
    else:
        r_val = n1.r_val + n2.p_sum
        r_idx = n1.r_idx
    if n1.mid_val + (n1.mid_r_idx - n1.mid_l_idx) * v > n2.mid_val + (n2.mid_r_idx - n2.mid_l_idx) * v:
        if n1.mid_val + (n1.mid_r_idx - n1.mid_l_idx) * v > n1.r_val + n2.l_val + (n2.l_idx - n1.r_idx) * v:
            mid_val = n1.mid_val
            mid_l_idx, mid_r_idx = n1.mid_l_idx, n1.mid_r_idx
        else:
            mid_val = n1.r_val + n2.l_val
            mid_l_idx, mid_r_idx = n1.r_idx, n2.l_idx
    else:
        if n2.mid_val + (n2.mid_r_idx - n2.mid_l_idx) * v > n1.r_val + n2.l_val + (n2.l_idx - n1.r_idx) * v:
            mid_val = n2.mid_val
            mid_l_idx, mid_r_idx = n2.mid_l_idx, n2.mid_r_idx
        else:
            mid_val = n1.r_val + n2.l_val
            mid_l_idx, mid_r_idx = n1.r_idx, n2.l_idx
    p_sum = n1.p_sum + n2.p_sum
    return Node(l_val, r_val, mid_val, p_sum, start_idx, end_idx, l_idx, r_idx, mid_l_idx, mid_r_idx)


n, q, u, v = map(int, input().split())
arr = list(map(int, input().split()))
for i in range(len(arr)):
    arr[i] = Node(u * arr[i], u * arr[i], u * arr[i], u * arr[i], i + 1, i + 1, i + 1, i + 1, i + 1, i + 1)
MIN = -pow(10, 15)
seg = ArraySegmentTree(arr, combine, Node(MIN, MIN, MIN, MIN, 0, 0, 0, 0, 0, 0))

for _ in range(q):
    a, i, j = map(int, sys.stdin.readline().split())
    if a == 0:
        res: Node = seg.search(0, seg.n - 1, i - 1, j - 1)
        # print(res)
        res = max(res.l_val + (res.l_idx - res.start_idx) * v, res.r_val + (res.end_idx - res.r_idx) * v, res.mid_val + (res.mid_r_idx - res.mid_l_idx) * v, res.p_sum + (res.end_idx - res.start_idx) * v)
        print(res)
    else:
        seg.update_from_leaf(i - 1, Node(u * j, u * j, u * j, u * j, i, i, i, i, i, i))