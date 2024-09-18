import sys

n, k = map(int, sys.stdin.readline().split())
planets = [[0, 0, 0]] + [list(map(int, sys.stdin.readline().split())) for _ in range(n)]
planets.sort(key=lambda x: x[1])
# print(planets)
ana_pos = [e for e in planets if e[0] == k][0][1]
for i in range(1, n + 1):
    planets[i][0], planets[i][1] = planets[i][0] + planets[i - 1][0], planets[i][1] + planets[i - 1][1]
    planets[i].append(planets[i][0] * planets[i][1] + planets[i - 1][2])

calc_center = lambda l, r: (planets[r][2] - planets[l - 1][2]) // (planets[r][1] - planets[l - 1][1])

l = r = ana_pos
