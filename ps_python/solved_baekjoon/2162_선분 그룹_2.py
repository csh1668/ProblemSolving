n = int(input())
lines = [list(map(int, input().split())) for _ in range(n)]
parents = [i for i in range(n)]


def find(element):
    if element == parents[element]:
        return element
    parents[element] = find(parents[element])
    return parents[element]


def union(x, y):
    parent_x = find(x)
    parent_y = find(y)
    if parent_x > parent_y:
        parent_x, parent_y = parent_y, parent_x
    parents[parent_y] = parent_x


def line_intersect(a, b, c, d):
    if a > b:
        a, b = b, a
    if c > d:
        c, d = d, c
    a_to_b = (b[0] - a[0], b[1] - a[1])
    a_to_c = (c[0] - a[0], c[1] - a[1])
    a_to_d = (d[0] - a[0], d[1] - a[1])
    triangle1 = a_to_b[0] * a_to_c[1] - a_to_b[1] * a_to_c[0]
    triangle2 = a_to_b[0] * a_to_d[1] - a_to_b[1] * a_to_d[0]
    if triangle1 * triangle2 <= 0:
        if triangle1 == 0 and triangle2 == 0:
            return a <= c <= b or a <= d <= b or c <= a <= d or c <= b <= d
        return 1
    else:
        return 0


for i in range(n - 1):
    for j in range(i + 1, n):
        ax1, ay1, ax2, ay2 = lines[i]
        bx1, by1, bx2, by2 = lines[j]
        p1, p2, p3, p4 = (ax1, ay1), (ax2, ay2), (bx1, by1), (bx2, by2)
        if line_intersect(p1, p2, p3, p4) and line_intersect(p3, p4, p1, p2) and find(i) != find(j):
            union(i, j)
            # print('union', i, j)

for i in range(n):
    find(i)

print(len(set(parents)))
parents.sort()
res = 1
cnt = 1
for i in range(1, n):
    cnt = cnt + 1 if parents[i - 1] == parents[i] else 1
    res = max(res, cnt)
print(res)