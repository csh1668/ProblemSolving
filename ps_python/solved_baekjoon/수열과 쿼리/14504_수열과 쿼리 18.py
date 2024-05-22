import math
import sys


class Node:
    def __init__(self, r_min, r_sum, r_max):
        self.r_min = r_min
        self.r_sum = r_sum
        self.r_max = r_max

    def __repr__(self):
        return f'node: min:{self.r_min}, max:{self.r_max}, sum:{self.r_sum}'


def floor(a, b):
    return a // b


def combine(a: Node, b: Node):
    if a is None:
        return b
    elif b is None:
        return a
    return Node(min(a.r_min, b.r_min), a.r_sum + b.r_sum, max(a.r_max, b.r_max))


class ArraySegmentTree:
    def __init__(self, init_arr):
        n = len(init_arr)
        self.init_arr = init_arr
        self.tree = [None for _ in range(pow(2, math.ceil(math.log(n, 2) + 1)))]
        self.lazy1 = [0] * pow(2, math.ceil(math.log(n, 2) + 1))
        self.lazy2 = [-sys.maxsize] * pow(2, math.ceil(math.log(n, 2) + 1))
        self.make_segment_tree(0, n - 1, 1)

    def make_segment_tree(self, left, right, i=1):
        if left == right:
            # 리프 노드라면 자기 자신 저장
            self.tree[i] = Node(self.init_arr[left], self.init_arr[left], self.init_arr[left])
            return self.tree[i]
        mid = (left + right) // 2
        # segment에 대한 연산
        left_seg = self.make_segment_tree(left, mid, i * 2)
        right_seg = self.make_segment_tree(mid + 1, right, i * 2 + 1)
        self.tree[i] = combine(left_seg, right_seg)
        return self.tree[i]

    def propagate_lazy(self, start, end, i):
        if self.lazy2[i] <= -sys.maxsize:
            self.tree[i].r_max += self.lazy1[i]
            self.tree[i].r_min += self.lazy1[i]
            self.tree[i].r_sum += (end - start + 1) * self.lazy1[i]
            if start != end:
                self.lazy1[i << 1] += self.lazy1[i]
                self.lazy1[i << 1 | 1] += self.lazy1[i]
        else:
            self.tree[i].r_max = self.lazy1[i] + self.lazy2[i]
            self.tree[i].r_min = self.lazy1[i] + self.lazy2[i]
            self.tree[i].r_sum = (end - start + 1) * (self.lazy1[i] + self.lazy2[i])
            if start != end:
                self.lazy1[i << 1] = self.lazy1[i]
                self.lazy1[i << 1 | 1] = self.lazy1[i]
                self.lazy2[i << 1] = self.lazy2[i]
                self.lazy2[i << 1 | 1] = self.lazy2[i]
        self.lazy1[i], self.lazy2[i] = 0, -sys.maxsize


    def search(self, start, end, left, right, mode, i=1):
        # lazy하게 업데이트
        self.propagate_lazy(start, end, i)
        # 범위를 벗어나면 기본값 리턴
        if end < left or start > right:
            return sys.maxsize if mode == 3 else 0
        # 찾는 범위 왼쪽이 구간 왼쪽보다 작거나 같고,
        # 찾는 범위 오른쪽이 구간 오른쪽보다 크거나 같으면 구간값 리턴
        if left <= start and end <= right:
            return self.tree[i].r_min if mode == 3 else self.tree[i].r_sum
        mid = (start + end) // 2
        left_seg = self.search(start, mid, left, right, mode, i * 2)
        right_seg = self.search(mid + 1, end, left, right, mode, i * 2 + 1)
        return min(left_seg, right_seg) if mode == 3 else left_seg + right_seg

    def add_range(self, start, end, left, right, diff, i=1):
        self.propagate_lazy(start, end, i)
        if end < left or start > right:
            return
        if left <= start and end <= right:
            self.lazy1[i] = diff
            self.propagate_lazy(start, end, i)
            return
        mid = (start + end) // 2
        self.add_range(start, mid, left, right, diff, i * 2)
        self.add_range(mid + 1, end, left, right, diff, i * 2 + 1)
        self.tree[i] = combine(self.tree[i * 2], self.tree[i * 2 + 1])

    def div_range(self, start, end, left, right, div, i=1):
        self.propagate_lazy(start, end, i)
        if end < left or start > right:
            return
        if left <= start and end <= right:
            floor_min = self.tree[i].r_min // div
            if floor_min == self.tree[i].r_max // div:
                self.lazy2[i] = floor_min
                self.propagate_lazy(start, end, i)
                return
            if self.tree[i].r_min + 1 == self.tree[i].r_max:
                self.lazy1[i] = floor_min - self.tree[i].r_min
                self.propagate_lazy(start, end, i)
                return
        if start == end:
            return
        mid = (start + end) // 2
        self.div_range(start, mid, left, right, div, i * 2)
        self.div_range(mid + 1, end, left, right, div, i * 2 + 1)
        self.tree[i] = combine(self.tree[i * 2], self.tree[i * 2 + 1])


# sys.stdin = open('tools/testcase.txt', 'r')
# sys.stdout = open('tools/output2.txt', 'w')

n, q = map(int, input().split())
arr = list(map(int, input().split()))
seg = ArraySegmentTree(arr)
for i in range(q):
    mode, *args = map(int, sys.stdin.readline().split())
    if mode == 1:
        seg.add_range(0, n - 1, args[0], args[1], args[2])
    elif mode == 2:
        seg.div_range(0, n - 1, args[0], args[1], args[2])
    else:
        print(seg.search(0, n - 1, args[0], args[1], mode))