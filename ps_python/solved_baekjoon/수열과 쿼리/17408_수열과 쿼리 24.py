import math
import sys


class ArraySegmentTree:
    def __init__(self, init_arr):
        self.n = len(init_arr)
        self.init_arr = init_arr
        self.tree = [0] * pow(2, math.ceil(math.log(self.n, 2) + 1))
        self.location = [0] * pow(2, math.ceil(math.log(self.n, 2) + 1))
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
        left_seg = self.make_segment_tree(left, mid, i * 2)
        right_seg = self.make_segment_tree(mid + 1, right, i * 2 + 1)
        if isinstance(left_seg, int) and isinstance(right_seg, int):
            self.tree[i] = [left_seg, right_seg]
        elif isinstance(left_seg, int):
            tmp = [left_seg, right_seg[0], right_seg[1]]
            tmp.sort(reverse=True)
            self.tree[i] = [tmp[0], tmp[1]]
        elif isinstance(right_seg, int):
            tmp = [left_seg[0], left_seg[1], right_seg]
            tmp.sort(reverse=True)
            self.tree[i] = [tmp[0], tmp[1]]
        else:
            tmp = [left_seg[0], left_seg[1], right_seg[0], right_seg[1]]
            tmp.sort(reverse=True)
            self.tree[i] = [tmp[0], tmp[1]]
        return self.tree[i]

    def search(self, start, end, left, right, i=1):
        # 범위를 벗어나면 기본값 리턴
        if end < left or start > right:
            return 0
        # 찾는 범위 왼쪽이 구간 왼쪽보다 작거나 같고,
        # 찾는 범위 오른쪽이 구간 오른쪽보다 크거나 같으면 구간값 리턴
        if left <= start and end <= right:
            return self.tree[i]
        mid = (start + end) // 2
        left_seg = self.search(start, mid, left, right, i * 2)
        right_seg = self.search(mid + 1, end, left, right, i * 2 + 1)
        if isinstance(left_seg, int) and isinstance(right_seg, int):
            return [left_seg, right_seg]
        elif isinstance(left_seg, int):
            tmp = [left_seg, right_seg[0], right_seg[1]]
            tmp.sort(reverse=True)
            return [tmp[0], tmp[1]]
        elif isinstance(right_seg, int):
            tmp = [left_seg[0], left_seg[1], right_seg]
            tmp.sort(reverse=True)
            return [tmp[0], tmp[1]]
        else:
            tmp = [left_seg[0], left_seg[1], right_seg[0], right_seg[1]]
            tmp.sort(reverse=True)
            return [tmp[0], tmp[1]]

    def update_from_leaf(self, i, val):
        idx = self.location[i]
        self.tree[idx] = val
        while idx > 1:
            idx //= 2
            left_seg = self.tree[idx * 2]
            right_seg = self.tree[idx * 2 + 1]
            if isinstance(left_seg, int) and isinstance(right_seg, int):
                self.tree[idx] = [left_seg, right_seg]
            elif isinstance(left_seg, int):
                tmp = [left_seg, right_seg[0], right_seg[1]]
                tmp.sort(reverse=True)
                self.tree[idx] = [tmp[0], tmp[1]]
            elif isinstance(right_seg, int):
                tmp = [left_seg[0], left_seg[1], right_seg]
                tmp.sort(reverse=True)
                self.tree[idx] = [tmp[0], tmp[1]]
            else:
                tmp = [left_seg[0], left_seg[1], right_seg[0], right_seg[1]]
                tmp.sort(reverse=True)
                self.tree[idx] = [tmp[0], tmp[1]]


n = int(input())
arr = list(map(int, input().split()))
m = int(input())

seg = ArraySegmentTree(arr)
for _ in range(m):
    a, b, c = map(int, sys.stdin.readline().split())
    if a == 1:
        seg.update_from_leaf(b - 1, c)
    else:
        print(sum(seg.search(0, n - 1, b - 1, c - 1)))