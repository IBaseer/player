/**
 * @file Game4_WordTicTacToe_Board.h
 * @brief Word Tic-Tac-Toe - FIXED VERSION
 */

#ifndef GAME4_WORDTICTACTOE_BOARD_H
#define GAME4_WORDTICTACTOE_BOARD_H

#include "BoardGame_Classes.h"
#include <cctype>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

// ======================== WORD TIC-TAC-TOE BOARD ========================
class WordTicTacToe_Board : public Board<char> {
private:
	char blank_symbol = '.';
	set<string> dictionary;

	void loadDictionary() {
		ifstream file("dic.txt");
		if (file.is_open()) {
			string word;
			while (file >> word) {
				for (char& c : word) c = toupper(c);
				dictionary.insert(word);
			}
			file.close();
		}
	}

	bool isValidWord(const string& word) {
		string upper_word = word;
		for (char& c : upper_word) c = toupper(c);
		return dictionary.find(upper_word) != dictionary.end();
	}

	bool checkWord(int x, int y, int dx, int dy) {
		string word = "";
		for (int i = 0; i < 3; i++) {
			int nx = x + i * dx;
			int ny = y + i * dy;
			if (nx < 0 || nx >= rows || ny < 0 || ny >= columns)
				return false;
			if (board[nx][ny] == blank_symbol)
				return false;
			word += board[nx][ny];
		}
		return isValidWord(word);
	}

public:
	WordTicTacToe_Board() : Board(3, 3) {
		for (auto& row : board)
			for (auto& cell : row)
				cell = blank_symbol;
		loadDictionary();
	}

	bool update_board(Move<char>* move) override {
		int x = move->get_x();
		int y = move->get_y();
		char letter = toupper(move->get_symbol());

		if (x < 0 || x >= rows || y < 0 || y >= columns)
			return false;

		if (board[x][y] != blank_symbol)
			return false;

		if (!isalpha(letter))
			return false;

		board[x][y] = letter;
		n_moves++;
		return true;
	}

	bool is_win(Player<char>* player) override {
		// Check rows
		for (int i = 0; i < rows; i++) {
			if (checkWord(i, 0, 0, 1))
				return true;
		}

		// Check columns
		for (int j = 0; j < columns; j++) {
			if (checkWord(0, j, 1, 0))
				return true;
		}

		// Check diagonals
		if (checkWord(0, 0, 1, 1) || checkWord(0, 2, 1, -1))
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

// ======================== WORD TIC-TAC-TOE UI ========================
class WordTicTacToe_UI : public UI<char> {
public:
	WordTicTacToe_UI() : UI<char>("=== Word Tic-Tac-Toe ===\nForm valid 3-letter words!", 3) {}

	Player<char>* create_player(string& name, char symbol, PlayerType type) override {
		return new Player<char>(name, symbol, type);
	}

	Move<char>* get_move(Player<char>* player) override {
		WordTicTacToe_Board* board = dynamic_cast<WordTicTacToe_Board*>(player->get_board_ptr());
		int x, y;
		char letter;

		if (player->get_type() == PlayerType::HUMAN) {
			cout << "\n" << player->get_name() << "'s turn\n";
			cout << "Enter row (0-2), column (0-2), and letter: ";
			cin >> x >> y >> letter;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "? Invalid input!\n";
				return get_move(player);
			}

			if (!isalpha(letter)) {
				cout << "? Invalid letter! Enter A-Z.\n";
				return get_move(player);
			}

			if (x < 0 || x >= 3 || y < 0 || y >= 3) {
				cout << "? Position out of bounds! Enter 0-2.\n";
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
				x = rand() % 3;
				y = rand() % 3;
			}
			letter = 'A' + (rand() % 26);
			cout << "\n" << player->get_name() << " plays: (" << x << ", " << y << ", " << letter << ")\n";
		}

		return new Move<char>(x, y, toupper(letter));
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

#endif // GAME4_WORDTICTACTOE_BOARD_H