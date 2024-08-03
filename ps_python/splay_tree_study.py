import sys


class Node:
    def __init__(self, l=None, r=None, p=None):
        self.l: Node = l
        self.r: Node = r
        self.p: Node = p
        self.key = 0
        self.cnt = 0
        self.sum = 0

    def __repr__(self):
        return f'{self.key}|{self.cnt}'


def init(n: int):
    x: Node; i: int
    global root
    x = root = Node()
    x.cnt = n
    x.sum = x.key = 0
    for i in range(1, n):
        x.r = Node(p=x)
        x = x.r
        x.l = x.r = None
        x.cnt = n - i
        x.sum = x.key = arr[i - 1]


def update(x: Node):
    x.cnt = 1
    # 구간 합 구하기
    x.sum = x.key
    if x.l is not None:
        x.cnt += x.l.cnt
        x.sum += x.l.sum
    if x.r is not None:
        x.cnt += x.r.cnt
        x.sum += x.r.sum


def rotate(x: Node):
    global root
    p = x.p
    b: Node = None
    if p is None: return
    if x == p.l:    # x가 왼쪽 자식일 경우
        p.l = b = x.r
        x.r = p
    else:           # 오른쪽 자식일 경우
        p.r = b = x.l
        x.l = p
    x.p, p.p = p.p, x
    if b is not None: b.p = p
    if x.p is not None:
        if p == x.p.l: x.p.l = x
        else: x.p.r = x
    else: root = x
    update(p)
    update(x)


def splay(x: Node):
    while x.p is not None:
        p, g = x.p, x.p.p
        if g is not None:
            rotate(p if (x == p.l) == (p == g.l) else x)
        rotate(x)


def insert(k):
    global root
    p: Node = root
    if p is None:
        root = x = Node()
        x.key = k
        return
    while True:
        if k == p.key: return
        if k < p.key:
            if p.l is None:
                x = Node(None, None, p, k)
                p.l = x
                splay(x)
                return
            p = p.l
        else:
            if p.r is None:
                x = Node(None, None, p, k)
                p.r = x
                splay(x)
                return
            p = p.r


def find(k):
    global root
    p: Node = root
    if p is None: return False
    while p is not None:
        if k == p.key: break
        if k < p.key:
            if p.l is None: break
            p = p.l
        else:
            if p.r is None: break
            p = p.r
    splay(p)
    return k == p.key


def find_kth(k):
    x: Node = root
    while True:
        while x.l is not None and x.l.cnt > k:
            x = x.l
        if x.l:
            k -= x.l.cnt
        if k == 0:
            break
        k -= 1
        x = x.r
    splay(x)


def delete(k):
    global root
    if not find(k): return
    p: Node = root
    if p.l is not None:
        if p.r is not None:
            root = p.l
            root.p = None
            x = root
            while x.r is not None:
                x = x.r
            x.r = p.r
            p.r.p = x
            splay(x)
            return
        root = p.l
        root.p = None
        return
    if p.r is not None:
        root = p.r
        root.p = None
        return
    root = None


def add(i, z):
    find_kth(i)
    global root
    root.sum += z
    root.key += z


def interval(l, r):
    find_kth(l - 1)
    global root
    x = root
    root = x.r
    root.p = None
    find_kth(r - 1 + 1)
    x.r = root
    root.p = x
    root = x


def get_sum(l, r):
    interval(l, r)
    return root.r.l.sum


def print_tree(n, level=0, prefix='Root: '):
    if n is None: return
    print(' ' * (level * 4) + prefix, n)
    print_tree(n.l, level + 1, prefix='L--')
    print_tree(n.r, level + 1, prefix='R--')


n, m, k = map(int, sys.stdin.readline().split())
arr = [int(sys.stdin.readline()) for _ in range(n)]
root = None
init(10)
print_tree(root)
print_tree(root)
print(get_sum(2, 5))
print(get_sum(2, 4))
# while True:
#     mode, inp = map(int, input().split())
#     if mode == 0:
#         break
#     if mode == 1:
#         insert(inp)
#         print_tree(root)
#     if mode == 2:
#         delete(inp)
#         print_tree(root)
#     if mode == 3:
#         find_kth(inp)
#         print_tree(root)



# REFERENCE: https://cubelover.tistory.com/10