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
        self.tree[i] = self.make_segment_tree(left, mid, i * 2) + self.make_segment_tree(mid + 1, right, i * 2 + 1)
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
        return self.search(start, mid, left, right, i * 2) + self.search(mid + 1, end, left, right, i * 2 + 1)

    def update_from_leaf(self, i, val):
        idx = self.location[i]
        self.tree[idx] = val
        while idx > 1:
            idx //= 2
            self.tree[idx] = self.tree[idx * 2] + self.tree[idx * 2 + 1]


# sys.stdin = open('tools/testcase.txt', 'r')
n = int(input())
arr = list(map(int, input().split()))
m = int(input())
seg = ArraySegmentTree(arr)
change_queries = []
print_queries = []
order = 0
for _ in range(m):
    a, *b = map(int, sys.stdin.readline().split())
    if a == 1:
        change_queries.append((b[0] - 1, b[1]))
    else:
        print_queries.append([b[0], b[1] - 1, b[2] - 1, order])
        order += 1

print_queries.sort()
p_idx = 0
while len(print_queries) > p_idx and print_queries[p_idx][0] == 0:
    _, a, b, _ = print_queries[p_idx]
    print_queries[p_idx].append(seg.search(0, seg.n - 1, a, b))
    p_idx += 1

for c_idx in range(len(change_queries)):
    i, v = change_queries[c_idx]
    seg.update_from_leaf(i, v)
    while len(print_queries) > p_idx and print_queries[p_idx][0] == c_idx + 1:
        _, a, b, _ = print_queries[p_idx]
        print_queries[p_idx].append(seg.search(0, seg.n - 1, a, b))
        p_idx += 1

print_queries.sort(key=lambda x: x[3])
for print_query in print_queries:
    print(print_query[-1])
# https://www.acmicpc.net/problem/16978
# test_case_maker.py
# import random
# import sys
#
# sys.stdout = open('testcase.txt', 'w')
# MAX = 100000
# print(MAX)
# min_int, max_int = 1, 1000000
# print(*[random.randint(min_int, max_int) for _ in range(MAX)])
# cnt_change = 0
# print(100000)
# for _ in range(100000):
#     a = random.randint(1, 2)
#     if a == 1:
#         print(a, random.randint(0, MAX - 1), random.randint(min_int, max_int))
#         cnt_change += 1
#     else:
#         b = random.randint(1, 100000 - 1)
#         c = random.randint(0, b - 1)
#         print(a, random.randint(0, cnt_change), c, b)