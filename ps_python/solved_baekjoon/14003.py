import bisect
import sys
from bisect import bisect_left, bisect_right

n = int(input())
arr = list(map(int, sys.stdin.readline().split()))
dp_idx = [0]    # 0 -> arr, 1 -> idx
dp_arr = [arr[0]]
dic_idx = dict()
dic_arr = dict()
for i in range(1, n):
    if arr[i] > dp_arr[-1]:
        dp_idx.append(i)
        dp_arr.append(arr[i])
        # dp.append((arr[i], i))
    else:
        idx = bisect_left(dp_arr, arr[i])
        if idx not in dic_idx.keys():
            dic_idx[idx] = [dp_idx[idx]]
            dic_arr[idx] = [dp_arr[idx]]
        else:
            dic_idx[idx].append(dp_idx[idx])
            dic_arr[idx].append(dp_arr[idx])
        dp_idx[idx] = i
        dp_arr[idx] = arr[i]

# print(list(zip(dp_arr, dp_idx)))
for i in range(len(dp_idx) - 2, -1, -1):
    # print(i, "=====")
    others_arr = dic_arr.get(i)
    others_idx = dic_idx.get(i)
    if others_arr is None:
        continue
    if dp_idx[i] > dp_idx[i + 1]:
        idx = bisect_right(others_idx, dp_idx[i + 1] - 1) - 1
        # print(idx, list(zip(others_idx, others_arr)), (dp_arr[i], dp_idx[i]), (dp_arr[i + 1], dp_idx[i+ 1]))
        dp_idx[i] = others_idx[idx]
        dp_arr[i] = others_arr[idx]


print(len(dp_idx))
# print(dp)
print(*dp_arr)