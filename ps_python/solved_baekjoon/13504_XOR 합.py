import sys


class Node:
    def __init__(self, key):
        self.key = key
        self.children = [None for _ in range(2)]

def insert(root, key):
    cur = root
    for i in range(31, -1, -1):
        bit = (key >> i) & 1
        if cur.children[bit] is None:
            cur.children[bit] = Node(bit)
        cur = cur.children[bit]

def find_max_xor(root, key):
    cur = root
    xor = 0
    for i in range(31, -1, -1):
        bit = (key >> i) & 1
        if cur.children[bit ^ 1] is not None:
            xor |= 1 << i
            cur = cur.children[bit ^ 1]
        else:
            cur = cur.children[bit]
    return xor

for _ in range(int(sys.stdin.readline())):
    n = int(sys.stdin.readline())
    root = Node('')
    insert(root, 0)

    prefix = ans = 0
    for x in map(int, sys.stdin.readline().split()):
        prefix ^= x
        ans = max(ans, find_max_xor(root, prefix))
        insert(root, prefix)
    print(ans)