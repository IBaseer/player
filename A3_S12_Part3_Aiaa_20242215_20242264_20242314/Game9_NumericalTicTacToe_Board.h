/**
 * @file Game9_NumericalTicTacToe_Board.h
 * @brief Numerical Tic-Tac-Toe Game - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 3x3 grid
 * - Player 1 uses odd numbers (1, 3, 5, 7, 9)
 * - Player 2 uses even numbers (2, 4, 6, 8)
 * - Each number can only be used once
 * - Win by making a line that sums to exactly 15
 */

#ifndef GAME9_NUMERICALTICTACTOE_BOARD_H
#define GAME9_NUMERICALTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <set>
#include <vector>

using namespace std;

// ======================== NUMERICAL TIC-TAC-TOE BOARD ========================
class Numerical_Board : public Board<int> {
private:
	int blank_symbol = 0;
	set<int> used_numbers;

	bool checkSum15(int a, int b, int c) {
		return (a != blank_symbol && b != blank_symbol && c != blank_symbol &&
			a + b + c == 15);
	}

public:
	Numerical_Board() : Board(3, 3) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
	}

	bool update_board(Move<int>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		int num = move->get_symbol();

		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		// Check if number is valid (1-9) and not used
		if (num < 1 || num > 9)
			return false;

		if (used_numbers.find(num) != used_numbers.end())
			return false;

		board[x][y] = num;
		used_numbers.insert(num);
		n_moves++;
		return true;
	}

	bool is_win(Player<int>* player) override {
		// Check rows
		for (int i = 0; i < rows; i++) {
			if (checkSum15(board[i][0], board[i][1], board[i][2]))
				return true;
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			if (checkSum15(board[0][j], board[1][j], board[2][j]))
				return true;
		}

		// Check diagonals
		if (checkSum15(board[0][0], board[1][1], board[2][2]))
			return true;
		if (checkSum15(board[0][2], board[1][1], board[2][0]))
			return true;

		return false;
	}

	bool is_lose(Player<int>* player) override {
		return false;
	}

	bool is_draw(Player<int>* player) override {
		return (n_moves == 9 && !is_win(player));
	}

	bool game_is_over(Player<int>* player) override {
		return is_win(player) || is_draw(player);
	}

	bool isNumberUsed(int num) {
		return used_numbers.find(num) != used_numbers.end();
	}

	vector<pair<int, int>> getValidPositions() const {
		vector<pair<int, int>> pos;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol) {
					pos.push_back({ i, j });
				}
			}
		}
		return pos;
	}
};

// ======================== NUMERICAL TIC-TAC-TOE UI ========================
class Numerical_UI : public UI<int> {
public:
	Numerical_UI() : UI<int>("=== Numerical Tic-Tac-Toe ===\nSum to 15 to win!", 3) {}

	Player<int>* create_player(string& name, int symbol, PlayerType type) override {
		return new Player<int>(name, symbol, type);
	}

	Move<int>* get_move(Player<int>* player) override {
		Numerical_Board* board = dynamic_cast<Numerical_Board*>(player->get_board_ptr());
		int x, y, num;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn\n";
			if (player->get_symbol() == 1) {
				cout << "Your numbers: 1, 3, 5, 7, 9 (odd)\n";
			}
			else {
				cout << "Your numbers: 2, 4, 6, 8 (even)\n";
			}
			cout << "Enter row (0-2), column (0-2), and number: ";
			cin >> x >> y >> num;

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

			// Validate odd/even
			if (player->get_symbol() == 1 && num % 2 == 0) {
				cout << "❌ Player 1 must use ODD numbers only!\n";
				return get_move(player);
			}
			if (player->get_symbol() == 2 && num % 2 == 1) {
				cout << "❌ Player 2 must use EVEN numbers only!\n";
				return get_move(player);
			}

			if (num < 1 || num > 9) {
				cout << "❌ Number must be between 1 and 9!\n";
				return get_move(player);
			}

			if (board && board->isNumberUsed(num)) {
				cout << "❌ Number " << num << " has already been used!\n";
				return get_move(player);
			}

			if (board && board->get_board_matrix()[x][y] != 0) {
				cout << "❌ Position (" << x << ", " << y << ") is already occupied!\n";
				return get_move(player);
			}
		}
		else {
			// SMART COMPUTER
			if (board) {
				auto valid_pos = board->getValidPositions();
				if (!valid_pos.empty()) {
					int idx = rand() % valid_pos.size();
					x = valid_pos[idx].first;
					y = valid_pos[idx].second;
				}
				else {
					x = y = 0;
				}

				// Choose valid number (odd or even)
				if (player->get_symbol() == 1) {
					// Odd numbers
					int odds[] = { 1, 3, 5, 7, 9 };
					do {
						num = odds[rand() % 5];
					} while (board->isNumberUsed(num));
				}
				else {
					// Even numbers
					int evens[] = { 2, 4, 6, 8 };
					do {
						num = evens[rand() % 4];
					} while (board->isNumberUsed(num));
				}
			}
			else {
				x = rand() % 3;
				y = rand() % 3;
				num = (player->get_symbol() == 1) ? (1 + 2 * (rand() % 5)) : (2 + 2 * (rand() % 4));
			}
			cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ", " << num << ")\n";
		}

		return new Move<int>(x, y, num);
	}

	Player<int>** setup_players() override {
		Player<int>** players = new Player<int>*[2];

		string name1 = "Player 1";
		cout << "Enter Player 1 name (odd numbers): ";
		getline(cin >> ws, name1);

		cout << "Choose Player 1 type:\n1. Human\n2. Computer\nChoice: ";
		int choice1;
		cin >> choice1;
		PlayerType type1 = (choice1 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[0] = create_player(name1, 1, type1); // 1 represents odd player

		string name2 = "Player 2";
		cout << "Enter Player 2 name (even numbers): ";
		getline(cin >> ws, name2);

		cout << "Choose Player 2 type:\n1. Human\n2. Computer\nChoice: ";
		int choice2;
		cin >> choice2;
		PlayerType type2 = (choice2 == 2) ? PlayerType::COMPUTER : PlayerType::HUMAN;

		players[1] = create_player(name2, 2, type2); // 2 represents even player

		return players;
	}
};

#endif // GAME9_NUMERICALTICTACTOE_BOARD_H