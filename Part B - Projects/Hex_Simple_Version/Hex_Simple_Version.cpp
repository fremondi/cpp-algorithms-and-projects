// Hex_Simple_Version.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stack>

using namespace std;

enum class Player { NONE = '.', BLUE = 'B', RED = 'R' };

class Graph {
private:
    int size;
    vector<vector<int>> adj;

    bool is_valid(int r, int c) const {
        return r >= 0 && r < size && c >= 0 && c < size;
    }

public:
    Graph(int s) : size(s), adj(s* s) {
        int dr[] = { -1, -1, 0, 0, 1, 1 }; // neighbour: change in row
        int dc[] = { 0, 1, -1, 1, -1, 0 }; // neighbour: change in column

        for (int r = 0; r < size; ++r) {   // iterate rows
            for (int c = 0; c < size; ++c) {  // iterate columns
                int u = r * size + c;           // identify node in flattened adjacency list
                for (int d = 0; d < 6; ++d) {   // iterate for each potential direction
                    int nr = r + dr[d];
                    int nc = c + dc[d];
                    if (is_valid(nr, nc)) {     // ensures neighbour is in the board
                        int v = nr * size + nc;
                        adj[u].push_back(v);
                    }
                }
            }
        }
    }

    const vector<int>& neighbors(int node) const {
        return adj[node];
    }
};

void draw_board(const vector<Player>& board, int size) {
    for (int r = 0; r < size; ++r) {
        cout << string(r, ' ');
        for (int c = 0; c < size; ++c) {
            cout << static_cast<char>(board[r * size + c]) << " ";
        }
        cout << endl;
    }
}

bool dfs_check_win(const Graph& g, const vector<Player>& board, Player player, int size, bool vertical) {
    vector<bool> visited(size * size, false);
    stack<int> stk;

    if (vertical) {
        for (int col = 0; col < size; ++col) {
            int idx = 0 * size + col;
            if (board[idx] == player) {
                stk.push(idx);
                visited[idx] = true;
            }
        }
    }
    else {
        for (int row = 0; row < size; ++row) {
            int idx = row * size + 0;
            if (board[idx] == player) {
                stk.push(idx);
                visited[idx] = true;
            }
        }
    }

    while (!stk.empty()) {
        int curr = stk.top(); stk.pop(); // current flattened cell being explored in the DFS, then removed from stk
        int r = curr / size, c = curr % size;

        if (vertical && r == size - 1)
            return true; // blue wins
        if (!vertical && c == size - 1)
            return true; // red wins

        for (int nei : g.neighbors(curr)) {
            if (!visited[nei] && board[nei] == player) {
                visited[nei] = true;
                stk.push(nei); // add neighbour of the same colour to continue path towards other end of the table, new "curr"
            }
        }
    }

    return false; // if stk is empty and no win is recorded, then player didn't win
}

bool is_valid_move(const vector<Player>& board, int pos) {
    return board[pos] == Player::NONE; // valid if no player already selected that node previously
}

int get_random_move(const vector<Player>& board) {
    vector<int> empty;
    for (int i = 0; i < board.size(); ++i) {
        if (board[i] == Player::NONE)
            empty.push_back(i);  // add all the available nodes/moves
    }
    if (empty.empty()) return -1; // if board is full no move can be done
    return empty[rand() % empty.size()]; // random integer % size to ensure it is within empty list bounds
}

int main() {
    srand(time(0));
    const int size = 7;
    Graph g(size);
    vector<Player> board(size * size, Player::NONE);

    bool blue_turn = true;
    while (true) {
        int move = get_random_move(board);
        if (move == -1) break;
        board[move] = blue_turn ? Player::BLUE : Player::RED;  // assign BLUE to the node/move if blue_turn is true, RED otherwise

        if (dfs_check_win(g, board, Player::BLUE, size, true)) {
            draw_board(board, size);
            cout << "BLUE wins!\n";
            break;
        }
        if (dfs_check_win(g, board, Player::RED, size, false)) {
            draw_board(board, size);
            cout << "RED wins!\n";
            break;
        }

        blue_turn = !blue_turn;  // switch blue and red turn one after the other
    }
    return 0;
}