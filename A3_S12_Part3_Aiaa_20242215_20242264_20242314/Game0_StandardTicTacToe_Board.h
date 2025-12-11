/**
 * @file Game0_StandardTicTacToe_Board.h
 * @brief Standard (Classic) Tic-Tac-Toe Game - FIXED VERSION
 *
 * FIXES:
 * - Computer player shows name instead of just "Computer"
 * - Smart computer move selection (only valid positions)
 * - Informative error messages for invalid human moves
 */

#ifndef GAME0_STANDARDTICTACTOE_BOARD_H
#define GAME0_STANDARDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <iostream>
#include <vector>

using namespace std;

// ======================== STANDARD TIC-TAC-TOE BOARD ========================
class StandardTicTacToe_Board : public Board<char> {
private:
	char blank_symbol = '.';

public:
	StandardTicTacToe_Board() : Board(3, 3) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
	}

	bool update_board(Move<char>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		char mark = move->get_symbol();

		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		board[x][y] = toupper(mark);
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		char sym = player->get_symbol();

		// Check rows
		for (int i = 0; i < rows; i++) {
			if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
				return true;
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			if (board[0][j] == sym && board[1][j] == sym && board[2][j] == sym)
				return true;
		}

		// Check diagonals
		if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
			return true;
		if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
			return true;

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

	// Helper: Get all valid empty positions
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

// ======================== STANDARD TIC-TAC-TOE UI ========================
class StandardTicTacToe_UI : public UI<char> {
public:
	StandardTicTacToe_UI() : UI<char>("=== Standard Tic-Tac-Toe (Classic 3x3) ===", 3) {}

	~StandardTicTacToe_UI() {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		StandardTicTacToe_Board* board = dynamic_cast<StandardTicTacToe_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Enter row and column (0-2): ";
			cin >> x >> y;

			// Validate input
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers only.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 3 || y < 0 || y >= 3) {
				cout << "❌ Position out of bounds! Please enter row and column between 0-2.\n";
				return get_move(player);
			}

			// Check if position is already taken
			if (board && board->get_board_matrix()[x][y] != '.') {
				cout << "❌ Position (" << x << ", " << y << ") is already occupied! Choose an empty cell.\n";
				return get_move(player);
			}
		}
		else {
			// SMART COMPUTER: Only choose from valid positions
			if (board) {
				auto valid_moves = board->getValidMoves();
				if (!valid_moves.empty()) {
					int random_idx = rand() % valid_moves.size();
					x = valid_moves[random_idx].first;
					y = valid_moves[random_idx].second;
					cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ")\n";
				}
				else {
					x = 0; y = 0; // Fallback (shouldn't happen)
				}
			}
			else {
				x = rand() % 3;
				y = rand() % 3;
				cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ")\n";
			}
		}

		return new Move<char>(x, y, player->get_symbol());
	}

	Player<char>** setup_players() override {
		Player<char>** players = new Player<char>*[2];

		string nameX = "Player X";
		cout << "Enter Player X name: ";
		getline(cin >> ws, nameX);

		cout << "Choose Player X type:\n1. Human\n2. Computer\nChoice: ";
		int choiceX;
		cin >> choiceX;
		PlayerType typeX = (choiceX == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[0] = create_player(nameX, 'X', typeX);

		string nameO = "Player O";
		cout << "Enter Player O name: ";
		getline(cin >> ws, nameO);

		cout << "Choose Player O type:\n1. Human\n2. Computer\nChoice: ";
		int choiceO;
		cin >> choiceO;
		PlayerType typeO = (choiceO == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[1] = create_player(nameO, 'O', typeO);

		return players;
	}
};

#endif // GAME0_STANDARDTICTACTOE_BOARD_H