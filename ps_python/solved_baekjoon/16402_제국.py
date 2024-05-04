def find(x):
    tmp = x
    parent = kingdoms[x]
    while parent != x:
        x = parent
        parent = kingdoms[parent]
    if kingdoms[tmp] != parent:
        kingdoms[tmp] = parent
    return parent


def make_subject(parent, child):
    parent_idx = kingdom_names[parent]
    child_idx = kingdom_names[child]
    parent_of_parent = find(parent_idx)
    if parent_of_parent == child_idx:
        kingdom_names[parent], kingdom_names[child] = child_idx, parent_idx
    else:
        kingdoms[find(child_idx)] = parent_idx


n, m = map(int, input().split())
kingdoms = [i for i in range(n)]
kingdom_names = dict()
for i in range(n):
    kingdom_names[input().split()[-1]] = i
for _ in range(m):
    war_result = input().split(',')
    a, b, who = war_result[0].split()[-1], war_result[1].split()[-1], war_result[-1]
    if who == '1':
        make_subject(a, b)
    else:
        make_subject(b, a)

kingdoms = [find(k) for k in kingdoms]

res = set(kingdoms)
print(len(res))
for k in sorted(kingdom_names.keys()):
    if find(kingdom_names[k]) == kingdom_names[k]:
        print('Kingdom of ' + k)