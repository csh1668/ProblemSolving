import sys

n = int(sys.stdin.readline())
frequency = 1
if n > 1000:
    frequency += 1
if n > 5000:
    frequency += 1
if n > 7500:
    frequency += 1
arr = list(map(int, sys.stdin.readline().split()))
a = b = 1
all_costs = 0
for i in range(n):
    arr[i] = [i - a + arr[i] - b, i + 1, arr[i]]
arr.sort(key=lambda x: -x[0])
buffer = 0
while len(arr) > 0:
    cost, old, new = arr.pop()
    a, b = old, new
    print(old, new)
    buffer += 1
    if buffer == frequency:
        for i in range(len(arr)):
            arr[i][0] = abs(arr[i][1] - a) + abs(arr[i][2] - b)
        buffer = 0
        arr.sort(key=lambda x: -x[0])