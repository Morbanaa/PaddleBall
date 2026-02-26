/*
============================================================
Teddy Rodd
Morbanaa Studios
Paddle Ball
============================================================

PROGRAM DESCRIPTION:
Paddle Ball is a console-based arcade game written in C++.
The player controls a horizontal paddle at the bottom of
the screen and must bounce the ball to prevent it from
falling past the paddle.

Each successful bounce increases the player's score.
If the ball falls below the paddle, the game resets.

------------------------------------------------------------
CONTROLS:
LEFT ARROW  - Move paddle left
RIGHT ARROW - Move paddle right

------------------------------------------------------------
GAME RULES:
- Bounce the ball using the paddle.
- Different sections of the paddle change the ball’s angle.
- The ball speeds across the screen and rebounds off walls.
- Missing the ball resets the score and positions.

------------------------------------------------------------
TECHNICAL FEATURES:
- 50 x 25 console playfield
- Real-time keyboard input using Windows API
- ASCII-based rendering
- Basic collision physics
- Frame-controlled game loop (50ms delay)

------------------------------------------------------------
Built with:
C++ | Windows Console | ANSI Color Codes

============================================================
*/

#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
using namespace std;

const int WORLDWIDTH = 50;
const int WORLDHEIGHT = 25;

char map[WORLDHEIGHT][WORLDWIDTH];

class Player {
protected:
	int playerXpos = WORLDWIDTH / 2 + 2;
	int playerYpos = WORLDHEIGHT - 3;
	int score = 0;

};

class Ball {
protected:
	int ballXpos = WORLDWIDTH / 2;
	int ballYpos = WORLDHEIGHT / 3;
	int yDir = 1; // -1 == Down 1 == Up
	int xDir = 0; // -1 == Left 0 == Straight 1 == Right
};

class World : public Player, public Ball {
public:

	//============================
	//=     Create World
	//============================
	void createWorld() {
		for (int y = 0; y < WORLDHEIGHT; y++) {
			for (int x = 0; x < WORLDWIDTH; x++) {
				if (y < 1) {
					map[y][x] = '#';
				}
				else if (x < 1 || y > WORLDHEIGHT - 2 || x > WORLDWIDTH - 2) {
					map[y][x] = '@';
				}
				else if (y == playerYpos && x == playerXpos) {
					map[playerYpos][playerXpos - 4] = '=';
					map[playerYpos][playerXpos - 3] = '=';
					map[playerYpos][playerXpos - 2] = '=';
					map[playerYpos][playerXpos - 1] = '=';
					map[playerYpos][playerXpos] = '=';
				}
				else {
					map[y][x] = ' ';
				}

			}
		}
	}

	//============================
	//=     Draw World
	//============================
	void drawWorld() {
		for (int y = 0; y < WORLDHEIGHT; y++) {
			for (int x = 0; x < WORLDWIDTH; x++) {
				if (y == ballYpos && x == ballXpos) {
					cout << "\033[31m";
					cout << "o";
					cout << "\033[0m";

				}
				else {
					cout << map[y][x];
				}

			}
			cout << endl;
		}
	}

	//============================
	//=     Controls
	//============================
	void controls() {
		if (GetAsyncKeyState(VK_LEFT)) {
			if (map[playerYpos][playerXpos - 3] != '@') {
				playerXpos--;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			if (map[playerYpos][playerXpos + 1] != '@') {
				playerXpos++;
			}
		}
	}

	//============================
	//=     Ball X Y Movement
	//============================
	void ballMove() {
		ballYpos += yDir;
		ballXpos += xDir;
	}

	//============================
	//=     Collision Mesh
	//============================
	void collision() {
		// Leaves Bottom of Screen
		if (ballYpos > playerYpos + 1) {
			reset();
		}
		// Hits Paddle
		else if (ballYpos == playerYpos) {
			if (ballXpos == playerXpos) {
				xDir = 2;
				yDir = -1;
				score++;
			}
			else if (ballXpos == playerXpos - 1) {
				xDir = 1;
				yDir = -1;
				score++;
			}
			else if (ballXpos == playerXpos - 2) {
				xDir = 0;
				yDir = -1;
				score++;
			}
			else if (ballXpos == playerXpos - 3) {
				xDir = -1;
				yDir = -1;
				score++;
			}
			else if (ballXpos == playerXpos - 4) {
				xDir = -2;
				yDir = -1;
				score++;
			}
		}
		// Hits Top Walls
		else if (map[ballYpos][ballXpos] == '#') {
			yDir = 1;
		}

		// Hits Sides Wall
		else if (map[ballYpos][ballXpos] == '@') {

			if (xDir == -2) {
				xDir = 2;
			}
			else if (xDir == -1) {
				xDir = 1;
			}
			else if (xDir == 0) {
				xDir = 0;
			}
			else if (xDir == 1) {
				xDir = -1;
			}
			else if (xDir == 2) {
				xDir = -2;
			}
		}
	}

	//============================
	//=     Reset Game
	//============================
	void reset() {
		playerXpos = WORLDWIDTH / 2 + 2;
		playerYpos = WORLDHEIGHT - 3;
		score = 0;

		ballXpos = WORLDWIDTH / 2;
		ballYpos = WORLDHEIGHT / 3;
		yDir = 1; // -1 == Down 1 == Up
		xDir = 0; // -1 == Left 0 == Straight 1 == Right

		this_thread::sleep_for(chrono::seconds(1));
	}

	//============================
	//=   Reset Cursor Next Frame
	//============================
	void moveCursorTopLeft() {
		COORD coord = { 0, 0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	//============================
	//=     Main Game Loop
	//============================
	void runGame() {
		bool run = true;

		do {
			cout << "\033[32m";
			cout << "PADDLE BALL  || MORBANAA STUDIOS\n";
			cout << "\033[0m";
			createWorld();
			drawWorld();
			cout << "\033[32m";
			cout << "Score: " << score << "         ";
			cout << "\033[0m";
			controls();
			collision();
			ballMove();
			moveCursorTopLeft();
			this_thread::sleep_for(chrono::milliseconds(50));
		} while (run);
	}

};

//============================
//=     Program Entry
//============================
int main() {
	World worldOne;
	worldOne.runGame();

	system("pause>0");
	return 0;
}

