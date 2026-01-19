#include <iostream>
using namespace std;

void playSnake();
void playTicTacToe();
void playMinesweeper();
void playZombieEscape();

void clearScreen() {    system("cls");    }

int main() {
    int choice;

    while (true) {
        clearScreen();
        cout << "===== Mini Game Hub =====\n";
        cout << "1. Snake\n";
        cout << "2. Tic Tac Toe\n";
        cout << "3. Minesweeper\n";
        cout << "4. Zombie Escape Maze\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                clearScreen();
                playSnake();
                break;
            case 2:
                clearScreen();
                playTicTacToe();
                break;
            case 3:
                clearScreen();
                playMinesweeper();
                break;
            case 4:
                clearScreen();
                playZombieEscape();
                break;
            case 5:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.ignore();
        cin.get();
    }
}
