from collections import defaultdict

n = int(input())
words = [input() for _ in range(n)]
alphabet_dic = defaultdict(int)

for word in words:
    for i, ch in enumerate(reversed(word)):
        alphabet_dic[ch] += pow(10, i)
items = list(alphabet_dic.items())
items.sort(key=lambda x: -x[-1])
nums = list(range(10))
res = 0
for ch, score in items:
    res += score * nums.pop()
print(res)