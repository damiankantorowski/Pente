#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include "pente.h"
using namespace std;

void title() { //prints a title
	system("cls");
	cout << " ~~~~~~~~~~~~~~~~~-=PENTE=-~~~~~~~~~~~~~~~~~\n";
}

int menu() { //prints main menu and loops for a mode selection
	int mode = -1;
	title();
	cout << " 1. Nowa gra z komputerem\n";
	cout << " 2. Nowa gra 1 vs 1\n";
	cout << " 3. Wczytaj ostatni\u0105 gr\u0119\n";
	cout << " 4. Zasady\n";
	cout << " 0. Wyj\u015Bcie\n ";
	while (mode < 0 || mode > 4)
		mode = _getch() - '0';
	if (mode == 4) {
		title();
		cout << " Celem gry  jest u\u0142o\u017Cenie pi\u0119ciu lub  wi\u0119cej\n";
		cout << " kamieni  swojego koloru  w ci\u0105g\u0142ej  linii -\n";
		cout << " poziomej,  pionowej lub uko\u015Bnej b\u0105d\u017A zbicie\n";
		cout << " 10  kamieni  przeciwnika.  Pierwszy  gracz,\n";
		cout << " kt\u00F3ry tego dokona, zostaje zwyci\u0119zc\u0105; je\u015Bli\n";
		cout << " nie uda si\u0119 to nikomu (plansza zostanie za-\n";
		cout << " pe\u0142niona), nast\u0119puje remis.  Bicie  kamieni\n";
		cout << " przeciwnika   nast\u0119puje   przez   otoczenie\n";
		cout << " pary  jego  s\u0105siaduj\u0105cych   kamieni  z  obu\n";
		cout << " stron. Zbite kamienie s\u0105 usuwane z planszy,\n";
		cout << " a  pola, kt\u00F3re okupowa\u0142y, mog\u0105 by\u0107 ponownie\n";
		cout << " wykorzystane w grze. W jednym  ruchu  mo\u017Cna\n";
		cout << " zbi\u0107 wi\u0119cej ni\u017C jedn\u0105 par\u0119.\n "; //copyright cc by-nc 2.0 2008 Marek Futrega gomoku.5v.pl
		char c = _getch();
		return -1;
	}
	return mode;
}

void game(int mode) { //main game instructions are here
	ifstream input;
	ofstream output;
	gameboard board;
	human playerOne;
	human playerTwo;
	computer playerComputer;
	player* player1 = &playerOne;
	player* player2 = NULL;
	//pointer to first player always points to a human type object
	//pointer to second player should point to a computer type object if
	//oponent is a computer and to a human type object if not
	int turn = rand() % 2 + 1; //randomly selects which player starts
	if (turn == 1) { //setting players' colors based on selected first turn order
		playerOne.color = 'W'; //white always goes first
		playerTwo.color = playerComputer.color = 'B';
	}
	else {
		playerOne.color = 'B';
		playerTwo.color = playerComputer.color = 'W';
	}
	title();
	switch (mode) { //mode is returned by menu()
	case 1:
		cout << " Podaj nazw\u0119 gracza: ";
		getline(cin, playerOne.name);
		player2 = &playerComputer; //pointer to second player should point to a computer type object
		break;
	case 2:
		cout << " Podaj nazw\u0119 1. gracza: ";
		getline(cin, playerOne.name);
		title();
		cout << " Podaj nazw\u0119 2. gracza: ";
		getline(cin, playerTwo.name);
		player2 = &playerTwo; //pointer to second player should point to a human type object 
		break;
	case 3: //reading game's state from file
		//state.txt containts last game's state formatted like this
		//B . W . W
		//. . B W . (19x19 board where W = white and B = black)
		//. . B . .
		//1 2 (mode and turn)
		//B 0 (color1 and score1)
		//W 2 (color2 and score2)
		//name1
		//name2
		input.open("state.txt");
		if (!input.is_open()) { //terminates game if state.txt is inaccessible
			title();
			cout << " B\u0142\u0105d wczytywania zapisu gry.\n ";
			char c = _getch();
			return;
		}
		for (int i = 0; i < 19; i++)
			for (int j = 0; j < 19; j++)
				input >> board.board[i][j];
		input >> mode >> turn;
		//pointer to second player should point to a computer type object
		//if last game was played with computer and to a human type object if not
		if (mode == 1)
			player2 = &playerComputer;
		else
			player2 = &playerTwo;
		input >> player1->color >> player1->captures;
		input >> player2->color >> player2->captures;
		input.ignore();
		getline(input, player1->name);
		getline(input, player2->name);
		input.close();
		break;
	default:
		return;
	}
	while (true) { //loops for turns
		board.print();
		cout << "Zbite kamienie: " << player1->name << " - " << player1->captures << " ";
		cout << player2->name << " - " << player2->captures << endl;
		//ends the game is any player has won already or the board is full
		if (player1->isWinner(&board)) {
			cout << player1->name << " wygra\u0142!\n";
			char c = _getch();
			break;
		}
		if (player2->isWinner(&board)) {
			cout << player2->name << " wygra\u0142!\n";
			char c = _getch();
			break;
		}
		if (board.isFull()) { //there's a draw
			cout << " Remis!\n";
			char c = _getch();
			break;
		}
		if (turn == 1) { //first player's turn
			point* move = player1->get_move();
			if (move == NULL) //exits game if player entered 0 or not enough memory
				break;
			if (move->isLegal(&board)) { //updates the board if a move is legal
				board.make_move(player1->color, move->x, move->y);
				player1->captures += move->captures(&board, player1->color); //checks for captures and updates player's score
				turn = 2; //next turn is second player's
				delete move;
			}
			else {
				delete move;
				continue;
			}
		}
		else { //second player's turn
			point* move = player2->get_move();
			if (move == NULL) //exits game if player entered 0 or not enough memory
				break;
			if (move->isLegal(&board)) { //updates the board if a move is legal
				board.make_move(player2->color, move->x, move->y); 
				player2->captures += move->captures(&board, player2->color); //checks for captures and updates player's score
				turn = 1; //next turn is first player's
				delete move;
			}
			else {
				delete move;
				continue;
			}
		}
	}
	//saving all data necessary to reopen an interrupted game
	output.open("state.txt");
	if (output.is_open()) {
		for (int i = 0; i < 19; i++) {
			for (int j = 0; j < 19; j++)
				output << board.board[i][j] << " ";
			output << endl;
		}
		output << mode << " " << turn << endl;
		output << player1->color << " " << player1->captures << endl;
		output << player2->color << " " << player2->captures << endl;
		output << player1->name << endl;
		output << player2->name << endl;
		output.close();
	}
}

int main() {
	srand((int)time(NULL));
	SetConsoleOutputCP(65001); //sets console code page to utf-8
	int mode;
	while (mode = menu()) //game loop
		game(mode);
	return 0;
}
