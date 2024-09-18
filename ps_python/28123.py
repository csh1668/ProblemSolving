rule = [1, 2, 4, 8, 1, 3, 6, 1, 2, 5]

cnt = 0
for n in range(0, 10000 + 1):
    p = pow(2, n)
    # for num in range(1, n + 1):
    #     num = pow(2, num)
    #     if str(num)[0] == '4':
    #         cnt += 1
    s = str(p)
    if s[0] == '4':
        cnt += 1
    if int(s[0]) == 4:
        print(f'\033[92m{s[0]}\033[0m', end=' ' if n % len(rule) != 0 else f' {cnt}:{n // 10}:{len(s)}\n')
    elif int(s[0]) != rule[n % len(rule)]:
        print(f'\033[93m{s[0]}\033[0m', end=' ' if n % len(rule) != 0 else f' {cnt}:{n // 10}:{len(s)}\n')
    else:
        print(s[0], end=' ' if n % len(rule) != 0 else f' {cnt}:{n // 10}:{len(s)}\n')