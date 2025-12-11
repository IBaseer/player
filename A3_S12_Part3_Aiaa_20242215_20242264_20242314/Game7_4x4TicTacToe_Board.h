/**
 * @file Game7_4x4TicTacToe_Board.h
 * @brief 4x4 Tic-Tac-Toe with Moving Tokens - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 4x4 grid
 * - Each player places 4 tokens first
 * - Then players can move tokens to adjacent empty squares
 * - First to align 3 tokens in a row wins
 */

#ifndef GAME7_4X4TICTACTOE_BOARD_H
#define GAME7_4X4TICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <iostream>
#include <vector>

using namespace std;

// ======================== 4X4 TIC-TAC-TOE BOARD ========================
class FourByFour_Board : public Board<char> {
private:
	char blank_symbol = '.';

	bool hasThreeInRow(char symbol) {
		// Check rows
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j <= columns - 3; j++) {
				if (board[i][j] == symbol &&
					board[i][j + 1] == symbol &&
					board[i][j + 2] == symbol)
					return true;
			}
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			for (int i = 0; i <= rows - 3; i++) {
				if (board[i][j] == symbol &&
					board[i + 1][j] == symbol &&
					board[i + 2][j] == symbol)
					return true;
			}
		}

		// Check diagonals
		for (int i = 0; i <= rows - 3; i++) {
			for (int j = 0; j <= columns - 3; j++) {
				if (board[i][j] == symbol &&
					board[i + 1][j + 1] == symbol &&
					board[i + 2][j + 2] == symbol)
					return true;
				if (board[i][j + 2] == symbol &&
					board[i + 1][j + 1] == symbol &&
					board[i + 2][j] == symbol)
					return true;
			}
		}

		return false;
	}

public:
	FourByFour_Board() : Board(4, 4) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
	}

	bool update_board(Move<char>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		char mark = toupper(move->get_symbol());

		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		board[x][y] = mark;
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		return hasThreeInRow(player->get_symbol());
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		return (n_moves == 16 && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || (n_moves == 16);
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

// ======================== 4X4 TIC-TAC-TOE UI ========================
class FourByFour_UI : public UI<char> {
public:
	FourByFour_UI() : UI<char>("=== 4x4 Tic-Tac-Toe ===\nAlign 3 tokens to win!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		FourByFour_Board* board = dynamic_cast<FourByFour_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Enter row and column (0-3): ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 4 || y < 0 || y >= 4) {
				cout << "❌ Position out of bounds! Enter 0-3.\n";
				return get_move(player);
			}

			if (board && board->get_board_matrix()[x][y] != '.') {
				cout << "❌ Position (" << x << ", " << y << ") is already occupied!\n";
				return get_move(player);
			}
		}
		else {
			// SMART COMPUTER
			if (board) {
				auto valid_moves = board->getValidMoves();
				if (!valid_moves.empty()) {
					int idx = rand() % valid_moves.size();
					x = valid_moves[idx].first;
					y = valid_moves[idx].second;
				}
				else {
					x = y = 0;
				}
			}
			else {
				x = rand() % 4;
				y = rand() % 4;
			}
			cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ")\n";
		}

		return new Move<char>(x, y, player->get_symbol());
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

#endif // GAME7_4X4TICTACTOE_BOARD_H