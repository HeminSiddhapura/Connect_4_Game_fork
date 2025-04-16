// DSA INNOVATIVE ASSIGNMENT 
// TOPIC : CONNECT 4 Game Using Minimax Tree Algorithm.
// By:-
// 20BCE189 
// 20BCE195
// 20BCE211 
//-------------------------------------------------------------------------------//
#include <stdio.h>
#include <iostream>
#include <limits.h>
#include <array>
#include <sstream>
//-------------------------------------------------------------------------------//
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
//-------------------------------------------------------------------------------//

using namespace std;
const std::string red("\033[1;31m");
const std::string yellow("\033[1;33m");
const std::string reset("\033[0m");

//-------------------------------------------------------------------------------//

int PLAYER = 1; // player number
int COMPUTER = 2; // AI number
int MAX_DEPTH; // the default "difficulty" of the computer controlled AI
bool gameOver; // flag for if game is over
int turns; // count for # turns
int currentPlayer; // current player
int board[6][7]; // the game board

//-------------------------------------------------------------------------------//
// function declarations
void printBoard();
int userMove();
void makeMove(int [6][7], int, int);
int aiMove();
void copyBoard(int [6][7],int [6][7]);
bool winningMove(int [6][7], int);
int scoreSet(int [], int);
int tabScore(int [6][7],  int);
array<int, 2> miniMax(int [6][7], int, int, int,  int);
int heurFunction(int, int, int);
void setDifficulty();
bool vsAI = true; // true: Player vs AI, false: 2-Player mode

//-------------------------------------------------------------------------------//

void playGame() 
{
	printBoard(); 
	while (!gameOver) 
	{
		if (vsAI) 
		{
			if (currentPlayer == COMPUTER) 
			{
				makeMove(board, aiMove(), COMPUTER);
				gameOver = winningMove(board, COMPUTER);
			}
			else 
			{
				makeMove(board, userMove(), PLAYER);
				gameOver = winningMove(board, PLAYER);
			}
		}
		else 
		{
			cout << "Player " << currentPlayer << "'s turn:" << endl;
			makeMove(board, userMove(), currentPlayer);
			gameOver = winningMove(board, currentPlayer);
		}

		cout << endl;
		printBoard();

		if (gameOver) break;

		turns++;
		if (turns == 6 * 7) 
		{
			gameOver = true;
			break;
		}

		currentPlayer = (currentPlayer == PLAYER) ? COMPUTER : PLAYER;
	}

	if (turns == 6 * 7 && !winningMove(board, PLAYER) && !winningMove(board, COMPUTER)) 
	{
		cout << "Draw!" << endl;
	}
	else 
	{
		if (vsAI)
			cout << ((currentPlayer == PLAYER) ? "Player Wins!" : "AI Wins!") << endl;
		else
			cout << "Player " << currentPlayer << " Wins!" << endl;
	}
}
//-------------------------------------------------------------------------------//

void makeMove(int b[6][7], int c,  int p) 
{
	for (int r = 0; r < 6; r++) 
	{
		if (b[r][c] == 0) 
		{ // first available spot
			b[r][c] = p; // set piece
			break;
		}
	}
}
//-------------------------------------------------------------------------------//

int userMove() 
{
	int move = -1; 
	while (true) 
	{
		cout << "Enter a column: ";
		cin >> move; 
		if (!((int)move < 7 && move >= 0)) 
		{ 
		    cout << "INVALID COLUMN ";
		}
		else if (board[5][move] != 0) 
		{ 
		    cout << "COLUMN IS FULL ";
		}
		else { // if it gets here, input valid
			break;
		}
		cout << endl << endl;
	}
	return move;
}
//-------------------------------------------------------------------------------//

int aiMove() 
{
	cout << "AI is thinking about a move..." << endl;
	return miniMax(board, MAX_DEPTH, 0 - INT_MAX, INT_MAX, COMPUTER)[1];
}
//-------------------------------------------------------------------------------//

array<int, 2> miniMax(int b[6][7],  int d, int alf, int bet, int p)
{
	if (d == 0 || d >= (7 * 6) - turns) 
	{
		array<int, 2> arr = {tabScore(b, COMPUTER), -1};
        return arr;
	}
	if (p == COMPUTER) 
	{ 
		array<int, 2> moveSoFar = {INT_MIN, -1}; // since maximizing, we want lowest possible value
		
		if (winningMove(b, PLAYER)) 
		{ // if player about to win
			return moveSoFar; // force it to say it's worst possible score, so it knows to avoid move
		} // otherwise, business as usual
		
		for ( int c = 0; c < 7; c++) 
		{ // for each possible move
			if (b[5][c] == 0) 
			{ // but only if that column is non-full
			    int newBoard[6][7];
				copyBoard(newBoard,b); // make a copy of the board
				makeMove(newBoard, c, p); // try the move
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER)[0]; // find move based on that new board state
				if (score > moveSoFar[0]) 
				{ // if better score, replace it, and consider that best move (for now)
					moveSoFar[0]= score;
                    moveSoFar[1]= (int)c;
				}
				alf = max(alf, moveSoFar[0]);
				if (alf >= bet) { break; } // for pruning
			}
		}
		return moveSoFar; // return best possible move
	}
	else 
	{
		array<int, 2> moveSoFar = {INT_MAX, -1}; // since PLAYER is minimized, we want moves that diminish this score
		if (winningMove(b, COMPUTER)) 
		{
			return moveSoFar; // if about to win, report that move as best
		}
		for ( int c = 0; c < 7; c++) 
		{
			if (b[5][c] == 0) 
			{
				int newBoard[6][7];
				copyBoard(newBoard,b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER)[0];
				if (score < moveSoFar[0]) 
				{
					moveSoFar[0]= score;
                    moveSoFar[1]= (int)c;
				}
				bet = min(bet, moveSoFar[0]);
				if (alf >= bet) { break; }
			}
		}
		return moveSoFar;
	}
}
//-------------------------------------------------------------------------------//

int tabScore(int b[6][7],  int p) {
	int score = 0;
	int rs[7];
	int cs[6];
	int set[4];
	
	
	for ( int r = 0; r < 6; r++) 
	{
		for ( int c = 0; c < 7; c++) 
		{
			rs[c] = b[r][c]; // this is a distinct row alone
		}
		for ( int c = 0; c < 4; c++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
			}
			score += scoreSet(set, p); // find score
		}
	}
	// vertical
	for ( int c = 0; c < 7; c++) 
	{
		for ( int r = 0; r < 6; r++) 
		{
			cs[r] = b[r][c];
		}
		for ( int r = 0; r < 3; r++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	// diagonals
	for ( int r = 0; r < 6 - 3; r++) 
	{
		for ( int c = 0; c < 7; c++) 
		{
			rs[c] = b[r][c];
		}
		for ( int c = 0; c < 4; c++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for ( int r = 0; r <3; r++) 
	{
		for ( int c = 0; c < 7; c++) 
		{
			rs[c] = b[r][c];
		}
		for ( int c = 0; c < 4; c++) 
		{
			for (int i = 0; i < 4; i++) 
			{
				set[i] = b[r + 3 - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score;
}
//-------------------------------------------------------------------------------//

int scoreSet(int v[],  int p) {
	 int good = 0; // points in favor of p
	 int bad = 0; // points against p
	 int empty = 0; // neutral spots
	for ( int i = 0; i < 4; i++) 
	{ // just enumerate how many of each
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == PLAYER || v[i] == COMPUTER) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	// bad was calculated as (bad + good), so remove good
	bad -= good;
	return heurFunction(good, bad, empty);
}
//-------------------------------------------------------------------------------//

int heurFunction( int g,  int b,  int z) {
	int score = 0;
	if (g == 4) { score += 500001; } // preference to go for winning move
	else if (g == 3 && z == 1) { score += 5000; } // 3 'X' and 1 Empty
	else if (g == 2 && z == 2) { score += 500; } // 2 'X' and 2 Empty
	else if (b == 2 && z == 2) { score -= 501; } // 2 'O' and 2 Empty
	else if (b == 3 && z == 1) { score -= 5001; } // 3 'O' and 1 Empty
	else if (b == 4) { score -= 500000; } // Block the Opposition Winning Move
	return score;
}
//-------------------------------------------------------------------------------//

bool winningMove(int b[6][7], int p) {
    // Horizontal check
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c <= 7 - 4; c++) {
            if (b[r][c] == p && b[r][c+1] == p && b[r][c+2] == p && b[r][c+3] == p)
                return true;
        }
    }

    // Vertical check
    for (int c = 0; c < 7; c++) {
        for (int r = 0; r <= 6 - 4; r++) {
            if (b[r][c] == p && b[r+1][c] == p && b[r+2][c] == p && b[r+3][c] == p)
                return true;
        }
    }

    // Diagonal (\) check
    for (int r = 0; r <= 6 - 4; r++) {
        for (int c = 0; c <= 7 - 4; c++) {
            if (b[r][c] == p && b[r+1][c+1] == p && b[r+2][c+2] == p && b[r+3][c+3] == p)
                return true;
        }
    }

    // Diagonal (/) check
    for (int r = 3; r < 6; r++) {
        for (int c = 0; c <= 7 - 4; c++) {
            if (b[r][c] == p && b[r-1][c+1] == p && b[r-2][c+2] == p && b[r-3][c+3] == p)
                return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------//

void initBoard() // to reset the board
{
	for (int r = 0; r < 6; r++) 
	{
		for (int c = 0; c < 7; c++) 
		{
			board[r][c] = 0; // make sure board is empty initially
		}
	}
}
//-------------------------------------------------------------------------------//

void copyBoard(int a[6][7],int b[6][7])
{
    for(int i =0 ; i < 6 ; i++)
    {
        for(int j = 0 ; j < 7 ; j++)
        {
            a[i][j] = b[i][j];
        }
    }
}
//-------------------------------------------------------------------------------//

void printBoard() 
{
	cout << endl << "-0-1-2-3-4-5-6-" << endl;
	for ( int r = 0; r < 6; r++) 
	{
		for ( int c = 0; c < 7; c++) 
		{
			cout << "|";
			switch (board[5 - r][c]) {
			case 0: cout << " "; break; // no piece
			case 1: cout << yellow << "O" << reset; break; // one player's piece
			case 2: cout << red << "X" << reset; break; // other player's piece
			}
			if (c + 1 == 7) { cout << "|"; }
		}
		cout << endl;
	}
	cout << "---------------" << endl;
	cout << endl;
}
//-------------------------------------------------------------------------------//

void setDifficulty()
{
    int x;
    while(true)
    {
        cout <<"\n---------------------------\nSelect difficulty Of AI :-\n1.Easy\n2.Medium\n3.Hard" << endl;
        cin >> x ;
        cout <<"---------------------------" << endl;
        switch(x)
        {
            case 1: 
                MAX_DEPTH = 1;
                break;
            case 2: 
                MAX_DEPTH = 3;
                break;
            case 3:
                MAX_DEPTH = 5; 
                break;
            default:
                cout << "Error!! Enter again :" << endl;
        }
        if(MAX_DEPTH == 1 || MAX_DEPTH == 3 || MAX_DEPTH == 5)
            break;
    }
}
//-------------------------------------------------------------------------------//

int main() 
{
    system("clear");
    int ch=1;
    cout << "_______________________20BCE189_______________________" << endl;
    cout << "_______________________20BCE195_______________________" << endl;
    cout << "_______________________20BCE211_______________________" << endl << endl << endl;
    while(ch != 0)
    {
        cout <<"---------------------------\n0 - To Exit The Game.\n1 - To Continue To Play" << endl << endl;
        cout << "Enter your choice : ";
        cin >> ch;
        cout <<"---------------------------" << endl;
        if(ch == 0)
            break;
    
        int modeChoice;
        while (true)
        {
            cout << "\nChoose Game Mode:\n1. Player vs AI\n2. 2 Player Mode" << endl;
            cin >> modeChoice;
            if (modeChoice == 1)
            {
                vsAI = true;
                setDifficulty();
                break;
            }
            else if (modeChoice == 2)
            {
                vsAI = false;
                break;
            }
            else
            {
                cout << "Invalid input. Try again." << endl;
            }
        }
    
        gameOver = false;
        turns = 0;
        currentPlayer = PLAYER;
        initBoard();
        playGame();
    }
    
    
	return 0;
}
//-------------------------------------------------------------------------------//
//---------------------------------- END OF CODE --------------------------------//
//-------------------------------------------------------------------------------//

