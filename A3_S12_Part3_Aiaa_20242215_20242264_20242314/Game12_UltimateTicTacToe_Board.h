/**
 * @file Game12_UltimateTicTacToe_Board.h
 * @brief Ultimate Tic Tac Toe Game (BONUS) - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 3x3 main grid, each cell contains a 3x3 sub-grid
 * - Player 1 starts on any sub-board
 * - Your move determines which sub-board opponent plays next
 * - Win 3 sub-boards in a row to win the game
 */

#ifndef GAME12_ULTIMATETICTACTOE_BOARD_H
#define GAME12_ULTIMATETICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <vector>

using namespace std;

// ======================== ULTIMATE TIC-TAC-TOE BOARD ========================
class Ultimate_Board : public Board<char> {
private:
	char blank_symbol = '.';
	char sub_board_winners[3][3]; // Tracks winner of each sub-board
	int next_board_x = -1; // -1 means any board
	int next_board_y = -1;

	bool checkSubBoardWin(int board_x, int board_y, char symbol) {
		int start_x = board_x * 3;
		int start_y = board_y * 3;

		// Check rows in sub-board
		for (int i = 0; i < 3; i++) {
			if (board[start_x + i][start_y] == symbol &&
				board[start_x + i][start_y + 1] == symbol &&
				board[start_x + i][start_y + 2] == symbol)
				return true;
		}

		// Check columns in sub-board
		for (int j = 0; j < 3; j++) {
			if (board[start_x][start_y + j] == symbol &&
				board[start_x + 1][start_y + j] == symbol &&
				board[start_x + 2][start_y + j] == symbol)
				return true;
		}

		// Check diagonals
		if (board[start_x][start_y] == symbol &&
			board[start_x + 1][start_y + 1] == symbol &&
			board[start_x + 2][start_y + 2] == symbol)
			return true;

		if (board[start_x][start_y + 2] == symbol &&
			board[start_x + 1][start_y + 1] == symbol &&
			board[start_x + 2][start_y] == symbol)
			return true;

		return false;
	}

	bool checkMainBoardWin(char symbol) {
		// Check rows
		for (int i = 0; i < 3; i++) {
			if (sub_board_winners[i][0] == symbol &&
				sub_board_winners[i][1] == symbol &&
				sub_board_winners[i][2] == symbol)
				return true;
		}

		// Check columns
		for (int j = 0; j < 3; j++) {
			if (sub_board_winners[0][j] == symbol &&
				sub_board_winners[1][j] == symbol &&
				sub_board_winners[2][j] == symbol)
				return true;
		}

		// Check diagonals
		if (sub_board_winners[0][0] == symbol &&
			sub_board_winners[1][1] == symbol &&
			sub_board_winners[2][2] == symbol)
			return true;

		if (sub_board_winners[0][2] == symbol &&
			sub_board_winners[1][1] == symbol &&
			sub_board_winners[2][0] == symbol)
			return true;

		return false;
	}

public:
	Ultimate_Board() : Board(9, 9) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				sub_board_winners[i][j] = blank_symbol;
	}

	bool update_board(Move<char>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		char mark = toupper(move->get_symbol());

		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		// Check if move is in correct sub-board
		int board_x = x / 3;
		int board_y = y / 3;

		if (next_board_x != -1) {
			if (board_x != next_board_x || board_y != next_board_y)
				return false; // Must play in specified sub-board
		}

		board[x][y] = mark;
		n_moves++;

		// Check if this wins the sub-board
		if (checkSubBoardWin(board_x, board_y, mark)) {
			sub_board_winners[board_x][board_y] = mark;
		}

		// Set next board based on position within sub-board
		next_board_x = x % 3;
		next_board_y = y % 3;

		// If next board is already won, allow any board
		if (sub_board_winners[next_board_x][next_board_y] != blank_symbol) {
			next_board_x = -1;
			next_board_y = -1;
		}

		return true;
	}

	bool is_win(Player<char>* player) override {
		return checkMainBoardWin(player->get_symbol());
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		return (n_moves >= 81 && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
	}

	int getNextBoardX() const { return next_board_x; }
	int getNextBoardY() const { return next_board_y; }

	vector<pair<int, int>> getValidMoves(int required_board_x = -1, int required_board_y = -1) const {
		vector<pair<int, int>> moves;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol) {
					int bx = i / 3;
					int by = j / 3;
					if (required_board_x == -1 || (bx == required_board_x && by == required_board_y)) {
						moves.push_back({ i, j });
					}
				}
			}
		}
		return moves;
	}
};

// ======================== ULTIMATE TIC-TAC-TOE UI ========================
class Ultimate_UI : public UI<char> {
public:
	Ultimate_UI() : UI<char>("=== Ultimate Tic Tac Toe (BONUS) ===\nWin 3 sub-boards!", 2) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Ultimate_Board* board = dynamic_cast<Ultimate_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";

			if (board && board->getNextBoardX() != -1) {
				cout << "You must play in sub-board (" << board->getNextBoardX()
					<< ", " << board->getNextBoardY() << ")\n";
			}

			cout << "Enter row and column (0-8): ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 9 || y < 0 || y >= 9) {
				cout << "❌ Position out of bounds! Enter 0-8.\n";
				return get_move(player);
			}

			if (board) {
				int bx = x / 3;
				int by = y / 3;
				if (board->getNextBoardX() != -1) {
					if (bx != board->getNextBoardX() || by != board->getNextBoardY()) {
						cout << "❌ Must play in required sub-board ("
							<< board->getNextBoardX() << ", " << board->getNextBoardY() << ")!\n";
						return get_move(player);
					}
				}

				if (board->get_board_matrix()[x][y] != '.') {
					cout << "❌ Position (" << x << ", " << y << ") is already occupied!\n";
					return get_move(player);
				}
			}
		}
		else {
			// SMART COMPUTER
			if (board) {
				auto valid_moves = board->getValidMoves(board->getNextBoardX(), board->getNextBoardY());
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
				x = rand() % 9;
				y = rand() % 9;
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

#endif // GAME12_ULTIMATETICTACTOE_BOARD_H