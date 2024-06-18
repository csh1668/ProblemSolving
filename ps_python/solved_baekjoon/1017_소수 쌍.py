sieve = [True] * 2001
sieve[0] = sieve[1] = False
for i in range(2, int(2001 ** 0.5) + 1):
    if sieve[i]:
        j = 2
        while i * j < 2001:
            sieve[i * j] = False
            j += 1

n = int(input())
arr = list(map(int, input().split()))
# print(arr)


def match_num(x):
    if ever[x]:
        return False
    ever[x] = True
    for y, v in enumerate(graph):
        if not sieve[v + graph[x]]:
            continue
        if match[y] == -1 or match_num(match[y]):
            match[y] = x
            return True
    return False


ans = []
for i in range(1, n):
    if sieve[arr[0] + arr[i]] is False:
        continue

    graph = [num for num in filter(lambda x: x != arr[0] and x != arr[i], arr)]
    match = [-1] * len(graph)
    for j in range(len(graph)):
        ever = [False] * len(graph)
        match_num(j)

    if all(map(lambda x: x >= 0, match)):
        # print(i, match)
        ans.append(arr[i])

if ans:
    print(*sorted(ans))
else:
    print(-1)