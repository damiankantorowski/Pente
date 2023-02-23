#include <iostream>
#include <string>
#include "pente.h"
using namespace std;

player::player() :
	name(""),
	captures(0),
	color('0')
{}

computer::computer() {
	name = "komputer";
}

point::point() : //cant initialize point with (0, 0) because it would be a correct move
	x(-1), 
	y(-1) 
{}

gameboard::gameboard() { //fills the board with blanks
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			board[i][j] = '.';
}

void gameboard::print() { //prints the board
	title();
	cout << " __________________________________________\n";
	cout << "|    A B C D E F G H I J K L M N O P Q R S |\n";
	for (int i = 0; i < 19; i++) {
		cout << "| ";
		if (i > 9) cout << ' ';
		cout << 19 - i << ' ';
		for (int j = 0; j < 19; j++) {
			if (board[i][j] == 'B')
				cout << "\u25CB";
			else if (board[i][j] == 'W')
				cout << "\u25CF";
			else
				cout << board[i][j];
			if (j < 18)	cout << ' ';
		}
		cout << " |\n";
	}
	cout << "|__________________________________________|\n";
	cout << "       *Aby wr\u00F3ci\u0107 do menu wprowad\u017A 0*\n";
}

void gameboard::make_move(char color, int x, int y) { //changes board's coordinates to given color
	board[x][y] = color;
}

bool gameboard::isFull() { //checks if all spaces are diffrent from the period 
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (board[i][i] == '.')
				return false;
	return true;
}

bool point::isLegal(gameboard* gb) { //checks if point's coordinates fit on the board and aren't already occupied
	if (x >= 0 && x < 19 &&
		y >= 0 && y < 19 &&
		gb->board[x][y] == '.')
		return 1;
	return 0;
}

int point::captures(gameboard* gb, char color) { //returns number of pieces captured with a move of coordinates (x, y) and removes them from gameboard
	//Xs capture Os when two Os are surrounded by two Xs horizontally, vertically or diagonally
	//                  X          X                X
	// X O O X    or    O    or     O      or      O
	//                  O            O            O
	//                  X             X          X 
	int count = 0;
	char enemycolor;
	if (color == 'W')
		enemycolor = 'B';
	else
		enemycolor = 'W';
	//horizontally
	if (y > 2 &&
		gb->board[x][y - 1] == enemycolor &&
		gb->board[x][y - 2] == enemycolor &&
		gb->board[x][y - 3] == color) {
		gb->board[x][y - 1] = '.';
		gb->board[x][y - 2] = '.';
		count++;
	}
	if (y < 16 &&
		gb->board[x][y + 1] == enemycolor &&
		gb->board[x][y + 2] == enemycolor &&
		gb->board[x][y + 3] == color) {
		gb->board[x][y + 1] = '.';
		gb->board[x][y + 2] = '.';
		count++;
	}
	//vertically
	if (x > 2 &&
		gb->board[x - 1][y] == enemycolor &&
		gb->board[x - 2][y] == enemycolor &&
		gb->board[x - 3][y] == color) {
		gb->board[x - 1][y] = '.';
		gb->board[x - 2][y] = '.';
		count++;
	}
	if (x < 16 &&
		gb->board[x + 1][y] == enemycolor &&
		gb->board[x + 2][y] == enemycolor &&
		gb->board[x + 3][y] == color) {
		gb->board[x + 1][y] = '.';
		gb->board[x + 2][y] = '.';
		count++;
	}
	//diagonally
	if (x > 2 && y > 2 &&
		gb->board[x - 1][y - 1] == enemycolor &&
		gb->board[x - 2][y - 2] == enemycolor &&
		gb->board[x - 3][y - 3] == color) {
		gb->board[x - 1][y - 1] = '.';
		gb->board[x - 2][y - 2] = '.';
		count++;
	}
	if (x < 16 && y < 16 &&
		gb->board[x + 1][y + 1] == enemycolor &&
		gb->board[x + 2][y + 2] == enemycolor &&
		gb->board[x + 3][y + 3] == color) {
		gb->board[x + 1][y + 1] = '.';
		gb->board[x + 2][y + 2] = '.';
		count++;
	}
	if (x > 2 && y < 16 &&
		gb->board[x - 1][y + 1] == enemycolor &&
		gb->board[x - 2][y + 2] == enemycolor &&
		gb->board[x - 3][y + 3] == color) {
		gb->board[x - 1][y + 1] = '.';
		gb->board[x - 2][y + 2] = '.';
		count++;
	}
	if (x < 16 && y < 16 && y > 2 &&
		gb->board[x + 1][y - 1] == enemycolor &&
		gb->board[x + 2][y - 2] == enemycolor &&
		gb->board[x + 3][y - 3] == color) {
		gb->board[x + 1][y - 1] = '.';
		gb->board[x + 2][y - 2] = '.';
		count++;
	}
	return 2 * count;
}

bool player::isWinner(gameboard* gb) { //checks if player has already won
	if (captures >= 10) //player won if his score is >=10
		return true;
	//player won if 5 adjacent spaces are his color
	//horizontaly
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 15; j++)
			if (gb->board[i][j] == color &&
				gb->board[i][j + 1] == color &&
				gb->board[i][j + 2] == color &&
				gb->board[i][j + 3] == color &&
				gb->board[i][j + 4] == color)
				return true;
	//vertically
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 19; j++)
			if (gb->board[i][j] == color &&
				gb->board[i + 1][j] == color &&
				gb->board[i + 2][j] == color &&
				gb->board[i + 3][j] == color &&
				gb->board[i + 4][j] == color)
				return true;
	//diagonally
	for (int i = 0; i < 15; i++)
		for (int j = 4; j < 19; j++)
			if (gb->board[i][j] == color &&
				gb->board[i + 1][j - 1] == color &&
				gb->board[i + 2][j - 2] == color &&
				gb->board[i + 3][j - 3] == color &&
				gb->board[i + 4][j - 4] == color)
				return true;
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			if (gb->board[i][j] == color &&
				gb->board[i + 1][j + 1] == color &&
				gb->board[i + 2][j + 2] == color &&
				gb->board[i + 3][j + 3] == color &&
				gb->board[i + 4][j + 4] == color)
				return true;
	return false;
}

point* computer::get_move() { //randomly selects two numbers from range [0,18] and returns them as a new point object 
	point* move = new point;
	if (move == NULL)
		return NULL;
	move->x = rand() % 19;
	move->y = rand() % 19;
	return move;
}

point* human::get_move() { //reads x and y coordinates from a player and returns them as a new point object  
	string input;
	point* move = new point;
	if (move == NULL)
		return NULL;
	cout << "Ruch (np. A19) gracza " << name;
	if (color == 'W')
		cout << " (bia\u0142e): ";
	else
		cout << " (czarne): ";
	getline(cin, input);
	if (input[0] == '0') {
		delete move;
		return NULL;
	}
	if (isalpha(input[0])) {
		move->y = toupper(input[0]) - 65; //where A means y = 0, B means y = 1 etc.
		if (isdigit(input[1])) {
			if (isdigit(input[2]))
				move->x = 19 - (10 * (input[1] - '0') + input[2] - '0'); //where 19 means x = 0, 18 means x = 1 etc.
			else
				move->x = 19 - (input[1] - '0'); //where 1 means x = 18, 2 means x = 17 etc. 
		}
	}
	return move;
}