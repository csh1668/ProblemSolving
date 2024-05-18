import sys

n = int(input())
arr = list(map(int, input().split()))
m = int(input())
queries = [list(map(int, sys.stdin.readline().split())) for _ in range(m)]