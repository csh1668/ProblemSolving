mp = [[0] * 30 for _ in range(30)]; sm = 0

for i in range(15):
    mp[14][i] = 1; sm += 1
    mp[14][i + 15] = 15; sm += 15
    mp[i][14] = 15*15; sm += 15*15
    mp[i + 15][14] = 15*15*15; sm += 15*15*15

mp[14][14] = 0; sm -= 1
[print(*row) for row in mp]
# print(sm)