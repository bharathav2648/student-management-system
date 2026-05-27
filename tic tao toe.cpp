#include <iostream>
#include <limits>

using namespace std;

// --- Function Prototypes ---
void displayBoard(const char board[3][3]);
bool checkWin(const char board[3][3], char player);
bool checkDraw(const char board[3][3]);
void resetBoard(char board[3][3]);
void playGame();

int main() {
    char playAgain;
    
    // Outer Loop: Handles the "Replay" functionality
    do {
        playGame();
        
        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    cout << "Thanks for playing! Goodbye.\n";
    return 0;
}

// --- Main Game Logic ---
void playGame() {
    // Array: 2D array to represent the 3x3 game board
    char board[3][3];
    resetBoard(board);
    
    char currentPlayer = 'X';
    bool gameWon = false;
    bool gameDraw = false;
    
    cout << "\n=== Welcome to Tic-Tac-Toe ===" << endl;
    cout << "Player 1 is 'X' | Player 2 is 'O'" << endl;
    
    // Inner Loop: Handles the turn-by-turn gameplay
    while (!gameWon && !gameDraw) {
        displayBoard(board);
        
        int choice;
        int row, col;
        bool validMove = false;
        
        // Loop: Input validation
        while (!validMove) {
            cout << "Player " << currentPlayer << ", enter a number (1-9) to place your mark: ";
            cin >> choice;
            
            // Conditional Logic: Ensure input is a number between 1 and 9
            if (cin.fail() || choice < 1 || choice > 9) {
                cin.clear(); // clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
                cout << "Invalid input. Please enter a number between 1 and 9.\n";
                continue;
            }
            
            // Math to convert 1-9 into 2D array coordinates (0-2 for row/col)
            row = (choice - 1) / 3;
            col = (choice - 1) % 3;
            
            // Conditional Logic: Check if the spot is already taken
            if (board[row][col] != 'X' && board[row][col] != 'O') {
                validMove = true;
            } else {
                cout << "That spot is already taken! Try again.\n";
            }
        }
        
        // Update the board array
        board[row][col] = currentPlayer;
        
        // Check for Win or Draw
        gameWon = checkWin(board, currentPlayer);
        if (!gameWon) {
            gameDraw = checkDraw(board);
        }
        
        // Switch players if the game is still going
        if (!gameWon && !gameDraw) {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
    
    // Final dynamic board display
    displayBoard(board);
    
    // Conditional Logic: Announce outcome
    if (gameWon) {
        cout << ">>> Congratulations! Player " << currentPlayer << " wins! <<<\n";
    } else if (gameDraw) {
        cout << ">>> It's a draw! No one wins. <<<\n";
    }
}

// --- Helper Functions ---

void displayBoard(const char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << " " << board[i][j] << " ";
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "---|---|---\n";
    }
    cout << "\n";
}

void resetBoard(char board[3][3]) {
    char cellNumber = '1';
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = cellNumber++;
        }
    }
}

bool checkWin(const char board[3][3], char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) || 
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

bool checkDraw(const char board[3][3]) {
    // If any cell still contains a number (1-9), the board is not full
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                return false; 
            }
        }
    }
    return true; // All cells are 'X' or 'O', and no one won
}