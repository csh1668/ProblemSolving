n = int(input())
arr = [list(sorted(map(int, input().split()))) for _ in range(4)]
indices = [0, 0, 0, 0]


def cal_diff(i, j, k, l):
    min_val = min(arr[0][i], arr[1][j], arr[2][k], arr[3][l])
    max_val = max(arr[0][i], arr[1][j], arr[2][k], arr[3][l])
    return max_val - min_val


# [print(line) for line in arr]
MAX = pow(10, 9) + 1
res = cal_diff(*indices)
while any(map(lambda x: x != n - 1, indices)):
    nxt_idx = -1
    nxt_diff = MAX
    for idx in range(4):
        if indices[idx] + 1 < n:
            indices[idx] += 1
            tmp = cal_diff(*indices)
            if nxt_diff > tmp:
                nxt_diff = tmp
                nxt_idx = idx
            indices[idx] -= 1
    indices[nxt_idx] += 1
    res = min(res, nxt_diff)
print(res)