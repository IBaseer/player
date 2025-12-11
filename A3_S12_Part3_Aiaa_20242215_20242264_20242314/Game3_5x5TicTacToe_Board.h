/**
 * @file Game3_5x5TicTacToe_Board.h
 * @brief 5x5 Tic Tac Toe - FIXED VERSION
 */

#ifndef GAME3_5X5TICTACTOE_BOARD_H
#define GAME3_5X5TICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <iostream>
#include <vector>

using namespace std;

// ======================== 5X5 TIC TAC TOE BOARD ========================
class FiveByFive_Board : public Board<char> {
private:
	char blank_symbol = '.';

	int countThreeInRow(char symbol) {
		int count = 0;

		// Check rows
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j <= columns - 3; j++) {
				if (board[i][j] == symbol &&
					board[i][j + 1] == symbol &&
					board[i][j + 2] == symbol) {
					count++;
				}
			}
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			for (int i = 0; i <= rows - 3; i++) {
				if (board[i][j] == symbol &&
					board[i + 1][j] == symbol &&
					board[i + 2][j] == symbol) {
					count++;
				}
			}
		}

		// Check diagonals (down-right)
		for (int i = 0; i <= rows - 3; i++) {
			for (int j = 0; j <= columns - 3; j++) {
				if (board[i][j] == symbol &&
					board[i + 1][j + 1] == symbol &&
					board[i + 2][j + 2] == symbol) {
					count++;
				}
			}
		}

		// Check diagonals (down-left)
		for (int i = 0; i <= rows - 3; i++) {
			for (int j = 2; j < columns; j++) {
				if (board[i][j] == symbol &&
					board[i + 1][j - 1] == symbol &&
					board[i + 2][j - 2] == symbol) {
					count++;
				}
			}
		}

		return count;
	}

public:
	FiveByFive_Board() : Board(5, 5) {
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
		return false;
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		return (n_moves == 24);
	}

	bool game_is_over(Player<char>* player) override {
		return (n_moves == 24);
	}

	int getPlayerScore(char symbol) {
		return countThreeInRow(symbol);
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

// ======================== 5X5 TIC TAC TOE UI ========================
class FiveByFive_UI : public UI<char> {
public:
	FiveByFive_UI() : UI<char>("=== 5x5 Tic Tac Toe ===\nMost 3-in-a-rows wins!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		FiveByFive_Board* board = dynamic_cast<FiveByFive_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Enter row and column (0-4): ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "? Invalid input!\n";
				return get_move(player);
			}

			if (x < 0 || x >= 5 || y < 0 || y >= 5) {
				cout << "? Position out of bounds! Enter 0-4.\n";
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
					int idx = rand() % valid_moves.size();
					x = valid_moves[idx].first;
					y = valid_moves[idx].second;
				}
				else {
					x = y = 0;
				}
			}
			else {
				x = rand() % 5;
				y = rand() % 5;
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

#endif // GAME3_5X5TICTACTOE_BOARD_H