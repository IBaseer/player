/**
 * @file Game11_InfinityTicTacToe_Board.h
 * @brief Infinity Tic-Tac-Toe Game - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 3x3 grid
 * - After every 3 moves, the oldest mark disappears
 * - First to align 3 marks before they vanish wins
 */

#ifndef GAME11_INFINITYTICTACTOE_BOARD_H
#define GAME11_INFINITYTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <queue>
#include <iostream>
#include <vector>

using namespace std;

// ======================== INFINITY TIC-TAC-TOE BOARD ========================
class Infinity_Board : public Board<char> {
private:
	char blank_symbol = '.';
	queue<pair<int, int>> move_history; // Track order of moves
	int total_moves_made = 0;
	int max_moves_limit = 50; // Prevent infinite games

	bool hasThreeInRow(char symbol) {
		// Check rows
		for (int i = 0; i < rows; i++) {
			if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
				return true;
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol)
				return true;
		}

		// Check diagonals
		if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
			return true;
		if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
			return true;

		return false;
	}

public:
	Infinity_Board() : Board(3, 3) {
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
		move_history.push({ x, y });
		total_moves_made++;
		n_moves++;

		// Remove oldest move if we have more than 3 moves on board
		if (move_history.size() > 3) {
			auto oldest = move_history.front();
			move_history.pop();
			board[oldest.first][oldest.second] = blank_symbol;
			n_moves--;
		}

		return true;
	}

	bool is_win(Player<char>* player) override {
		return hasThreeInRow(player->get_symbol());
	}

	bool is_lose(Player<char>* player) override {
		return false;
	}

	bool is_draw(Player<char>* player) override {
		// Game is draw if max moves reached without winner
		return (total_moves_made >= max_moves_limit && !is_win(player));
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || (total_moves_made >= max_moves_limit);
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

// ======================== INFINITY TIC-TAC-TOE UI ========================
class Infinity_UI : public UI<char> {
public:
	Infinity_UI() : UI<char>("=== Infinity Tic-Tac-Toe ===\nMarks disappear after 3 moves!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Infinity_Board* board = dynamic_cast<Infinity_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
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

#endif // GAME11_INFINITYTICTACTOE_BOARD_H