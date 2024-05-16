
import math
import sys

class ArraySegmentTree:
    def __init__(self, init_arr, cmp_func, init_val):
        n = len(init_arr)
        self.n = n
        self.init_arr = init_arr
        self.init_val = init_val
        self.tree = [init_val for _ in range(pow(2, math.ceil(math.log(n, 2) + 1)))]
        self.location = [0] * n
        self.cmp_func = cmp_func
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
        self.tree[i] = self.cmp_func(self.make_segment_tree(left, mid, i * 2),
                                     self.make_segment_tree(mid + 1, right, i * 2 + 1))
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
        return self.cmp_func(self.search(start, mid, left, right, i * 2),
                             self.search(mid + 1, end, left, right, i * 2 + 1))

    def update_from_leaf(self, i, val):
        idx = self.location[i]
        self.tree[idx] = val
        while idx > 1:
            idx //= 2
            self.tree[idx] = self.cmp_func(self.tree[idx * 2], self.tree[idx * 2 + 1])


class Node:
    def __init__(self, left, right, mid, overall, start_v, end_v, left_v, right_v, mid_left_v, mid_right_v):
        self.mid_right_v = mid_right_v
        self.mid_left_v = mid_left_v
        self.right_v = right_v
        self.left_v = left_v
        self.left = left
        self.right = right
        self.mid = mid
        self.overall = overall
        self.start_v = start_v
        self.end_v = end_v


def combine(n1: Node, n2: Node):
    start_v = n1.start_v
    end_v = n2.end_v

    left = n1.left
    left_v = n1.left_v
    if n1.end_v != n2.start_v and left < n1.overall + n2.left:
        left = n1.overall + n2.left
        left_v = n2.left_v

    right = n2.right
    right_v = n2.right_v
    if n1.end_v != n2.start_v and right < n1.right + n2.overall:
        right = n1.right + n2.overall
        right_v = n1.right_v

    mid = n1.mid
    mid_left_v, mid_right_v = n1.mid_left_v, n1.mid_right_v
    if mid < n2.mid:
        mid = n2.mid
        mid_left_v, mid_right_v = n2.mid_left_v, n2.mid_right_v
    if n1.mid_right_v != n2.mid_left_v and mid < n1.mid + n2. mid:
        mid = n1.mid + n2.mid
        mid_left_v, mid_right_v = n1.mid_left_v, n2.mid_right_v

    overall = n1.overall + n2.overall if n1.end_v != n2.start_v else 0
    return Node(left, right, mid, overall, start_v, end_v, left_v, right_v, mid_left_v, mid_right_v)


n, q = map(int, input().split())
l_node = Node(1, 1, 1, 1, 'L', 'L', 'L', 'L', 'L', 'L')
r_node = Node(1, 1, 1, 1, 'R', 'R', 'R', 'R', 'R', 'R')
init_arr = [l_node] * n
init_val = Node(0, 0, 0, 0, None, None, None, None, None, None)
seg = ArraySegmentTree(init_arr, combine, init_val)
for _ in range(q):
    idx = int(sys.stdin.readline()) - 1
    orig: Node = seg.tree[seg.location[idx]]
    if orig.start_v == 'R':
        seg.update_from_leaf(idx, l_node)
    else:
        seg.update_from_leaf(idx, r_node)
    res: Node = seg.search(0, n - 1, 0, n - 1)
    ans = max(res.left, res.right, res.mid, res.overall)
    print(ans)

# 문제점:
# '연속함'을 구현하지 않았음