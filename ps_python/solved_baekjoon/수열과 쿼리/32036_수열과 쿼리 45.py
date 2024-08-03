import sys
from heapq import heappush, heappop

left, right = [], []
left_sum = right_sum = 0
offset = 0

for _ in range(int(sys.stdin.readline())):
    op, *args = map(int, sys.stdin.readline().split())
    if op == 1:
        x, y = args
        if not left or -left[0] >= x or (right and right[0] > x):
            heappush(left, -x)
            left_sum += x
        else:
            heappush(right, x)
            right_sum += x
        if len(left) < len(right):
            tmp = heappop(right)
            right_sum -= tmp
            heappush(left, -tmp)
            left_sum += tmp
        elif len(left) > len(right) + 1:
            tmp = -heappop(left)
            left_sum -= tmp
            heappush(right, tmp)
            right_sum += tmp
        offset += y
    else:
        if left:
            cnt = len(left) + len(right)
            print(-left[0], right_sum - left_sum + (-left[0] if cnt & 1 else 0) + offset)
        else:
            print(-pow(10, 9), 0)