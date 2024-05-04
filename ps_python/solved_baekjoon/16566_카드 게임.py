import bisect

n, m, k = map(int, input().split())
cards = list(map(int, input().split()))
cheolsu = list(map(int, input().split()))
used = [False for _ in range(m)]

cards.sort()

for aite in cheolsu:
    idx = bisect.bisect_right(cards, aite)
    if used[idx]:
        for j in range(idx + 1, m):
            if not used[j]:
                print(cards[j])
                used[j] = True
                break
    else:
        print(cards[idx])
        used[idx] = True