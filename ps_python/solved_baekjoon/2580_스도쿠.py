sudoku = [list(map(int, input().split())) for _ in range(9)]

row = [[False] * 10 for _ in range(9)]
col = [[False] * 10 for _ in range(9)]
box = [[[False] * 10 for _ in range(3)] for _ in range(3)]

blanks = [(i, j) for i in range(9) for j in range(9) if sudoku[i][j] == 0]

for i in range(9):
    for j in range(9):
        if sudoku[i][j] == 0: continue
        row[i][sudoku[i][j]] = True
        col[j][sudoku[i][j]] = True
        box[i // 3][j // 3][sudoku[i][j]] = True

def backtracking(idx):
    if idx == len(blanks):
        [print(*line) for line in sudoku]
        exit()
    i, j = blanks[idx]
    for num in range(1, 10):
        if row[i][num] or col[j][num] or box[i // 3][j // 3][num]: continue
        row[i][num] = True; col[j][num] = True
        box[i // 3][j // 3][num] = True
        sudoku[i][j] = num
        backtracking(idx + 1)
        sudoku[i][j] = 0
        row[i][num] = False; col[j][num] = False
        box[i // 3][j // 3][num] = False

backtracking(0)