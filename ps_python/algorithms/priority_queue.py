from collections import deque

# Work in Progress
class Node:
    def __init__(self, data, priority):
        self.data = data
        self.priority = priority

class PriorityQueue:
    def __init__(self):
        self.q = deque()

    def push(self, item):
        q = self.inner
        q.append(item)
        idx = len(q) - 1
        while idx > 0:
            parent_idx = idx // 2
            if q[idx] > q[parent_idx]:
                q[idx], q[parent_idx] = q[parent_idx], q[idx]
                idx = parent_idx
            else:
                break


    def pop(self):
        res = self.inner.popleft()
        return res