rule = [1, 2, 4, 8, 1, 3, 6, 1, 2, 5]

cnt = 0
for n in range(0, 255 + 1):
    p = pow(2, n)
    # for num in range(1, n + 1):
    #     num = pow(2, num)
    #     if str(num)[0] == '4':
    #         cnt += 1
    s = str(p)
    if s[0] == '4':
        cnt += 1
    print(n, len(s), s[0], rule[n % len(rule)] == int(s[0]), cnt)