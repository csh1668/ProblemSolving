def det(a, b):
    return a[0] * b[1] - a[1] * b[0]


def ccw(a, b, c):
    a_to_c = (a[0] - c[0], a[1] - c[1])
    b_to_c = (b[0] - c[0], b[1] - c[1])
    d = det(a_to_c, b_to_c)
    return 1 if d > 0 else (-1 if d < 0 else 0)


def on_line(a, b, c):
    return c[0] >= min(a[0], b[0]) and c[0] <= max(a[0], b[0]) and c[1] >= min([a[1], b[1]]) and c[1] <= max(a[1], b[1])


def line_intersect(a, b, c, d):
    d1 = ccw(c, d, a)
    d2 = ccw(c, d, b)
    d3 = ccw(a, b, c)
    d4 = ccw(a, b, d)
    if d1 * d2 < 0 and d3 * d4 < 0:
        return True
    if d1 == 0 and on_line(c, d, a):
        return True
    if d2 == 0 and on_line(c, d, b):
        return True
    if d3 == 0 and on_line(a, b, c):
        return True
    if d4 == 0 and on_line(a, b, d):
        return True


n = int(input())
lines = [list(map(int, input().split())) for _ in range(n)]
lines.sort()
parents = [i for i in range(n)]


def find(element):
    if parents[element] == element:
        return element
    parents[element] = find(parents[element])
    return parents[element]


def union(x, y):
    x_parent, y_parent = find(x), find(y)
    if x_parent < y_parent:
        parents[y_parent] = x_parent
    else:
        parents[x_parent] = y_parent


for i in range(n):
    for j in range(n):
        if i == j:
            continue
        ax1, ay1, ax2, ay2 = lines[i]
        bx1, by1, bx2, by2 = lines[j]
        p1, p2, p3, p4 = (ax1, ay1), (ax2, ay2), (bx1, by1), (bx2, by2)
        if line_intersect(p1, p2, p3, p4):
            union(i, j)
            # print('union', i, j)

for i in range(n):
    parents = [find(element) for element in parents]

counter = dict()
for parent in parents:
    if parent not in counter.keys():
        counter[parent] = 1
    else:
        counter[parent] += 1

print(len(counter))
print(max(counter.values()))