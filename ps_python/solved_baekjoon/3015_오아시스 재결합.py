import sys
idx, tokens = 0, sys.stdin.read().split()
def ri():global idx;return int(tokens[(idx := idx + 1) - 1])

N = ri()
st = []
cnt = dict()
ans = 0
for _ in range(N):
    a = ri()
    while st and st[-1] < a:
        p = st.pop()
        ans += cnt[p]
        del cnt[p]
    if st:
        # ans += 1
        if st[-1] == a:
            ans += cnt[st[-1]]
            if len(st) > 1:
                ans += 1
        else:
            ans += 1
    if st and st[-1] == a:
        cnt[st[-1]] += 1
    else:
        cnt[a] = 1
        st.append(a)

print(ans)