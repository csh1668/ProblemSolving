n, k = int(input()), int(input())
arr = sorted(list(map(int, input().split())))
print(sum(sorted([arr[i + 1] - arr[i] for i in range(n - 1)])[:n - k]))