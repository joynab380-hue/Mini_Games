#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

const int ROWS = 15;
const int COLS = 25;
const int ZOMBIE_COUNT = 6;
const int WALL_DENSITY_PERCENT = 22;

struct Pos { int r, c; };

enum CellType { EMPTY, WALL, PLAYER, ZOMBIE, EXITCELL };

vector<string> legend = {
    ". = empty",
    "# = wall",
    "P = you (player)",
    "Z = zombie",
    "E = exit"
};

vector<vector<CellType>> board;
Pos playerPos;
Pos exitPos;
vector<Pos> zombies;
bool gameover = false;
bool playerWon = false;

inline bool inside(int r, int c) { return r >= 0 && r < ROWS && c >= 0 && c < COLS; }

const int dr[4] = { -1, 1, 0, 0 };
const int dc[4] = { 0, 0, -1, 1 };

void initBoard() {
    board.assign(ROWS, vector<CellType>(COLS, EMPTY));
    srand((unsigned)time(NULL));

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (rand() % 100 < WALL_DENSITY_PERCENT) board[r][c] = WALL;
        }
    }

    for (int r = 0; r < ROWS; ++r) {
        bool found = false;
        for (int c = 0; c < COLS; ++c) {
            if (board[r][c] == EMPTY) {
                playerPos = {r, c};
                board[r][c] = PLAYER;
                found = true; break;
            }
        }
        if (found) break;
    }

    for (int r = ROWS - 1; r >= 0; --r) {
        bool found = false;
        for (int c = COLS - 1; c >= 0; --c) {
            if (board[r][c] == EMPTY) {
                exitPos = {r, c};
                board[r][c] = EXITCELL;
                found = true; break;
            }
        }
        if (found) break;
    }

    zombies.clear();
    int placed = 0;
    while (placed < ZOMBIE_COUNT) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (board[r][c] != EMPTY) continue;

        int dist = abs(r - playerPos.r) + abs(c - playerPos.c);
        if (dist < 6) continue;
        board[r][c] = ZOMBIE;
        zombies.push_back({r, c});
        ++placed;
    }

    int pr = playerPos.r, pc = playerPos.c;
    int er = exitPos.r, ec = exitPos.c;
    while (pr != er) {
        if (board[pr][pc] == WALL) board[pr][pc] = EMPTY;
        if (pr < er) ++pr; else --pr;
    }
    while (pc != ec) {
        if (board[pr][pc] == WALL) board[pr][pc] = EMPTY;
        if (pc < ec) ++pc; else --pc;
    }
    board[playerPos.r][playerPos.c] = PLAYER;
    board[exitPos.r][exitPos.c] = EXITCELL;
}

void printBoard() {
    system("cls");
    cout << "=== Zombie Escape Maze ===\n\n";
    for (int i = 0; i < (int)legend.size(); ++i) cout << legend[i] << (i+1==legend.size()? "\n\n":" | ");
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (r == playerPos.r && c == playerPos.c) cout << 'P';
            else if (r == exitPos.r && c == exitPos.c) cout << 'E';
            else {
                bool printed = false;

                for (auto &z : zombies) if (z.r == r && z.c == c) { cout << 'Z'; printed = true; break; }
                if (printed) continue;
                // others
                if (board[r][c] == WALL) cout << '#';
                else cout << '.';
            }
        }
        cout << '\n';
    }
    cout << "\nControls: W A S D to move. Press Q to quit. Reach E to escape.\n";
}

vector<vector<int>> computeDistanceFromPlayer() {
    vector<vector<int>> dist(ROWS, vector<int>(COLS, -1));
    queue<Pos> q;
    dist[playerPos.r][playerPos.c] = 0;
    q.push(playerPos);

    while (!q.empty()) {
        Pos p = q.front(); q.pop();
        for (int k = 0; k < 4; ++k) {
            int nr = p.r + dr[k], nc = p.c + dc[k];
            if (!inside(nr, nc))
               continue;
            if (board[nr][nc] == WALL)
                continue;

            if (dist[nr][nc] == -1) {
                dist[nr][nc] = dist[p.r][p.c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return dist;
}

void moveZombies() {

    vector<vector<int>> dist = computeDistanceFromPlayer();

    for (size_t i = 0; i < zombies.size(); ++i) {
        Pos z = zombies[i];
        int bestDist = dist[z.r][z.c];
        Pos bestMove = z;
        bool moved = false;

        for (int k = 0; k < 4; ++k) {
            int nr = z.r + dr[k], nc = z.c + dc[k];
            if (!inside(nr, nc)) continue;
            if (board[nr][nc] == WALL) continue;

            int d = dist[nr][nc];
            if (d >= 0 && (bestDist == -1 || d < bestDist)) {
                bestDist = d;
                bestMove = {nr, nc};
                moved = true;
            }
        }

        if (!moved) {
            vector<Pos> opts;
            for (int k = 0; k < 4; ++k) {
                int nr = z.r + dr[k], nc = z.c + dc[k];
                if (!inside(nr, nc)) continue;
                if (board[nr][nc] == WALL) continue;

                if (nr == exitPos.r && nc == exitPos.c) continue;
                opts.push_back({nr,nc});
            }
            if (!opts.empty()) {
                bestMove = opts[rand() % opts.size()];
            } else {
                bestMove = z;
            }
        }
        zombies[i] = bestMove;
    }

    for (auto &z : zombies) {
        if (z.r == playerPos.r && z.c == playerPos.c) {
            Beep(400, 300);
            gameover = true;
            playerWon = false;
            return;
        }
    }
    for (auto &z : zombies) {
        int d = abs(z.r - playerPos.r) + abs(z.c - playerPos.c);
        if (d <= 1 && !gameover) {
            Beep(800, 80);
            break;
        }
    }
}

void tryMovePlayer(int drr, int dcc) {
    int nr = playerPos.r + drr;
    int nc = playerPos.c + dcc;
    if (!inside(nr, nc)) return;
    if (board[nr][nc] == WALL) return;

    for (auto &z : zombies) if (z.r == nr && z.c == nc) {
        playerPos = {nr, nc};
        Beep(300, 400);
        gameover = true; playerWon = false; return;
    }

    if (!(playerPos.r == exitPos.r && playerPos.c == exitPos.c)) board[playerPos.r][playerPos.c] = EMPTY;
    playerPos.r = nr; playerPos.c = nc;

    if (playerPos.r == exitPos.r && playerPos.c == exitPos.c) {
        Beep(1200, 300);
        gameover = true;
        playerWon = true;
        return;
    }

    board[playerPos.r][playerPos.c] = PLAYER;
}

void playZombieEscape() {
    initBoard();
    gameover = false;
    playerWon = false;

    printBoard();
    Beep(1000, 80);

    while (!gameover) {
        if (_kbhit()) {
            char ch = _getch();
            ch = tolower(ch);
            if (ch == 'q') { gameover = true; playerWon = false; break; }
            if (ch == 'w') tryMovePlayer(-1, 0);
            else if (ch == 's') tryMovePlayer(1, 0);
            else if (ch == 'a') tryMovePlayer(0, -1);
            else if (ch == 'd') tryMovePlayer(0, 1);

            moveZombies();
            printBoard();

            Sleep(80);
        } else {
            Sleep(40);
        }
    }

    printBoard();
    if (playerWon) {
        cout << "\nYou reached the exit_ you escaped the zombie maze! Congratulations!\n";
        Beep(1500, 300);
    } else {
        cout << "\nYou were caught by the zombies or quit. Game Over.\n";
    }

    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
}
