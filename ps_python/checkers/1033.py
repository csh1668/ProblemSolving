from dmoj.checkers import CheckerOutput
from dmoj.graders.standard import StandardGrader
from dmoj.result import CheckerResult, Result
from dmoj.problem import TestCase

MIN_N, MAX_N = 1, 100
MIN_M, MAX_M = 1, 100
MIN_E, MAX_E = 1, 10000

class Grader(StandardGrader):
    def check_result(self, case: TestCase, result: Result) -> CheckerOutput:
        try:
            input_tokens = result.output.split(); token_idx = 0
            def read_int():
                nonlocal token_idx
                token_idx += 1
                return int(input_tokens[token_idx - 1])

            N, M = read_int(), read_int()
            if not (MIN_N <= N <= MAX_N):
                return CheckerResult(False, 0, f'N의 범위는 {MIN_N} 이상 {MAX_N} 이하여야 합니다.')
            if not (MIN_M <= M <= MAX_M):
                return CheckerResult(False, 0, f'M의 범위는 {MIN_M} 이상 {MAX_M} 이하여야 합니다.')

            G = [[] for _ in range(N + 1)]
            for _ in range(M):
                u, v, w = read_int(), read_int(), read_int()
                if not (1 <= u <= N and 1 <= v <= N):
                    return CheckerResult(False, 0, f'간선은 1 이상 {N} 이하의 정점을 연결해야 합니다.')
                if not (MIN_E <= w <= MAX_E):
                    return CheckerResult(False, 0, f'간선의 가중치는 {MIN_E} 이상 {MAX_E} 이하여야 합니다.')
                G[u].append((v, w))
                G[v].append((u, w))

            def dijkstra(start):
                from heapq import heappush, heappop
                cnt = 0
                dists = [1_000_000_000] * (N + 1)
                pq = [(0, start)]
                while pq:
                    d, u = heappop(pq)
                    if dists[u] < d: continue
                    for v, w in G[u]:
                        if dists[v] > d + w:
                            dists[v] = d + w
                            heappush(pq, (dists[v], v))
                            if u == N: cnt += 1
                return cnt


        except Exception as e:
            return CheckerResult(False, 0, f'채점 중 오류가 발생했습니다.', f'{e}')