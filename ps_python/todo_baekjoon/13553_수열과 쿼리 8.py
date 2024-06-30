n, k = map(int, input().split())
sqrt_n = int(n ** 0.5)
arr = list(map(int, input().split()))
m = int(input())
queries = [list(map(lambda x: int(x) - 1, input().split())) for _ in range(m)]


def add(k):
    count_of[arr[k]] += 1


def sub(k):
    count_of[arr[k]] -= 1


count_of = [0] * 100001
ans = 0
x = y = 0
for idx, q in enumerate(sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1]))):
    left, right = q
    # 최초 초기화
    if idx == 0:
        x, y = left, left - 1
    while y < right:
        y += 1
        add(y)
    while y > right:
        sub(y)
        y -= 1
    while x > left:
        x -= 1
        add(x)
    while x < left:
        sub(x)
        x += 1
    print(q, count_of[:66])