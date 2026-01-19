#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

/* ================= GLOBAL DP ================= */
map<string,int> dp;

/* ================= HELPER ================= */
string boardToString(char b[3][3]) {
    string s="";
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            s+=b[i][j];
    return s;
}

/* ================= CHECK WINNER ================= */
char checkWinner(char b[3][3], int &winType, int &winIndex) {
    // winType: 1=row, 2=column, 3=diag1, 4=diag2
    for(int i=0;i<3;i++){
        if(b[i][0]==b[i][1] && b[i][1]==b[i][2]){
            winType=1; winIndex=i; return b[i][0];
        }
        if(b[0][i]==b[1][i] && b[1][i]==b[2][i]){
            winType=2; winIndex=i; return b[0][i];
        }
    }
    if(b[0][0]==b[1][1] && b[1][1]==b[2][2]){
        winType=3; winIndex=0; return b[0][0];
    }
    if(b[0][2]==b[1][1] && b[1][1]==b[2][0]){
        winType=4; winIndex=0; return b[0][2];
    }
    return 'N';
}

bool isDraw(char b[3][3]) {
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(b[i][j]!='X' && b[i][j]!='O')
                return false;
    return true;
}

/* ================= MINIMAX (D&C + BACKTRACKING + DP) ================= */
int minimax(char b[3][3], char turn) {
    string key = boardToString(b);
    if(dp.count(key)) return dp[key];

    int winType, winIndex;
    char win = checkWinner(b, winType, winIndex);
    if(win=='X') return dp[key]=1;
    if(win=='O') return dp[key]=-1;
    if(isDraw(b)) return dp[key]=0;

    int best = (turn=='X') ? -100 : 100;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(b[i][j]!='X' && b[i][j]!='O'){
                char temp=b[i][j];        // BACKTRACK start
                b[i][j]=turn;

                int score = minimax(b, turn=='X'?'O':'X');

                b[i][j]=temp;             // BACKTRACK restore

                if(turn=='X') best=max(best,score);
                else best=min(best,score);
            }
        }
    }
    return dp[key]=best;
}

/* ================= DISPLAY WITH HIGHLIGHT ================= */
void displayBoard(char b[3][3], int winType=-1, int winIndex=-1) {
    system("cls");
    cout << "=== TicTacToe ===\n\n";
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i=0;i<3;i++){
        cout << "+---+---+---+\n";
        for(int j=0;j<3;j++){
            cout << "| ";

            bool highlight = (winType==1 && i==winIndex) ||
                             (winType==2 && j==winIndex) ||
                             (winType==3 && i==j) ||
                             (winType==4 && i+j==2);

            if(highlight){
                SetConsoleTextAttribute(hConsole, 9); // Blue color
                cout << b[i][j];
                SetConsoleTextAttribute(hConsole, 7); // Reset to default
            } else {
                cout << b[i][j];
            }
            cout << " ";
        }
        cout << "|\n";
    }
    cout << "+---+---+---+\n\n";
}

/* ================= GAME ================= */
void playTicTacToe() {
    char board[3][3];
    char ch='1';
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            board[i][j]=ch++;

    char current='X';
    int choice;
    int winType=-1, winIndex=-1;

    while(true){
        displayBoard(board, winType, winIndex);
        cout<<"Player "<<current<<" turn\n";

        cout<<"Enter position (1-9): ";
        cin >> choice;
        Beep(500,80);

        int r=(choice-1)/3, c=(choice-1)%3;
        if(choice<1 || choice>9 || board[r][c]=='X' || board[r][c]=='O'){
            cout<<"Invalid move!\n";
            Sleep(700);
            continue;
        }
        board[r][c]=current;

        char win = checkWinner(board, winType, winIndex);
        if(win!='N'){
            displayBoard(board, winType, winIndex);
            cout << "\n*** PLAYER " << win << " WINS ***\n";
            Beep(900,200);
            break;
        }

        if(isDraw(board)){
            displayBoard(board);
            cout<<" DRAW GAME \n";
            break;
        }

        current=(current=='X')?'O':'X';
    }
    system("pause");
}
