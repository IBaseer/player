/**
 * @file Game2_FourInARow_Board.h
 * @brief Four-in-a-Row Game - FIXED VERSION
 */

#ifndef GAME2_FOURINAROW_BOARD_H
#define GAME2_FOURINAROW_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <vector>

using namespace std;

// ======================== FOUR-IN-A-ROW BOARD ========================
class FourInARow_Board : public Board<char> {
private:
	char blank_symbol = '.';

	bool checkFour(int x, int y, int dx, int dy, char symbol) {
		for (int i = 0; i < 4; i++) {
			int nx = x + i * dx;
			int ny = y + i * dy;
			if (nx < 0 || nx >= rows || ny < 0 || ny >= columns)
				return false;
			if (board[nx][ny] != symbol)
				return false;
		}
		return true;
	}

public:
	FourInARow_Board() : Board(6, 7) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
	}

	bool update_board(Move<char>* move) override {
		int col = move->get_y();
		char mark = toupper(move->get_symbol());

		if (col < 0 || col >= columns)
			return false;

		// Find lowest empty row
		int row = -1;
		for (int i = rows - 1; i >= 0; i--) {
			if (board[i][col] == blank_symbol) {
				row = i;
				break;
			}
		}

		if (row == -1)
			return false;

		board[row][col] = mark;
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		char sym = player->get_symbol();

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == sym) {
					if (checkFour(i, j, 0, 1, sym) ||
						checkFour(i, j, 1, 0, sym) ||
						checkFour(i, j, 1, 1, sym) ||
						checkFour(i, j, 1, -1, sym))
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
		return (n_moves == rows * columns && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
	}

	// Get valid columns (not full)
	vector<int> getValidColumns() const {
		vector<int> cols;
		for (int j = 0; j < columns; j++) {
			if (board[0][j] == blank_symbol) {
				cols.push_back(j);
			}
		}
		return cols;
	}
};

// ======================== FOUR-IN-A-ROW UI ========================
class FourInARow_UI : public UI<char> {
public:
	FourInARow_UI() : UI<char>("=== Four-in-a-Row (Connect 4) ===", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		FourInARow_Board* board = dynamic_cast<FourInARow_Board*>(player->get_board_ptr());
		int col;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Enter column (0-6): ";
			cin >> col;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "? Invalid input!\n";
				return get_move(player);
			}

			if (col < 0 || col >= 7) {
				cout << "? Column out of bounds! Enter 0-6.\n";
				return get_move(player);
			}

			if (board && board->get_board_matrix()[0][col] != '.') {
				cout << "? Column " << col << " is full! Choose another.\n";
				return get_move(player);
			}
		}
		else {
			// SMART COMPUTER: Only valid columns
			if (board) {
				auto valid_cols = board->getValidColumns();
				if (!valid_cols.empty()) {
					col = valid_cols[rand() % valid_cols.size()];
				}
				else {
					col = 0;
				}
			}
			else {
				col = rand() % 7;
			}
			cout << "\n" << player->get_name() << " plays column: " << col << "\n";
		}

		return new Move<char>(0, col, player->get_symbol());
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

#endif // GAME2_FOURINAROW_BOARD_H