n, k = map(int, input().split())
sqrt_n = int(n ** 0.5)
arr = list(map(int, input().split()))
m = int(input())
queries = [list(map(lambda x: int(x) - 1, input().split())) for _ in range(m)]

count_of = [0] * 100001
ans = 0
x = y = 0
for idx, q in enumerate(sorted(queries, key=lambda x: (x[0] // sqrt_n, x[1]))):
    left, right = q
    # 최초 초기화
    if idx == 0:
        x = y = left
        count_of[arr[left]] += 1
    while y < right:
        y += 1
        count_of[arr[y]] += 1
        # 연산
    while y > right:
        # 연산
        count_of[arr[y]] -= 1
        y -= 1
    while x > left:
        x -= 1
        count_of[arr[x]] -= 1
        # 연산
    while x < left:
        # 연산
        count_of[arr[x]] += 1
        x += 1
    print('done')