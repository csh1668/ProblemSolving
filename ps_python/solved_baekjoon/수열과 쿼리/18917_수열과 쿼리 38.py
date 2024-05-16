import sys

s = 0
xor = 0

for _ in range(int(input())):
    a, *b = map(int, sys.stdin.readline().split())
    if a == 1:
        s += b[0]
        xor ^= b[0]
    elif a == 2:
        s -= b[0]
        xor ^= b[0]
    elif a == 3:
        print(s)
    else:
        print(xor)