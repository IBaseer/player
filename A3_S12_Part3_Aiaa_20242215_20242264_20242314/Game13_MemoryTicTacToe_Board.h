/**
 * @file Game13_MemoryTicTacToe_Board.h
 * @brief Memory Tic-Tac-Toe Game (BONUS) - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 3x3 grid
 * - Marks are HIDDEN after being placed
 * - Players must remember positions
 * - First to align 3 hidden marks wins
 */

#ifndef GAME13_MEMORYTICTACTOE_BOARD_H
#define GAME13_MEMORYTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <vector>

using namespace std;

// ======================== MEMORY TIC-TAC-TOE BOARD ========================
class Memory_Board : public Board<char> {
private:
	char blank_symbol = '.';
	char hidden_symbol = '?';
	vector<vector<char>> hidden_board; // Actual board state

	bool hasThreeInRow(char symbol) {
		// Check rows
		for (int i = 0; i < rows; i++) {
			if (hidden_board[i][0] == symbol &&
				hidden_board[i][1] == symbol &&
				hidden_board[i][2] == symbol)
				return true;
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			if (hidden_board[0][j] == symbol &&
				hidden_board[1][j] == symbol &&
				hidden_board[2][j] == symbol)
				return true;
		}

		// Check diagonals
		if (hidden_board[0][0] == symbol &&
			hidden_board[1][1] == symbol &&
			hidden_board[2][2] == symbol)
			return true;
		if (hidden_board[0][2] == symbol &&
			hidden_board[1][1] == symbol &&
			hidden_board[2][0] == symbol)
			return true;

		return false;
	}

public:
	Memory_Board() : Board(3, 3) {
		hidden_board.resize(3, vector<char>(3, blank_symbol));
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

		if (hidden_board[x][y] != blank_symbol)
			return false;

		// Place mark in hidden board
		hidden_board[x][y] = mark;

		// Show as hidden in display board
		board[x][y] = hidden_symbol;

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
		return (n_moves == 9 && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
	}

	// Reveal the actual board (for debugging or game end)
	void revealBoard() {
		board = hidden_board;
	}

	vector<pair<int, int>> getValidMoves() const {
		vector<pair<int, int>> moves;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (hidden_board[i][j] == blank_symbol) {
					moves.push_back({ i, j });
				}
			}
		}
		return moves;
	}
};

// ======================== MEMORY TIC-TAC-TOE UI ========================
class Memory_UI : public UI<char> {
public:
	Memory_UI() : UI<char>("=== Memory Tic-Tac-Toe (BONUS) ===\nMarks are hidden!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Memory_Board* board = dynamic_cast<Memory_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Remember: marks are hidden after placement!\n";
			cout << "Enter row and column (0-2): ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 3 || y < 0 || y >= 3) {
				cout << "❌ Position out of bounds! Enter 0-2.\n";
				return get_move(player);
			}

			// Note: Can't check if occupied visually (it's hidden)
			// Will be caught by update_board if invalid
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
				x = rand() % 3;
				y = rand() % 3;
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

#endif // GAME13_MEMORYTICTACTOE_BOARD_H