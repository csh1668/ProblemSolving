directions = [[0, -1], [0, 1], [-1, 0], [1, 0]]

n, m = map(int, input().split())
board = list()
red_pos: list
blue_pos: list

result_turn = -1
clear = False
queue = list()

for i in range(n):
    chars = list(input())
    inner_list = list()
    for j, c in enumerate(chars):
        if c == 'R':
            red_pos = [i, j]
            inner_list.append('.')
        elif c == 'B':
            blue_pos = [i, j]
            inner_list.append('.')
        else:
            inner_list.append(c)
    board.append(inner_list)

queue.append([red_pos, blue_pos, 0])


def print_board(board):
    for row in board:
        print(row)


def tilt(red, blue, direction):
    red_i, red_j = red
    blue_i, blue_j = blue
    red_clear = blue_clear = moved_once = False
    while True:
        moved = False
        if not red_clear:
            next_red_i, next_red_j = red_i + direction[0], red_j + direction[1]
            if (0 <= next_red_i < n and 0 <= next_red_j < m) and not (blue_i == next_red_i and blue_j == next_red_j):
                next_cell = board[next_red_i][next_red_j]
                if next_cell == '.':
                    moved = moved_once = True
                    red_i, red_j = next_red_i, next_red_j
                elif next_cell == 'O':
                    moved = red_clear = moved_once = True
                    red_i, red_j = -1, -1

        if not blue_clear:
            next_blue_i, next_blue_j = blue_i + direction[0], blue_j + direction[1]
            if (0 <= next_blue_i < n and 0 <= next_blue_j < m) and not (red_i == next_blue_i and red_j == next_blue_j):
                next_cell = board[next_blue_i][next_blue_j]
                if next_cell == '.':
                    moved = moved_once = True
                    blue_i, blue_j = next_blue_i, next_blue_j
                elif next_cell == 'O':
                    moved = blue_clear = moved_once = True
                    blue_i, blue_j = -1, -1

        if not moved:
            break

    result = ['', [red_i, red_j], [blue_i, blue_j]]
    if blue_clear:
        result[0] = 'game over'
    elif red_clear:
        result[0] = 'clear'
    elif moved_once:
        result[0] = 'moved'
    else:
        result[0] = 'not moved'

    return result


while len(queue) > 0 and not clear:
    red, blue, turn = queue.pop(0)
    if turn >= 10:
        break
    for direction in directions:
        if clear:
            break
        result = tilt(red, blue, direction)
        if result[0] == 'moved':
            queue.append([result[1], result[2], turn + 1])
        elif result[0] == 'clear':
            clear = True
            result_turn = turn + 1

print(result_turn)
