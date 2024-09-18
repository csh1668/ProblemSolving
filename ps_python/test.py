lines = open('dict.txt', 'r').readlines()


class Node:
    def __init__(self, key):
        self.key = key
        self.children = {}
        self.is_end = False


root = Node('')
for word in lines:
    word = word.strip()
    node = root
    for ch in word:
        if ch not in node.children:
            node.children[ch] = Node(ch)
        node = node.children[ch]
    node.is_end = True

output = open('output.txt', 'w')


def search(node):
    output.write(f'{node.key}')
    if node.index:
        output.write('*')
    for child in node.children.values():
        search(child)
    if len(node.children) > 0:
        output.write('^')
    # output.write('\n')


search(root)
output.close()