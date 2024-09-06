import sys
from collections import deque

dim = list(map(int, sys.stdin.readline().split()))
arr = [[[[[[[[[[list(map(int, sys.stdin.readline().split())) for _ in range(dim[1])] for _ in range(dim[2])] for _ in range(dim[3])] for _ in range(dim[4])] for _ in range(dim[5])] for _ in range(dim[6])] for _ in range(dim[7])] for _ in range(dim[8])] for _ in range(dim[9])] for _ in range(dim[10])]

q = deque()
for i10 in range(dim[10]):
    for i9 in range(dim[9]):
        for i8 in range(dim[8]):
            for i7 in range(dim[7]):
                for i6 in range(dim[6]):
                    for i5 in range(dim[5]):
                        for i4 in range(dim[4]):
                            for i3 in range(dim[3]):
                                for i2 in range(dim[2]):
                                    for i1 in range(dim[1]):
                                        for i0 in range(dim[0]):
                                            if arr[i10][i9][i8][i7][i6][i5][i4][i3][i2][i1][i0] == 1:
                                                q.append((0, (i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10)))

delta = []
for i in range(11):
    tmp = [0] * 11
    tmp[i] = 1
    delta.append(tmp)
    tmp = tmp[:]
    tmp[i] = -1
    delta.append(tmp)

res = 0
while q:
    d, (i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10) = q.popleft()
    res = d
    for (d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10) in delta:
        n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10 = i0 + d0, i1 + d1, i2 + d2, i3 + d3, i4 + d4, i5 + d5, i6 + d6, i7 + d7, i8 + d8, i9 + d9, i10 + d10
        if 0 <= n0 < dim[0] and 0 <= n1 < dim[1] and 0 <= n2 < dim[2] and 0 <= n3 < dim[3] and 0 <= n4 < dim[4] and 0 <= n5 < dim[5] and 0 <= n6 < dim[6] and 0 <= n7 < dim[7] and 0 <= n8 < dim[8] and 0 <= n9 < dim[9] and 0 <= n10 < dim[10] and arr[n10][n9][n8][n7][n6][n5][n4][n3][n2][n1][n0] == 0:
            q.append((d + 1, (n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)))
            arr[n10][n9][n8][n7][n6][n5][n4][n3][n2][n1][n0] = 1

cnt = 0
for i10 in range(dim[10]):
    for i9 in range(dim[9]):
        for i8 in range(dim[8]):
            for i7 in range(dim[7]):
                for i6 in range(dim[6]):
                    for i5 in range(dim[5]):
                        for i4 in range(dim[4]):
                            for i3 in range(dim[3]):
                                for i2 in range(dim[2]):
                                    for i1 in range(dim[1]):
                                        cnt += arr[i10][i9][i8][i7][i6][i5][i4][i3][i2][i1].count(0)

print(res if not cnt else -1)