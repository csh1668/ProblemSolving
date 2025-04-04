def is_prime(n):
    if n <= 5 or n % 2 == 0 or n % 3 == 0:
        return n == 2 or n == 3 or n == 5
    s = ((n - 1) & (1 - n)).bit_length() - 1
    d = n >> s

    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        p = pow(a, d, n)
        if p == 1 or p == n - 1 or a % n == 0: continue
        for _ in range(s):
            p = (p * p) % n
            if p == n - 1: break
        else: return False
    return True

A, B = map(int, input().split())
ans = []
for K in range(A, B + 1):
    if K % 2 == 0: continue
    if is_prime(K) or K == 9:
        ans.append(K)
print(*ans)