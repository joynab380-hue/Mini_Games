#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int MINES = 10;

struct Cell {
    bool mine = false;
    bool revealed = false;
    bool flagged = false;
    int adjacent = 0;
};

vector<vector<Cell>> grid(ROWS, vector<Cell>(COLS));
vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

string convertValue(int v) {
    if (v == 0) return "$";
    if (v == 1) return "A";
    if (v == 2) return "B";
    if (v == 3) return "C";
    return to_string(v);
}

void initGrid() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            grid[i][j] = Cell();

    srand(time(0));
    int placed = 0;

    while (placed < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (!grid[r][c].mine) {
            grid[r][c].mine = true;
            placed++;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j].mine) continue;
            int count = 0;

            for (int dr = -1; dr <= 1; dr++)
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = i + dr, nc = j + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && grid[nr][nc].mine)
                        count++;
                }

            grid[i][j].adjacent = count;
        }
    }
}

void printGrid(bool revealAll = false) {
    system("cls");
    cout << "=== Minesweeper ===\n\n   ";

    for (int j = 0; j < COLS; j++) cout << j << " ";
    cout << "\n";

    for (int i = 0; i < ROWS; i++) {
        cout << i << "  ";
        for (int j = 0; j < COLS; j++) {

            if (revealAll) {
                if (grid[i][j].mine) cout << "* ";
                else cout << convertValue(grid[i][j].adjacent) << " ";
            }
            else if (grid[i][j].flagged) cout << "F ";
            else if (!grid[i][j].revealed) cout << "# ";
            else if (grid[i][j].mine) cout << "* ";
            else cout << convertValue(grid[i][j].adjacent) << " ";
        }
        cout << "\n";
    }
}

bool revealCell(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return true;
    if (visited[r][c] || grid[r][c].flagged) return true;

    visited[r][c] = true;
    grid[r][c].revealed = true;

    if (grid[r][c].mine) {
        Beep(500, 300);
        return false;
    }

    if (grid[r][c].adjacent == 0) {
        for (int dr = -1; dr <= 1; dr++)
            for (int dc = -1; dc <= 1; dc++)
                if (dr != 0 || dc != 0)
                    revealCell(r + dr, c + dc);
    }

    return true;
}

bool checkWin() {
    int revealedCount = 0;

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (grid[i][j].revealed) revealedCount++;

    return revealedCount == ROWS * COLS - MINES;
}

void playMinesweeper() {
    initGrid();
    visited.assign(ROWS, vector<bool>(COLS, false));

    int r, c;
    char action;

    while (true) {
        printGrid();
        cout << "\nEnter row col and action (r=Reveal, f=Flag): ";
        cin >> r >> c >> action;

        if (action == 'r') {
            if (!revealCell(r, c)) {
                printGrid(true);
                cout << "\nBOOM! You hit a mine! Game Over.\n";
                Beep(300, 500);
                break;
            }
        }
        else if (action == 'f') {
            grid[r][c].flagged = !grid[r][c].flagged;
            Beep(1000, 100);
        }

        if (checkWin()) {
            printGrid(true);
            cout << "\nCongratulations! You cleared all mines!\n";
            Beep(1500, 500);
            break;
        }
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
}
