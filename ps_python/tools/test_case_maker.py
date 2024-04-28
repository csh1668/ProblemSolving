import random
import sys

MAX = 100
sys.stdout = open('testcase.txt', 'w')
print(MAX, )
[print(*[random.randint(0, 0) for _ in range(MAX)], sep=' ') for _ in range(MAX)]
