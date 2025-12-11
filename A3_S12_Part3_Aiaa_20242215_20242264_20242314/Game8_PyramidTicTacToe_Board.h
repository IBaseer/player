/**
 * @file Game8_PyramidTicTacToe_Board.h
 * @brief Pyramid Tic-Tac-Toe Game - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - Pyramid-shaped board: 1 cell at top, 3 in middle, 5 at bottom
 * - First to align 3 marks (horizontal, vertical, diagonal) wins
 * - 9 total positions in pyramid
 */

#ifndef GAME8_PYRAMIDTICTACTOE_BOARD_H
#define GAME8_PYRAMIDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <vector>

using namespace std;

// ======================== PYRAMID TIC-TAC-TOE BOARD ========================
class Pyramid_Board : public Board<char> {
private:
	char blank_symbol = '.';

	bool isValidPosition(int x, int y) {
		// Pyramid shape:
		// Row 0: position (0,2) only
		// Row 1: positions (1,1), (1,2), (1,3)
		// Row 2: positions (2,0), (2,1), (2,2), (2,3), (2,4)
		if (x == 0) return y == 2;
		if (x == 1) return (y >= 1 && y <= 3);
		if (x == 2) return (y >= 0 && y <= 4);
		return false;
	}

	bool checkLine(int x, int y, int dx, int dy, char symbol) {
		for (int i = 0; i < 3; i++) {
			int nx = x + i * dx;
			int ny = y + i * dy;
			if (nx < 0 || nx >= rows || ny < 0 || ny >= columns)
				return false;
			if (!isValidPosition(nx, ny))
				return false;
			if (board[nx][ny] != symbol)
				return false;
		}
		return true;
	}

public:
	Pyramid_Board() : Board(3, 5) {
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

		// Check all possible 3-in-a-row from each position
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == sym && isValidPosition(i, j)) {
					// Check horizontal
					if (checkLine(i, j, 0, 1, sym))
						return true;
					// Check vertical
					if (checkLine(i, j, 1, 0, sym))
						return true;
					// Check diagonals
					if (checkLine(i, j, 1, 1, sym))
						return true;
					if (checkLine(i, j, 1, -1, sym))
						return true;
				}
			}
		}
		return false;
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		return (n_moves == 9 && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
	}

	vector<pair<int, int>> getValidMoves() const {
		vector<pair<int, int>> moves;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol) {
					bool valid = (i == 0 && j == 2) ||
						(i == 1 && j >= 1 && j <= 3) ||
						(i == 2 && j >= 0 && j <= 4);
					if (valid) {
						moves.push_back({ i, j });
					}
				}
			}
		}
		return moves;
	}
};

// ======================== PYRAMID TIC-TAC-TOE UI ========================
class Pyramid_UI : public UI<char> {
public:
	Pyramid_UI() : UI<char>("=== Pyramid Tic-Tac-Toe ===\nGet 3-in-a-row on pyramid!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Pyramid_Board* board = dynamic_cast<Pyramid_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Pyramid layout:\n";
			cout << "  Row 0: (0,2)\n";
			cout << "  Row 1: (1,1) (1,2) (1,3)\n";
			cout << "  Row 2: (2,0) (2,1) (2,2) (2,3) (2,4)\n";
			cout << "Enter row and column: ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 3 || y < 0 || y >= 5) {
				cout << "❌ Position out of bounds!\n";
				return get_move(player);
			}

			bool valid = (x == 0 && y == 2) ||
				(x == 1 && y >= 1 && y <= 3) ||
				(x == 2 && y >= 0 && y <= 4);

			if (!valid) {
				cout << "❌ Position (" << x << ", " << y << ") is not in pyramid!\n";
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
					x = 1; y = 2; // Fallback to center
				}
			}
			else {
				// Fallback random valid position
				x = 1; y = 2;
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

#endif // GAME8_PYRAMIDTICTACTOE_BOARD_H