def print_board(board):
    print("\n")
    for i in range(3):
        print(f" {board[i][0]} | {board[i][1]} | {board[i][2]} ")
        if i < 2:
            print("-----------")
    print("\n")

def check_winner(board, player):
    # Check rows, columns, and diagonals
    for i in range(3):
        if all(board[i][j] == player for j in range(3)):
            return True
        if all(board[j][i] == player for j in range(3)):
            return True
    if all(board[i][i] == player for i in range(3)):
        return True
    if all(board[i][2-i] == player for i in range(3)):
        return True
    return False

def tic_tac_toe():
    board = [[" " for _ in range(3)] for _ in range(3)]
    players = ["X", "O"]
    current_player = 0
    
    print("Welcome to Tic Tac Toe!")
    print("Positions are numbered 1-9 (row by row)")
    
    while True:
        print_board(board)
        
        while True:
            try:
                pos = int(input(f"Player {players[current_player]}, enter position (1-9): "))
                if pos < 1 or pos > 9:
                    print("Invalid position. Choose 1-9.")
                    continue
                row, col = (pos - 1) // 3, (pos - 1) % 3
                if board[row][col] != " ":
                    print("Position already taken!")
                    continue
                board[row][col] = players[current_player]
                break
            except ValueError:
                print("Invalid input. Enter a number 1-9.")
        
        if check_winner(board, players[current_player]):
            print_board(board)
            print(f"Player {players[current_player]} wins!")
            break
        
        if all(board[i][j] != " " for i in range(3) for j in range(3)):
            print_board(board)
            print("It's a draw!")
            break
        
        current_player = 1 - current_player

if __name__ == "__main__":
    tic_tac_toe()