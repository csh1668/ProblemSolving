import sys
from collections import deque

for _ in range(int(sys.stdin.readline())):
    n = int(sys.stdin.readline())

    s = deque()
    cnt = 0
    for i, c in enumerate(sys.stdin.readline().strip()):
        if c == 'H': continue
        s.append(i)

        while len(s) > 1:
            f, b = s[0], s[-1]
            if (b - f) & 1:
                cnt += b - f
                s.popleft(), s.pop()
            else: break

    if s: print(-1)
    else: print(cnt)