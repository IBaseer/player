/**
 * @file main.cpp
 * @brief Main entry point for Board Games Collection 2025
 * @author CS213 Assignment 3
 *
 * This file integrates all 14 board games (including classic X-O) with a professional menu system.
 * Features: Windows API colors, sound effects, AI players, and clean interface.
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h>
#include <conio.h>

#include "BoardGame_Classes.h"
#include "Game0_StandardTicTacToe_Board.h"
#include "Game1_SUS_Board.h"
#include "Game2_FourInARow_Board.h"
#include "Game3_5x5TicTacToe_Board.h"
#include "Game4_WordTicTacToe_Board.h"
#include "Game5_MisereTicTacToe_Board.h"
#include "Game6_DiamondTicTacToe_Board.h"
#include "Game7_4x4TicTacToe_Board.h"
#include "Game8_PyramidTicTacToe_Board.h"
#include "Game9_NumericalTicTacToe_Board.h"
#include "Game10_ObstaclesTicTacToe_Board.h"
#include "Game11_InfinityTicTacToe_Board.h"
#include "Game12_UltimateTicTacToe_Board.h"
#include "Game13_MemoryTicTacToe_Board.h"

using namespace std;

// ======================== COLOR UTILITIES ========================
class ColorConsole {
public:
	static void setColor(int color) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}

	static void reset() { setColor(7); } // White
	static void cyan() { setColor(11); } // Bright Cyan
	static void yellow() { setColor(14); } // Bright Yellow
	static void green() { setColor(10); } // Bright Green
	static void red() { setColor(12); } // Bright Red
	static void magenta() { setColor(13); } // Bright Magenta
	static void white() { setColor(15); } // Bright White
};

// ======================== SOUND UTILITIES ========================
class SoundFX {
public:
	static void menuSelect() { Beep(800, 100); }
	static void validMove() { Beep(600, 80); }
	static void invalidMove() { Beep(300, 150); }
	static void win() {
		Beep(523, 150); Sleep(50);
		Beep(659, 150); Sleep(50);
		Beep(784, 150); Sleep(50);
		Beep(1047, 300);
	}
	static void draw() { Beep(400, 200); Sleep(100); Beep(400, 200); }
	static void startup() {
		Beep(523, 100); Sleep(50);
		Beep(659, 100); Sleep(50);
		Beep(784, 150);
	}
};

// ======================== DISPLAY UTILITIES ========================
void clearScreen() {
	system("cls");
}

void pauseScreen() {
	ColorConsole::yellow();
	cout << "\n\n";
	cout << "================================================\n";
	cout << "    Press any key to return to main menu...\n";
	cout << "================================================\n";
	ColorConsole::reset();
	_getch();
}

void displayHeader() {
	ColorConsole::cyan();
	cout << "\n";
	cout << "====================================================\n";
	cout << "        BOARD GAMES COLLECTION 2025\n";
	cout << "        CS213 - Cairo University FCAI\n";
	cout << "====================================================\n";
	ColorConsole::reset();
}

void displayMenu() {
	clearScreen();
	displayHeader();

	ColorConsole::white();
	cout << "\n  Select a game to play:\n\n";

	ColorConsole::yellow();
	cout << "   [0]  "; ColorConsole::white(); cout << "Standard Tic-Tac-Toe ";
	ColorConsole::green(); cout << "(Classic 3x3)\n";
	ColorConsole::yellow();
	cout << "   [1]  "; ColorConsole::white(); cout << "SUS\n";
	ColorConsole::yellow();
	cout << "   [2]  "; ColorConsole::white(); cout << "Four-in-a-Row\n";
	ColorConsole::yellow();
	cout << "   [3]  "; ColorConsole::white(); cout << "5x5 Tic Tac Toe\n";
	ColorConsole::yellow();
	cout << "   [4]  "; ColorConsole::white(); cout << "Word Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [5]  "; ColorConsole::white(); cout << "Misere Tic Tac Toe\n";
	ColorConsole::yellow();
	cout << "   [6]  "; ColorConsole::white(); cout << "Diamond Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [7]  "; ColorConsole::white(); cout << "4x4 Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [8]  "; ColorConsole::white(); cout << "Pyramid Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [9]  "; ColorConsole::white(); cout << "Numerical Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [10] "; ColorConsole::white(); cout << "Obstacles Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [11] "; ColorConsole::white(); cout << "Infinity Tic-Tac-Toe\n";
	ColorConsole::yellow();
	cout << "   [12] "; ColorConsole::white(); cout << "Ultimate Tic Tac Toe ";
	ColorConsole::magenta(); cout << "(BONUS)\n";
	ColorConsole::yellow();
	cout << "   [13] "; ColorConsole::white(); cout << "Memory Tic-Tac-Toe ";
	ColorConsole::magenta(); cout << "(BONUS)\n";

	ColorConsole::red();
	cout << "\n   [99] "; ColorConsole::white(); cout << "Exit\n";

	ColorConsole::cyan();
	cout << "\n====================================================\n";
	ColorConsole::green();
	cout << "Enter your choice: ";
	ColorConsole::reset();
}

// ======================== GAME LAUNCHERS ========================

void playGame0() {
	clearScreen();
	SoundFX::menuSelect();

	StandardTicTacToe_UI* ui = new StandardTicTacToe_UI();
	StandardTicTacToe_Board* board = new StandardTicTacToe_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame1() {
	clearScreen();
	SoundFX::menuSelect();

	SUS_UI* ui = new SUS_UI();
	SUS_Board* board = new SUS_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	// Display SUS count at end
	if (board->game_is_over(players[0])) {
		int total_sus = board->getTotalSUS();
		ColorConsole::cyan();
		cout << "\n=== GAME OVER ===\n";
		cout << "Total SUS sequences found: " << total_sus << "\n";
		if (total_sus == 0) {
			cout << "It's a draw! No SUS sequences.\n";
		}
		else {
			cout << "Game completed!\n";
		}
		ColorConsole::reset();
	}

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame2() {
	clearScreen();
	SoundFX::menuSelect();

	FourInARow_UI* ui = new FourInARow_UI();
	FourInARow_Board* board = new FourInARow_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame3() {
	clearScreen();
	SoundFX::menuSelect();

	FiveByFive_UI* ui = new FiveByFive_UI();
	FiveByFive_Board* board = new FiveByFive_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	// Display scores at end
	if (board->game_is_over(players[0])) {
		int scoreX = board->getPlayerScore('X');
		int scoreO = board->getPlayerScore('O');

		ColorConsole::cyan();
		cout << "\n=== GAME OVER ===\n";
		cout << players[0]->get_name() << " (X) score: " << scoreX << "\n";
		cout << players[1]->get_name() << " (O) score: " << scoreO << "\n";

		if (scoreX > scoreO) {
			ColorConsole::green();
			cout << players[0]->get_name() << " WINS!\n";
			SoundFX::win();
		}
		else if (scoreO > scoreX) {
			ColorConsole::green();
			cout << players[1]->get_name() << " WINS!\n";
			SoundFX::win();
		}
		else {
			ColorConsole::yellow();
			cout << "It's a DRAW!\n";
			SoundFX::draw();
		}
		ColorConsole::reset();
	}

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame4() {
	clearScreen();
	SoundFX::menuSelect();

	WordTicTacToe_UI* ui = new WordTicTacToe_UI();
	WordTicTacToe_Board* board = new WordTicTacToe_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame5() {
	clearScreen();
	SoundFX::menuSelect();

	Misere_UI* ui = new Misere_UI();
	Misere_Board* board = new Misere_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame6() {
	clearScreen();
	SoundFX::menuSelect();

	Diamond_UI* ui = new Diamond_UI();
	Diamond_Board* board = new Diamond_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame7() {
	clearScreen();
	SoundFX::menuSelect();

	FourByFour_UI* ui = new FourByFour_UI();
	FourByFour_Board* board = new FourByFour_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame8() {
	clearScreen();
	SoundFX::menuSelect();

	Pyramid_UI* ui = new Pyramid_UI();
	Pyramid_Board* board = new Pyramid_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame9() {
	clearScreen();
	SoundFX::menuSelect();

	Numerical_UI* ui = new Numerical_UI();
	Numerical_Board* board = new Numerical_Board();
	Player<int>** players = ui->setup_players();

	GameManager<int> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame10() {
	clearScreen();
	SoundFX::menuSelect();

	Obstacles_UI* ui = new Obstacles_UI();
	Obstacles_Board* board = new Obstacles_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame11() {
	clearScreen();
	SoundFX::menuSelect();

	Infinity_UI* ui = new Infinity_UI();
	Infinity_Board* board = new Infinity_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame12() {
	clearScreen();
	SoundFX::menuSelect();

	Ultimate_UI* ui = new Ultimate_UI();
	Ultimate_Board* board = new Ultimate_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

void playGame13() {
	clearScreen();
	SoundFX::menuSelect();

	Memory_UI* ui = new Memory_UI();
	Memory_Board* board = new Memory_Board();
	Player<char>** players = ui->setup_players();

	GameManager<char> game(board, players, ui);
	game.run();

	// Reveal the board at game end
	if (board->game_is_over(players[0])) {
		ColorConsole::cyan();
		cout << "\n=== REVEALING HIDDEN BOARD ===\n";
		ColorConsole::reset();
		board->revealBoard();
		ui->display_board_matrix(board->get_board_matrix());
	}

	delete board;
	for (int i = 0; i < 2; i++) delete players[i];
	delete[] players;
	delete ui;

	pauseScreen();
}

// ======================== MAIN FUNCTION ========================
int main() {
	srand(static_cast<unsigned int>(time(0)));

	// Startup sound
	SoundFX::startup();

	int choice;
	bool running = true;

	while (running) {
		displayMenu();
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			SoundFX::invalidMove();
			continue;
		}

		switch (choice) {
		case 0: playGame0(); break;
		case 1: playGame1(); break;
		case 2: playGame2(); break;
		case 3: playGame3(); break;
		case 4: playGame4(); break;
		case 5: playGame5(); break;
		case 6: playGame6(); break;
		case 7: playGame7(); break;
		case 8: playGame8(); break;
		case 9: playGame9(); break;
		case 10: playGame10(); break;
		case 11: playGame11(); break;
		case 12: playGame12(); break;
		case 13: playGame13(); break;
		case 99:
			clearScreen();
			ColorConsole::cyan();
			cout << "\n====================================================\n";
			cout << "     Thank you for playing!\n";
			cout << "     CS213 - Board Games Collection 2025\n";
			cout << "====================================================\n\n";
			ColorConsole::reset();
			SoundFX::draw();
			running = false;
			break;
		default:
			SoundFX::invalidMove();
			ColorConsole::red();
			cout << "\nInvalid choice! Please try again.\n";
			ColorConsole::reset();
			Sleep(1500);
		}
	}

	return 0;
}