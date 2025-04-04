from collections import deque

def cstyle_div(a, b):
    return abs(a) // abs(b) * (-1 if a * b < 0 else 1)

assert cstyle_div(-3, 2) == -1
assert cstyle_div(3, -2) == -1
assert cstyle_div(-3, -2) == 1

def calc(a, op, b):
    if op == '*': return a * b
    if op == '/': return cstyle_div(a, b)
    if op == '+': return a + b
    if op == '-': return a - b

buf = ''
dq = deque()
for ch in input():
    if ch == '*' or ch == '/' or ch == '+' or ch == '-':
        if buf: dq.append(int(buf)); buf = ''
        dq.append(ch)
    else:
        buf += ch
dq.append(int(buf))

if dq[0] == '-':
    dq.popleft()
    dq[0] = -dq[0]

while len(dq) != 1:
    a1, op1, b1 = dq[0], dq[1], dq[2]
    a2, op2, b2 = dq[-3], dq[-2], dq[-1]
    res1, res2 = calc(a1, op1, b1), calc(a2, op2, b2)
    if (op1 == '*' or op1 == '/') and (op2 == '+' or op2 == '-'):
        dq.popleft(); dq.popleft(); dq.popleft()
        dq.appendleft(res1)
    elif (op1 == '+' or op1 == '-') and (op2 == '*' or op2 == '/'):
        dq.pop(); dq.pop(); dq.pop()
        dq.append(res2)
    else:
        if res1 >= res2:
            dq.popleft(); dq.popleft(); dq.popleft()
            dq.appendleft(res1)
        else:
            dq.pop(); dq.pop(); dq.pop()
            dq.append(res2)

print(dq[0])