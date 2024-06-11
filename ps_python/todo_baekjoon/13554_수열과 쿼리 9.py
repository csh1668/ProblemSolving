import sys

sys.stdin = open('tools/testcase.txt')

n = int(sys.stdin.readline())
a = list(map(int, sys.stdin.readline().split()))
b = list(map(int, sys.stdin.readline().split()))
m = int(input())
queries = [list(map(int, sys.stdin.readline().split())) for _ in range(m)]

sq = int(100000 ** 0.5)
cnt_a = [0] * (100000 + 2)
cnt_b = [0] * (100000 + 2)
buckets_a = [0] * (sq + 2)
buckets_b = [0] * (sq + 2)


def add(k):
    va, vb = a[k], b[k]
    cnt_a[va] += 1
    buckets_a[va // sq] += 1
    cnt_b[vb] += 1
    buckets_b[vb // sq] += 1


def sub(k):
    va, vb = a[k], b[k]
    cnt_a[va] -= 1
    buckets_a[va // sq] -= 1
    cnt_b[vb] -= 1
    buckets_b[vb // sq] -= 1


x = y = 0
for idx, q in enumerate(sorted(queries, key=lambda x: (x[0] // sq, x[1]))):
    l, r, k  = q[0] - 1, q[1] - 1, q[2]
    if idx == 0:
        x = l
        y = l - 1
    if idx % 10 == 0:
        print(idx)
    while y < r:
        y += 1
        add(y)
    while y > r:
        sub(y)
        y -= 1
    while x > l:
        x -= 1
        add(x)
    while x < l:
        sub(x)
        x += 1
    ans = 0
    for i in range(sq + 2):
        if i * sq >= k:
            break
        if buckets_a[i] == 0:
            continue
        for j in range(sq + 2):
            if j * sq >= k:
                break
            if buckets_b[j] == 0:
                continue
            upper_a = (i + 1) * sq
            upper_b = (j + 1) * sq
            lower_a = upper_a - sq
            lower_b = upper_b - sq
            # TODO: 문제점: 1번 케이스인 경우가 사실상 없다
            # 범위 내 a, b 최댓값의 곱이 <= k인 경우
            if upper_a * lower_b <= k:
                ans += buckets_a[i] * buckets_b[j]
            # 최댓값은 > k이지만, 최솟값은 <= k인 경우
            # naive하게 계산
            elif lower_a * lower_b <= k:
                for i2 in range(i * sq, min((i + 1) * sq, 100001)):
                    if i2 > k:
                        break
                    if cnt_a[i2] == 0:
                        continue
                    for j2 in range(j * sq, min((j + 1) * sq, 100001)):
                        if j2 > k:
                            break
                        if cnt_b[j2] == 0:
                            continue
                        if i2 * j2 <= k:
                            ans += cnt_a[i2] * cnt_b[j2]
            else:
                break

    q.append(ans)

print(*map(lambda x: x[-1], queries), sep='\n')