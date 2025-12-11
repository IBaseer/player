/**
 * @file Game10_ObstaclesTicTacToe_Board.h
 * @brief Obstacles Tic-Tac-Toe Game - COMPLETE FIXED VERSION
 *
 * Game Rules:
 * - 6x6 grid
 * - After each round (2 moves), add 2 random obstacles
 * - Obstacles block cells from being used
 * - First to get 4-in-a-row wins
 */

#ifndef GAME10_OBSTACLESTICTACTOE_BOARD_H
#define GAME10_OBSTACLESTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <ctime>
#include <vector>

using namespace std;

// ======================== OBSTACLES TIC-TAC-TOE BOARD ========================
class Obstacles_Board : public Board<char> {
private:
	char blank_symbol = '.';
	char obstacle_symbol = '#';
	int moves_since_obstacle = 0;

	void addRandomObstacles() {
		int obstacles_added = 0;
		int attempts = 0;
		while (obstacles_added < 2 && attempts < 50) {
			int x = rand() % rows;
			int y = rand() % columns;
			if (board[x][y] == blank_symbol) {
				board[x][y] = obstacle_symbol;
				obstacles_added++;
			}
			attempts++;
		}
	}

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
	Obstacles_Board() : Board(6, 6) {
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
		moves_since_obstacle++;

		// Add obstacles after every 2 moves (1 round)
		if (moves_since_obstacle == 2) {
			addRandomObstacles();
			moves_since_obstacle = 0;
		}

		return true;
	}

	bool is_win(Player<char>* player) override {
		char sym = player->get_symbol();

		// Check all positions for 4-in-a-row
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == sym) {
					if (checkFour(i, j, 0, 1, sym) ||  // Horizontal
						checkFour(i, j, 1, 0, sym) ||  // Vertical
						checkFour(i, j, 1, 1, sym) ||  // Diagonal down-right
						checkFour(i, j, 1, -1, sym))   // Diagonal down-left
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
		// Check if board is full (no blank cells)
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (board[i][j] == blank_symbol)
					return false;
			}
		}
		return !is_win(player);
	}

	bool game_is_over(Player<char>* player) override {
		return is_win(player) || is_draw(player);
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

// ======================== OBSTACLES TIC-TAC-TOE UI ========================
class Obstacles_UI : public UI<char> {
public:
	Obstacles_UI() : UI<char>("=== Obstacles Tic-Tac-Toe ===\nGet 4-in-a-row! Watch obstacles (#)!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		Obstacles_Board* board = dynamic_cast<Obstacles_Board*>(player->get_board_ptr());
		int x, y;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn (" << player->get_symbol() << ")\n";
			cout << "Enter row and column (0-5): ";
			cin >> x >> y;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "❌ Invalid input! Please enter numbers.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 6 || y < 0 || y >= 6) {
				cout << "❌ Position out of bounds! Enter 0-5.\n";
				return get_move(player);
			}

			if (board) {
				char cell = board->get_board_matrix()[x][y];
				if (cell == '#') {
					cout << "❌ Position (" << x << ", " << y << ") is blocked by an obstacle!\n";
					return get_move(player);
				}
				if (cell != '.') {
					cout << "❌ Position (" << x << ", " << y << ") is already occupied!\n";
					return get_move(player);
				}
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
				x = rand() % 6;
				y = rand() % 6;
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

#endif // GAME10_OBSTACLESTICTACTOE_BOARD_H