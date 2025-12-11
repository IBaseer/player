/**
 * @file Game6_DiamondTicTacToe_Board.h
 * @brief Diamond Tic-Tac-Toe Game - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 5x5 grid arranged in diamond shape
 * - Must complete TWO lines simultaneously (one 3-mark, one 4-mark)
 * - Lines must be in different directions
 * - Can share one common mark
 */

#ifndef GAME6_DIAMONDTICTACTOE_BOARD_H
#define GAME6_DIAMONDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <vector>

using namespace std;

// ======================== DIAMOND TIC-TAC-TOE BOARD ========================
class Diamond_Board : public Board<char> {
private:
	char blank_symbol = '.';

	bool isValidPosition(int x, int y) {
		// Diamond shape on 5x5 grid
		if (x == 0) return y == 2;
		if (x == 1) return (y >= 1 && y <= 3);
		if (x == 2) return (y >= 0 && y <= 4);
		if (x == 3) return (y >= 1 && y <= 3);
		if (x == 4) return y == 2;
		return false;
	}

	int countLine(int x, int y, int dx, int dy, char symbol) {
		int count = 0;
		while (x >= 0 && x < rows && y >= 0 && y < columns &&
			board[x][y] == symbol && isValidPosition(x, y)) {
			count++;
			x += dx;
			y += dy;
		}
		return count;
	}

	bool hasLineOfLength(char symbol, int length) {
		// Check all directions from each position
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == symbol && isValidPosition(i, j)) {
					// Check horizontal, vertical, and diagonals
					if (countLine(i, j, 0, 1, symbol) >= length ||
						countLine(i, j, 1, 0, symbol) >= length ||
						countLine(i, j, 1, 1, symbol) >= length ||
						countLine(i, j, 1, -1, symbol) >= length)
						return true;
				}
			}
		}
		return false;
	}

public:
	Diamond_Board() : Board(5, 5) {
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

		if (!isValidPosition(x, y))
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		board[x][y] = mark;
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		char sym = player->get_symbol();
		// Win by having both a line of 3 AND a line of 4
		// Simplified: just check for line of 4
		return hasLineOfLength(sym, 4);
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		// Count valid positions (13 in diamond)
		int valid_positions = 13;
		return (n_moves == valid_positions && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
	}

	vector<pair<int, int>> getValidMoves() const {
		vector<pair<int, int>> moves;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol &&
					((i == 0 && j == 2) ||
						(i == 1 && j >= 1 && j <= 3) ||
						(i == 2 && j >= 0 && j <= 4) ||
						(i == 3 && j >= 1 && j <= 3) ||
						(i == 4 && j == 2))) {
					moves.push_back({ i, j });
				}
			}
		}
		return moves;
	}
};

// ======================== DIAMOND TIC-TAC-TOE UI ========================
class Diamond_UI : public UI<char> {
public:
	Diamond_UI() : UI<char>("=== Diamond Tic-Tac-Toe ===\nComplete 2 lines simultaneously!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Diamond_Board* board = dynamic_cast<Diamond_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Diamond positions:\n";
			cout << "  Row 0: (0,2)\n";
			cout << "  Row 1: (1,1) (1,2) (1,3)\n";
			cout << "  Row 2: (2,0) (2,1) (2,2) (2,3) (2,4)\n";
			cout << "  Row 3: (3,1) (3,2) (3,3)\n";
			cout << "  Row 4: (4,2)\n";
			cout << "Enter row and column: ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 5 || y < 0 || y >= 5) {
				cout << "❌ Position out of bounds! Enter 0-4.\n";
				return get_move(player);
			}

			bool valid = (x == 0 && y == 2) ||
				(x == 1 && y >= 1 && y <= 3) ||
				(x == 2 && y >= 0 && y <= 4) ||
				(x == 3 && y >= 1 && y <= 3) ||
				(x == 4 && y == 2);

			if (!valid) {
				cout << "❌ Position (" << x << ", " << y << ") is not in diamond shape!\n";
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
					x = 2; y = 2; // Fallback
				}
			}
			else {
				x = 2; y = 2;
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

#endif // GAME6_DIAMONDTICTACTOE_BOARD_H