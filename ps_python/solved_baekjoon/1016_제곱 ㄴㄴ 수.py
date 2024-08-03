import sys

a, b = map(int, input().split())

cnt = b - a + 1
sqrt = 2

sieve = [True] * (cnt + 2)
while sqrt * sqrt <= b:
	cur = sqrt * sqrt - a
	if cur >= 0 and not sieve[cur]:
		sqrt += 1
		continue
	elif cur >= 0:
		cnt -= 1
		sieve[cur] = False
	j = a // (sqrt * sqrt)
	while j * sqrt * sqrt <= b:
		k = j * sqrt * sqrt - a
		if k >= 0 and sieve[k]:
			sieve[k] = False
			cnt -= 1
		j += 1
	sqrt += 1

print(cnt)