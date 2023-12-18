import numpy as np

global cnt, p, e, ttt


def Input():
    global cnt, ttt
    while True:
        print("\nPlayer:")
        row, col = map(int, input().split())
        if 0 < row < 4 and 0 < col < 4 and ttt[row - 1][col - 1] == 0:
            ttt[row - 1][col - 1] = -1
            break
        else:
            print("Input Error!\n")
    cnt += 1


def Print():
    global ttt
    for i in range(3):
        for j in range(3):
            if ttt[i][j] == 1:
                print("O", end=" ")
            if ttt[i][j] == 0:
                print("-", end=" ")
            if ttt[i][j] == -1:
                print("X", end=" ")
        print()


def Win():
    global ttt
    if (ttt[0][0] == ttt[1][1] == ttt[2][2] == 1) or (ttt[2][0] == ttt[1][1] == ttt[0][2] == 1):
        return 1
    if (ttt[0][0] == ttt[1][1] == ttt[2][2] == -1) or (ttt[2][0] == ttt[1][1] == ttt[0][2] == -1):
        return -1
    for i in range(3):
        if (ttt[i][0] == ttt[i][1] == ttt[i][2] == 1) or (ttt[0][i] == ttt[1][i] == ttt[2][i] == 1):
            return 1
        if (ttt[i][0] == ttt[i][1] == ttt[i][2] == -1) or (ttt[0][i] == ttt[1][i] == ttt[2][i] == -1):
            return -1
    return 0


def Back_up():
    global ttt, p, e
    tmp = np.zeros((3, 3))
    p = e = 0
    for i in range(3):
        for j in range(3):
            if ttt[i][j] == 0:
                tmp[i][j] = 1
            else:
                tmp[i][j] = ttt[i][j]
    for i in range(3):
        e += (tmp[i][0] + tmp[i][1] + tmp[i][2]) / 3
        e += (tmp[0][i] + tmp[1][i] + tmp[2][i]) / 3
    e += (tmp[0][0] + tmp[1][1] + tmp[2][2]) / 3
    e += (tmp[2][0] + tmp[1][1] + tmp[0][2]) / 3
    for i in range(3):
        for j in range(3):
            if ttt[i][j] == 0:
                tmp[i][j] = -1
            else:
                tmp[i][j] = ttt[i][j]
    for i in range(3):
        p += (tmp[i][0] + tmp[i][1] + tmp[i][2]) / 3
        p += (tmp[0][i] + tmp[1][i] + tmp[2][i]) / 3
    p += (tmp[0][0] + tmp[1][1] + tmp[2][2]) / 3
    p += (tmp[2][0] + tmp[1][1] + tmp[0][2]) / 3
    return e + p


def AlphaBeta(value, d, first):
    global ttt
    prune = False
    if d == 6 or d + cnt == 9:  # 搜索结束
        return value, Back_up()  # 返回电脑的胜率
    if Win() == 1:
        return 10, 0
    if first:
        ab = 10
    else:
        ab = -10
    for i in range(3):
        if prune:
            break
        for j in range(3):
            if prune:
                break
            if ttt[i][j] == 0:
                if first:
                    ttt[i][j] = -1
                    if Win() == -1:
                        tmp = -10
                    else:
                        ab, tmp = AlphaBeta(ab, d + 1, not first)  # 搜索最小倒推值
                    if tmp < ab:
                        ab = tmp  # 更新倒推值
                    if ab < value:
                        prune = True  # β<α,α剪枝
                else:
                    ttt[i][j] = 1
                    if Win() == 1:
                        tmp = 10
                    else:
                        ab, tmp = AlphaBeta(ab, d + 1, not first)  # 搜索最大倒推值
                    if tmp > ab:
                        ab = tmp
                    if ab > value:
                        prune = True  # α<β,β剪枝
                ttt[i][j] = 0  # 复原棋盘
    if first:
        if ab > value:
            value = ab
    elif ab < value:
        value = ab
    return value, ab


def PvE():
    global cnt, ttt, p, e
    ttt = np.zeros((3, 3))
    cnt = 0
    row = col = 0
    value = -10
    tmp = -10
    d = 1
    print("Board:")
    Print()
    while True:
        print("\nWho take the first step:\n1.Player.  2.Computer.[ ]\b\b")
        op = int(input())
        if op != 1 and op != 2:
            print("Input Error!")
            input()
        else:
            break
    if op == 1:
        while True:
            Input()
            Print()
            if Win() == -1:
                print("\nYou win!!")
                return
            for i in range(3):
                for j in range(3):
                    if ttt[i][j] == 0:
                        ttt[i][j] = 1
                        tmp, x = AlphaBeta(tmp, d, 1)
                        if Win() == 1:
                            print("\nComputer:", i + 1, j + 1)
                            Print()
                            print("\nComputer win!")
                            return
                        if tmp > value:
                            value = tmp
                            row = i
                            col = j
                        ttt[i][j] = 0
                        tmp = -10
            if ttt[row][col] != 0:
                print("\nYou win")
                return
            ttt[row][col] = 1
            tmp = value = -10
            d = 1
            print("\nComputer:", row + 1, col + 1)
            Print()
            cnt += 1
            Back_up()
            if round(p, 2) == 0:
                print("\nTie!")
                return
    else:
        while True:
            for i in range(3):
                for j in range(3):
                    if ttt[i][j] == 0:
                        ttt[i][j] = 1
                        tmp, x = AlphaBeta(tmp, d, 1)
                        if Win() == 1:
                            print("\nComputer:", i + 1, j + 1)
                            Print()
                            print("\nComputer win!")
                            return
                        if tmp > value:
                            value = tmp
                            row = i
                            col = j
                        ttt[i][j] = 0
                        tmp = -10
            ttt[row][col] = 1
            print("\nComputer:", row + 1, col + 1)
            Print()
            cnt += 1
            Input()
            Print()
            Back_up()
            if round(e, 2) == 0:
                print("\nTie!")
                return
            if Win() == -1:
                print("\nYou win!")
                return
            tmp = value = -10
            d = 1


if __name__ == '__main__':
    while True:
        PvE()
        print("\n1.Try Again\t0.Exit [ ]\b\b")
        op = int(input())
        if op == 0:
            break
        if op != 1:
            input()
