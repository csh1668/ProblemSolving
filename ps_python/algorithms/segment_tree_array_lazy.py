import math


class ArraySegmentTree:
    def __init__(self, init_arr):
        n = len(init_arr)
        self.init_arr = init_arr
        self.tree = [0] * pow(2, math.ceil(math.log(n, 2) + 1))
        self.lazy = [0] * pow(2, math.ceil(math.log(n, 2) + 1))
        self.make_segment_tree(0, n - 1, 1)

    def make_segment_tree(self, left, right, i=1):
        if left == right:
            # 리프 노드라면 자기 자신 저장
            self.tree[i] = self.init_arr[left]
            return self.tree[i]
        mid = (left + right) // 2
        # segment에 대한 연산
        self.tree[i] = self.make_segment_tree(left, mid, i * 2) + self.make_segment_tree(mid + 1, right, i * 2 + 1)
        return self.tree[i]

    def update_lazy(self, start, end, i):
        if self.lazy[i] != 0:
            self.tree[i] += (end - start + 1) * self.lazy[i]
            if start != end:
                self.lazy[i * 2] += self.lazy[i]
                self.lazy[i * 2 + 1] += self.lazy[i]
            self.lazy[i] = 0

    def search(self, start, end, left, right, i=1):
        # lazy하게 업데이트
        self.update_lazy(start, end, i)
        # 범위를 벗어나면 기본값 리턴
        if end < left or start > right:
            return 0
        # 찾는 범위 왼쪽이 구간 왼쪽보다 작거나 같고,
        # 찾는 범위 오른쪽이 구간 오른쪽보다 크거나 같으면 구간값 리턴
        if left <= start and end <= right:
            return self.tree[i]
        mid = (start + end) // 2
        return self.search(start, mid, left, right, i * 2) + self.search(mid + 1, end, left, right, i * 2 + 1)

    def update_range(self, start, end, left, right, i, diff):
        self.update_lazy(start, end, i)
        if end < left or start > right:
            return
        if left <= start and end <= right:
            self.tree[i] += (end - start + 1) * diff
            if start != end:
                self.lazy[i * 2] += diff
                self.lazy[i * 2 + 1] += diff
            return
        mid = (start + end) // 2
        self.update_range(start, mid, left, right, i * 2, diff)
        self.update_range(mid + 1, end, left, right, i * 2 + 1, diff)
        self.tree[i] = self.tree[i * 2] + self.tree[i * 2 + 1]