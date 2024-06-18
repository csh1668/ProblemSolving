from collections import deque


def bfs(start, neighbors_getter, parent_setter, visited_getter):
    q = deque()
    q.append(start)
    parent_setter(start, start)
    while len(q) > 0:
        cur = q.popleft()
        for neighbor in neighbors_getter(cur):
            if visited_getter(neighbor) is None:
                q.append(neighbor)
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
    bfs((0, 0), get_adj, set_parent, get_parent)
    print(parents)