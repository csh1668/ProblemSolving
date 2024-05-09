import math
import sys

n = int(sys.stdin.readline())
sqrt_n = int(math.sqrt(n))
arr = list(map(int, sys.stdin.readline().split()))
for i in range(n):
    arr[i] = [i + 1, arr[i]]
arr.sort(key=lambda x: (x[0] // sqrt_n, -x[1] if (x[0] // sqrt_n) & 1 else x[1]))
for e in arr:
    print(*e)