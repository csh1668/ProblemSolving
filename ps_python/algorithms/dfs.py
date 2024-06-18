from collections import deque


def dfs(start, neighbors_getter, parent_setter, visited_getter):
    s = deque()
    s.append(start)
    parent_setter(start, start)
    while len(s) > 0:
        cur = s.pop()
        for neighbor in neighbors_getter(cur):
            if visited_getter(neighbor) is None:
                s.append(neighbor)
                parent_setter(neighbor, cur)


# usage example
if __name__ == '__main__':
    n = 4
    relations = [[1, 1, 1, 1], [1, 1, 1, 0], [1, 1, 0, 0], [1, 0, 0, 0]]


    def get_adj(pos):
        x, y = pos
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx, ny = x + dx, y + dy
            if 0 <= nx < n and 0 <= ny < n and relations[nx][ny] == 1:
                yield nx, ny


    def set_parent(child, parent):
        parents[child] = parent


    def get_parent(child):
        return parents.get(child)


    parents = dict()
    dfs((0, 0), get_adj, set_parent, get_parent)
    print(parents)