#pragma once

void title();

int menu();

void game(int);

class gameboard {
	char board[19][19];
public:
	gameboard();
	void print();
	void make_move(char, int, int);
	bool isFull();
	friend class point;
	friend class player;
	friend void game(int);
};

class point {
public:	
	int x;
	int y;
	point();
	bool isLegal(gameboard*);
	int captures(gameboard*, char);
};

class player {
public:
	std::string name;
	char color;
	int captures;
	player();
	virtual point* get_move() = 0;
	bool isWinner(gameboard*);
};

class computer : public player {
public:
	computer();
	point* get_move();
};

class human : public player {
public:
	point* get_move();
};