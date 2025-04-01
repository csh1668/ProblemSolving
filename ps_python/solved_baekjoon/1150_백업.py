import sys
from heapq import heappop, heappush, heapify
r = sys.stdin.readline

N, K = map(int, r().split())
A = [int(r()) for _ in range(N)]

pq = [(A[i] - A[i - 1], i) for i in range(1, N)]
D = [sys.maxsize] + [A[i] - A[i - 1] for i in range(1, N)] + [sys.maxsize]
L = [i - 1 for i in range(N + 2)]
R = [i + 1 for i in range(N + 2)]
done = [False] * (N + 2)
heapify(pq)

ans = 0
while K > 0:
    d, i = heappop(pq)
    if done[i]: continue
    ans += d
    K -= 1
    D[i] = D[L[i]] + D[R[i]] - D[i]
    heappush(pq, (D[i], i))
    done[L[i]] = done[R[i]] = True
    L[i] = L[L[i]]
    R[i] = R[R[i]]
    R[L[i]] = L[R[i]] = i

print(ans)