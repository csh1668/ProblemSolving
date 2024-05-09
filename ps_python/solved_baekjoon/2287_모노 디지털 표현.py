def repeat_e(number: int, times: int):
    sum = 0
    while times != 0:
        sum += number * (10 ** (times - 1))
        times -= 1
    return sum


e = int(input())
t = int(input())
cache = [{repeat_e(e, (i + 1))} for i in range(8)]
for i in range(1, len(cache)):
    for j in range(i):
        for op1 in cache[j]:
            for op2 in cache[i - j - 1]:
                cache[i].add(op1 + op2)
                cache[i].add(op1 - op2)
                cache[i].add(op1 * op2)
                if op2 != 0:
                    cache[i].add(op1 // op2)

for _ in range(t):
    n = int(input())
    for i in range(len(cache)):
        if n in cache[i]:
            print(i + 1)
            break
    else:
        print("NO")