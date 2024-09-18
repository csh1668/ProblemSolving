import random
import sys

sys.stdout = open('test.txt', 'w')
print(100, 10000)
for i in range(100):
    print(1, i+1, 10000)

# l1 = open('out1.txt', 'r').readlines()
# l2 = open('out2.txt', 'r').readlines()
# inp = open('test.txt', 'r').readlines()
# q = inp[3:]
# arr = list(map(int, inp[1].split()))
#
# for i, (a, b) in enumerate(zip(l1, l2)):
#     if a != b:
#         x, y = map(int, q[i].split())
#         print(i, str.strip(a), str.strip(b), x, y)
#         res = sys.maxsize
#         s = sorted(arr[x:y])
#         for i in range(len(s) - 1):
#             res = min(res, s[i + 1] - s[i])
#         print(res)
#         input()