#include <iostream>
#include <graphics.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
void typingEffect(const string &message, int x, int y, int delayTime) {
    setcolor(LIGHTGREEN);                      
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3); 

    string displayed = "";               
    for (int i = 0; i < message.length(); i++) {
        displayed += message[i];          
        cleardevice();                    
        outtextxy(x, y, const_cast<char*>(displayed.c_str())); 
        delay(delayTime);                 
    }
}

string getInput(int x, int y, const string &prompt) {
    setcolor(LIGHTGREEN); 
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);

    
    outtextxy(x, y, const_cast<char*>(prompt.c_str()));

    char input[50] = ""; 
    int len = 0;        

    while (true) {
        char key = getch(); 

        
        if (key == 8 && len > 0) { 
            len--;               
            input[len] = '\0';    

           
            setcolor(BLACK); 
            bar(x + textwidth(const_cast<char*>(prompt.c_str())) + 10, y,
                x + textwidth(const_cast<char*>(prompt.c_str())) + 400, y + textheight(input));
        }
       
        else if (key == 13) { 
            break; 
        }
        
        else if (key >= 32 && key <= 126) { 
            input[len++] = key; 
            input[len] = '\0';  
        }

        
        setcolor(WHITE);
        outtextxy(x + textwidth(const_cast<char*>(prompt.c_str())) + 10, y, input);
    }

    return string(input); 
}

void saveGame(const string &playerX, const string &playerO, int scoreX, int scoreO) {
    ofstream outFile("tictactoe_save.txt", ios::app);
    if (outFile) {
        outFile << left << setw(10) << playerX << setw(10) << scoreX
                << left << setw(10) << playerO << setw(10) << scoreO << endl;
    }
    outFile.close();
}


bool loadGame(string &playerX, string &playerO, int &scoreX, int &scoreO) {
    ifstream inFile("tictactoe_save.txt");
    if (!inFile) {
        return false; 
    }

    string line, lastLine;
    while (getline(inFile, line)) {
        if (!line.empty()) {
            lastLine = line; 
        }
    }
    inFile.close();

    if (lastLine.empty()) {
        return false; 
    }

   
    istringstream ss(lastLine);
    ss >> playerX >> scoreX >> playerO >> scoreO;

    return true;
}
string toString(int value) {
    stringstream ss;
    ss << value;
    return ss.str();
}


void drawBoard() {
    setcolor(WHITE);
    line(200, 100, 200, 400);
    line(300, 100, 300, 400);
    line(100, 200, 400, 200);
    line(100, 300, 400, 300);
}


void drawX(int row, int col) {
    int x = 150 + col * 100;
    int y = 150 + row * 100;
    setcolor(RED);

    for (int i = -2; i <= 2; i++) {
        line(x - 25 + i, y - 25, x + 25 + i, y + 25);
        line(x - 25 + i, y + 25, x + 25 + i, y - 25);
    }
}


void drawO(int row, int col) {
    int x = 150 + col * 100;
    int y = 150 + row * 100;
    setcolor(BLUE);

    for (int i = 0; i <= 3; i++) {
        circle(x, y, 25 - i);
    }
}
struct WinningLine {
    char winner; 
    int startX, startY, endX, endY;
};

WinningLine checkWinner(char board[3][3]) {
    WinningLine result = {' ', 0, 0, 0, 0};

  
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            result = {board[i][0], 100, 150 + i * 100, 400, 150 + i * 100};
            return result;
        }
    }

   
    for (int i = 0; i < 3; i++) {
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            result = {board[0][i], 150 + i * 100, 100, 150 + i * 100, 400};
            return result;
        }
    }

 
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        result = {board[0][0], 100, 100, 400, 400};
        return result;
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        result = {board[0][2], 400, 100, 100, 400};
        return result;
    }

    return result; 
}



void playGame(const string &playerX, const string &playerO, int &scoreX, int &scoreO) {
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    int moves = 0, turn = 0;

    cleardevice();
    drawBoard();

    while (true) {
        string turnText = (turn == 0 ? playerX + "'s Turn" : playerO + "'s Turn");
        setcolor(LIGHTGREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(150, 420, const_cast<char *>(turnText.c_str()));

        int x, y, row, col;
        while (!ismouseclick(WM_LBUTTONDOWN))
        delay(100);
        getmouseclick(WM_LBUTTONDOWN, x, y);

        if (x >= 100 && x < 400 && y >= 100 && y < 400) {
            col = (x - 100) / 100;
            row = (y - 100) / 100;

            if (board[row][col] == ' ') {
                board[row][col] = (turn == 0) ? 'X' : 'O';
                moves++;
                (turn == 0) ? drawX(row, col) : drawO(row, col);

                WinningLine winner = checkWinner(board);
                if (winner.winner != ' ') {
                    string winnerText = (winner.winner == 'X' ? (playerX + " Wins!") : (playerO + " Wins!"));
                    setcolor(YELLOW);
                    outtextxy(150, 450, const_cast<char *>(winnerText.c_str()));

                  
                    setcolor(GREEN); 
                    setlinestyle(SOLID_LINE, 0, 3); 
                    line(winner.startX, winner.startY, winner.endX, winner.endY);

                    delay(2000);

                    if (winner.winner == 'X')
                        scoreX++;
                    else
                        scoreO++;

                    break;
                } else if (moves == 9) {
                    setcolor(YELLOW);
                    outtextxy(150, 450, "It's a Draw!"); 
					delay(1000);               
                    break;
                }
                turn = 1 - turn;
            }
        }
    }
}
char displayMenu() {
    cleardevice();
  
    settextstyle(BOLD_FONT, HORIZ_DIR, 4); 
    setcolor(LIGHTCYAN);
    outtextxy(100, 50, "TIC TAC TOE MENU");

  
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2); // Simple font, size 2
    setcolor(LIGHTGREEN);
    outtextxy(50, 150, "Press 'N' for New Game");
    outtextxy(50, 200, "Press 'L' to Load Previous Game");
    outtextxy(50, 250, "Press 'E' to Exit");

    
    while (true) {
        char choice = getch();
        if (choice == 'N' || choice == 'n' || choice == 'L' || choice == 'l' || choice == 'E' || choice == 'e') {
            return choice;
        }
    }
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");
    typingEffect("WELCOME TO TIC TAC TOE", 100, 200, 200);
    delay(2000);
    cleardevice();
    while (true) {
        char choice = displayMenu();
        if (choice == 'E' || choice == 'e') {
            closegraph();
            return 0;
        }
        string playerX, playerO;
        int scoreX = 0, scoreO = 0;
        if (choice == 'L' || choice == 'l') {
        	cleardevice();
            if (loadGame(playerX, playerO, scoreX, scoreO)) {
                setcolor(LIGHTGREEN);
                outtextxy(30, 30, "Previous game loaded!");
                delay(1000);
            } else {
                setcolor(LIGHTGREEN);
                outtextxy(50, 50, "No saved games found. Starting a new game...");
                delay(2000);
                cleardevice();
                
                choice = 'N'; 
            }
        }
        if (choice == 'N' || choice == 'n') {
        	cleardevice();
            playerX = getInput(50, 50, "Enter Player X's name: ");
            playerO = getInput(50, 80, "Enter Player O's name: ");
        }
        cleardevice();
        playGame(playerX, playerO, scoreX, scoreO);
        saveGame(playerX, playerO, scoreX, scoreO);
    while (true) {
            cleardevice();
            setcolor(WHITE);
            settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
            outtextxy(100, 100, "Scores:");
            outtextxy(100, 150, const_cast<char*>(("Player X: " + toString(scoreX)).c_str()));
            outtextxy(100, 200, const_cast<char*>(("Player O: " + toString(scoreO)).c_str()));

            delay(1000);
            
            outtextxy(100, 300, "Press 'R' to Restart or 'E' to Exit");

            
            char choice = getch();
            if (choice == 'R' || choice == 'r') {
                cleardevice();
                break; 
            } else if (choice == 'E' || choice == 'e') {
                closegraph();
                exit(0);
            }
        }
    }

    return 0;
}

