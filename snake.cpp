#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

/* ================= CONFIG ================= */
const int W = 20, H = 20;

/* ================= GLOBALS ================= */
bool gameOver, aiMode = false, paused = false;
int score, highScore = 0, headX, headY;

enum Dir { LEFT, RIGHT, UP, DOWN };
Dir dir;

vector<pair<int, int>> snake;
vector<pair<int, int>> fruits;

/* ================= COLORS ================= */
const int WALL = 9, HEAD = 10, BODY = 2, FRUIT = 12,
          TEXT = 15, GAMEOVER = 12, HILIGHT = 14;

/* ================= UTILS ================= */
void setColor(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void beep() {
    Beep(800, 150);
}

void clearConsoleFast() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    COORD pos = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(h, ' ', info.dwSize.X * info.dwSize.Y, pos, &written);
    SetConsoleCursorPosition(h, pos);
}

/* ================= HIGH SCORE ================= */
void loadHS() {
    ifstream f("snake_highscore.txt");
    if (f) f >> highScore;
}

void saveHS() {
    if (score > highScore) {
        highScore = score;
        ofstream f("snake_highscore.txt");
        f << highScore;
    }
}

/* ================= GAME LOGIC ================= */
bool isBody(int x, int y) {
    for (size_t i = 1; i < snake.size(); i++)
        if (snake[i].first == x && snake[i].second == y)
            return true;
    return false;
}

void spawnFruit() {
    int x, y;
    do {
        x = rand() % W;
        y = rand() % H;
    } while (isBody(x, y) || (x == headX && y == headY));
    fruits.push_back({x, y});
}

void Setup() {
    gameOver = false;
    paused = false;
    dir = RIGHT;
    headX = W / 2;
    headY = H / 2;

    snake.clear();
    snake.push_back({headX, headY});
    snake.push_back({headX - 1, headY});
    snake.push_back({headX - 2, headY});

    fruits.clear();
    spawnFruit();
    score = 0;
}

/* ================= AI (GREEDY) ================= */
Dir greedy() {
    if (fruits.empty()) return dir;

    auto target = fruits[0];
    for (auto &f : fruits)
        if (abs(f.first - headX) + abs(f.second - headY) <
            abs(target.first - headX) + abs(target.second - headY))
            target = f;

    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    Dir dirs[4] = {LEFT, RIGHT, UP, DOWN};

    Dir best = dir;
    int bestDist = 1e9;

    for (int i = 0; i < 4; i++) {
        int nx = headX + dx[i];
        int ny = headY + dy[i];
        if (nx >= 0 && nx < W && ny >= 0 && ny < H && !isBody(nx, ny)) {
            int d = abs(nx - target.first) + abs(ny - target.second);
            if (d < bestDist) {
                bestDist = d;
                best = dirs[i];
            }
        }
    }
    return best;
}

/* ================= INPUT ================= */
void Input() {
    if (_kbhit()) {
        unsigned char k = _getch();

        if (k == 0 || k == 224) {
            unsigned char a = _getch();
            if (!aiMode && !paused) {
                if (a == 72) dir = UP;
                else if (a == 80) dir = DOWN;
                else if (a == 75) dir = LEFT;
                else if (a == 77) dir = RIGHT;
            }
            return;
        }

        if (k == 'p' || k == 'P') paused = !paused;
        if (k == 't' || k == 'T') aiMode = !aiMode;
        if (k == 'r' || k == 'R') Setup();
        if (k == 'x' || k == 'X') gameOver = true;
    }

    if (aiMode && !paused)
        dir = greedy();
}

/* ================= DRAW ================= */
void Draw() {
    clearConsoleFast();

    setColor(WALL);
    for (int i = 0; i < W + 2; i++) cout << "#";
    cout << "\n";

    for (int y = 0; y < H; y++) {
        setColor(WALL); cout << "#";
        for (int x = 0; x < W; x++) {
            if (x == headX && y == headY) {
                setColor(HEAD); cout << "O";
            }
            else if (isBody(x, y)) {
                setColor(BODY); cout << "o";
            }
            else {
                bool fruit = false;
                for (auto &f : fruits)
                    if (f.first == x && f.second == y) {
                        setColor(FRUIT); cout << "F"; fruit = true; break;
                    }
                if (!fruit) { setColor(TEXT); cout << " "; }
            }
        }
        setColor(WALL); cout << "#\n";
    }

    for (int i = 0; i < W + 2; i++) cout << "#";
    cout << "\n";

    setColor(TEXT);
    cout << "Score: " << score << " | High: " << highScore;
    if (aiMode) cout << " | [AI]";
    if (paused) cout << " | [PAUSED]";
    cout << "\nArrows=Move  T=AI  P=Pause  R=Restart  X=Exit\n";
}

/* ================= UPDATE ================= */
void Logic() {
    if (paused) return;

    int nx = headX, ny = headY;
    if (dir == LEFT) nx--;
    if (dir == RIGHT) nx++;
    if (dir == UP) ny--;
    if (dir == DOWN) ny++;

    if (nx < 0 || nx >= W || ny < 0 || ny >= H || isBody(nx, ny)) {
        gameOver = true;
        return;
    }

    bool ate = false;
    for (auto it = fruits.begin(); it != fruits.end(); ) {
        if (it->first == nx && it->second == ny) {
            score += 10;
            beep();
            it = fruits.erase(it);
            ate = true;
        } else ++it;
    }

    snake.insert(snake.begin(), {nx, ny});
    if (!ate) snake.pop_back();

    headX = nx;
    headY = ny;

    if (fruits.empty()) spawnFruit();
}

/* ================= ENTRY ================= */
void playSnake() {
    srand(time(0));
    loadHS();

    clearConsoleFast();
    setColor(10);
    cout << "Snake Game\n";
    setColor(TEXT);
    cout << "1. AI Mode\n2. Manual Mode\nChoose: ";
    aiMode = (_getch() == '1');

    Setup();
    int delay = aiMode ? 100 : 150;

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(delay);
    }

    saveHS();

    clearConsoleFast();
    setColor(GAMEOVER);
    cout << "\n\n!!! GAME OVER !!!\n\n";
    setColor(HILIGHT);
    cout << "Score: " << score << "\nHigh: " << highScore << "\n";
    setColor(TEXT);
    cout << "\nPress Enter to return to menu...";
    cin.ignore();
    cin.get();
}
