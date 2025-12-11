/**
 * @file Game1_SUS_Board.h
 * @brief SUS Game - Board and UI Implementation - FIXED VERSION
 *
 * Game Rules:
 * - 3x3 grid where players place 'S' or 'U'
 * - Score points for creating "S-U-S" sequences
 * - Player with most SUS sequences wins when board is full
 */

#ifndef GAME1_SUS_BOARD_H
#define GAME1_SUS_BOARD_H

#include "BoardGame_Classes.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

using namespace std;

// ======================== SUS BOARD ========================
class SUS_Board : public Board<char> {
private:
	char blank_symbol = '.';

	// Count all SUS sequences on the board
	int countAllSUS() {
		int count = 0;

		// Check all horizontal sequences
		for (int i = 0; i < rows; i++) {
			if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S')
				count++;
		}

		// Check all vertical sequences
		for (int j = 0; j < columns; j++) {
			if (board[0][j] == 'S' && board[1][j] == 'U' && board[2][j] == 'S')
				count++;
		}

		// Check diagonals
		if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S')
			count++;
		if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S')
			count++;

		return count;
	}

public:
	SUS_Board() : Board(3, 3) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
	}

	bool update_board(Move<char>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		char mark = toupper(move->get_symbol());

		// Validate position
		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		// Check if cell is empty
		if (board[x][y] != blank_symbol)
			return false;

		// Only allow S or U
		if (mark != 'S' && mark != 'U')
			return false;

		board[x][y] = mark;
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		// No winner during game, only at end
		return false;
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		if (n_moves == 9) {
			int sus_count = countAllSUS();
			// If there are SUS sequences, someone wins (not a draw)
			// This will be handled by displaying scores
			return true; // Game ends
		}
		return false;
	}

	bool game_is_over(Player<char>* player) override {
		return (n_moves == 9);
	}

	int getTotalSUS() {
		return countAllSUS();
	}

	vector<pair<int, int>> getValidMoves() const {
		vector<pair<int, int>> moves;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol) {
					moves.push_back({ i, j });
				}
			}
		}
		return moves;
	}
};

// ======================== SUS UI ========================
class SUS_UI : public UI<char> {
public:
	SUS_UI() : UI<char>("=== SUS Game ===\nCreate S-U-S sequences to score!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		SUS_Board* board = dynamic_cast<SUS_Board*>(player->get_board_ptr());
		int x, y;
		char letter;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn\n";
			cout << "Enter row (0-2), column (0-2), and letter (S or U): ";
			cin >> x >> y >> letter;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "? Invalid input!\n";
				return get_move(player);
			}

			letter = toupper(letter);
			if (letter != 'S' && letter != 'U') {
				cout << "? Invalid letter! Only S or U allowed.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 3 || y < 0 || y >= 3) {
				cout << "? Position out of bounds! Enter 0-2.\n";
				return get_move(player);
			}

			if (board && board->get_board_matrix()[x][y] != '.') {
				cout << "? Position (" << x << ", " << y << ") is occupied!\n";
				return get_move(player);
			}
		}
		else {
			// SMART COMPUTER
			if (board) {
				auto valid_moves = board->getValidMoves();
				if (!valid_moves.empty()) {
					int random_idx = rand() % valid_moves.size();
					x = valid_moves[random_idx].first;
					y = valid_moves[random_idx].second;
				}
				else {
					x = y = 0;
				}
			}
			else {
				x = rand() % 3;
				y = rand() % 3;
			}
			letter = (rand() % 2 == 0) ? 'S' : 'U';
			cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ", " << letter << ")\n";
		}

		return new Move<char>(x, y, toupper(letter));
	}

	Player<char>** setup_players() override {
		Player<char>** players = new Player<char>*[2];

		string name1 = "Player 1";
		cout << "Enter Player 1 name: ";
		getline(cin >> ws, name1);

		cout << "Choose Player 1 type:\n1. Human\n2. Computer\nChoice: ";
		int choice1;
		cin >> choice1;
		PlayerType type1 = (choice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[0] = create_player(name1, 'X', type1);

		string name2 = "Player 2";
		cout << "Enter Player 2 name: ";
		getline(cin >> ws, name2);

		cout << "Choose Player 2 type:\n1. Human\n2. Computer\nChoice: ";
		int choice2;
		cin >> choice2;
		PlayerType type2 = (choice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[1] = create_player(name2, 'O', type2);

		return players;
	}
};

#endif // GAME1_SUS_BOARD_H