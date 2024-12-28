#include<iostream>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<windows.h>

// Game board size
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// Tetromino (Tetris piece) shapes
class Tetromino {
public:
    // Different Tetris piece shapes
    static const int SHAPES[7][4][4];

    // Piece coordinates and shape
    int shape[4][4] = { {0} };
    int x = 0, y = 0;

    // Create a random Tetris piece
    void spawnRandomPiece() {
        // Choose a random piece shape
        int shapeIndex = rand() % 7;

        // Copy the selected shape
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                shape[row][col] = SHAPES[shapeIndex][row][col];
            }
        }

        // Start piece at the top center of the board
        x = BOARD_WIDTH / 2 - 2;
        y = 0;
    }

    // Rotate the piece 90 degrees
    void rotate() {
        int rotatedShape[4][4] = { {0} };

        // Perform rotation
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                rotatedShape[col][3 - row] = shape[row][col];
            }
        }

        // Try to place rotated piece
        memcpy(shape, rotatedShape, sizeof(rotatedShape));
    }
};

// Initialize Tetromino shapes
const int Tetromino::SHAPES[7][4][4] = {
    {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},  // I-piece
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},  // Square piece
    {{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}},  // T-piece
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},  // S-piece
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},  // Z-piece
    {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}},  // L-piece
    {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}}   // Reverse L-piece
};

class TetrisGame {
private:
    int board[BOARD_HEIGHT][BOARD_WIDTH] = { {0} };
    Tetromino currentPiece;
    int score = 0;
    int level = 1;

public:
    // Check if a piece can be placed at the given position
    bool isValidMove(const Tetromino& piece, int newX, int newY) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (piece.shape[row][col]) {
                    int boardX = newX + col;
                    int boardY = newY + row;

                    // Check board boundaries and existing blocks
                    if (boardX < 0 || boardX >= BOARD_WIDTH ||
                        boardY >= BOARD_HEIGHT ||
                        (boardY >= 0 && board[boardY][boardX])) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // Place current piece on the board
    void placePiece() {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (currentPiece.shape[row][col]) {
                    board[currentPiece.y + row][currentPiece.x + col] = 1;
                }
            }
        }
    }

    // Remove completed lines and update score
    void clearLines() {
        int linesCleared = 0;
        for (int row = BOARD_HEIGHT - 1; row >= 0; row--) {
            bool fullLine = true;
            for (int col = 0; col < BOARD_WIDTH; col++) {
                if (!board[row][col]) {
                    fullLine = false;
                    break;
                }
            }

            if (fullLine) {
                // Shift lines down
                for (int r = row; r > 0; r--) {
                    for (int col = 0; col < BOARD_WIDTH; col++) {
                        board[r][col] = board[r - 1][col];
                    }
                }
                // Clear top line
                for (int col = 0; col < BOARD_WIDTH; col++) {
                    board[0][col] = 0;
                }
                linesCleared++;
                row++; // Recheck the same row
            }
        }

        // Update score based on lines cleared
        switch (linesCleared) {
        case 1: score += 100; break;
        case 2: score += 300; break;
        case 3: score += 500; break;
        case 4: score += 800; break;
        }

        // Level up
        if (score >= level * 1000) {
            level++;
        }
    }

    // Draw the game board
    void drawBoard() {
        system("cls");

        // Top border
        for (int col = 0; col < BOARD_WIDTH + 2; col++) std::cout << "#";
        std::cout << std::endl;

        // Game board
        for (int row = 0; row < BOARD_HEIGHT; row++) {
            std::cout << "#";
            for (int col = 0; col < BOARD_WIDTH; col++) {
                bool isPieceHere = false;
                for (int prow = 0; prow < 4; prow++) {
                    for (int pcol = 0; pcol < 4; pcol++) {
                        if (currentPiece.shape[prow][pcol] &&
                            col == currentPiece.x + pcol &&
                            row == currentPiece.y + prow) {
                            isPieceHere = true;
                            break;
                        }
                    }
                    if (isPieceHere) break;
                }

                if (isPieceHere) {
                    std::cout << "O";
                }
                else if (board[row][col]) {
                    std::cout << "#";
                }
                else {
                    std::cout << " ";
                }
            }
            std::cout << "#" << std::endl;
        }

        // Bottom border
        for (int col = 0; col < BOARD_WIDTH + 2; col++) std::cout << "#";
        std::cout << std::endl;

        // Score and level display
        std::cout << "Score: " << score << " | Level: " << level << std::endl;
    }

    // Move piece down
    bool moveDown() {
        if (isValidMove(currentPiece, currentPiece.x, currentPiece.y + 1)) {
            currentPiece.y++;
            return true;
        }

        placePiece();
        clearLines();
        currentPiece.spawnRandomPiece();

        // Check game over condition
        if (!isValidMove(currentPiece, currentPiece.x, currentPiece.y)) {
            std::cout << "Game Over!\n";
            std::cout << "Final Score: " << score << "\n";
            std::cout << "Final Level: " << level << "\n";
            exit(0);
        }

        return false;
    }

    // Start the game
    void play() {
        srand(time(NULL));
        currentPiece.spawnRandomPiece();

        while (true) {
            // Handle keyboard input
            if (_kbhit()) {
                switch (_getch()) {
                case 'a':
                    if (isValidMove(currentPiece, currentPiece.x - 1, currentPiece.y))
                        currentPiece.x--;
                    break;
                case 'd':
                    if (isValidMove(currentPiece, currentPiece.x + 1, currentPiece.y))
                        currentPiece.x++;
                    break;
                case 's':
                    moveDown();
                    break;
                case 'w':
                    // Attempt to rotate piece
                    Tetromino tempPiece = currentPiece;
                    tempPiece.rotate();
                    if (isValidMove(tempPiece, currentPiece.x, currentPiece.y)) {
                        currentPiece = tempPiece;
                    }
                    break;
                }
            }

            // Automatic piece fall
            static clock_t lastFall = clock();
            if ((clock() - lastFall) / CLOCKS_PER_SEC * 1000 > (500 - level * 50)) {
                moveDown();
                lastFall = clock();
            }

            // Draw board and add slight delay
            drawBoard();
            Sleep(50);
        }
    }
};

int main() {
    TetrisGame game;
    game.play();
    return 0;
}
// Project Done 
