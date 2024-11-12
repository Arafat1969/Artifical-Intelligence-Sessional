#include<bits/stdc++.h>
using namespace std;

class Board {
public:
    vector<vector<char>> board;
    int size;
    int depth;
    int cost;
    bool isExplored;
    bool isExpanded;
    Board* parent;

    Board(int k, vector<vector<char>> board, int depth, Board* parent = nullptr) {
        size = k;
        this->board = board;
        this->depth = depth;
        this->cost = 0;
        this->isExpanded = false;
        this->isExplored = false;
        this->parent = parent;
    }

    char getTargetNumber(int row, int col, int size) {
        if (row == size - 1 && col == size - 1) {
            return '*';
        }
        return char(row * size + col + 1 + 48);  // Converts the number to char
    }

    vector<vector<char>> getTargetBoard() {
        vector<vector<char>> target(size, vector<char>(size));  // Proper initialization
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                target[i][j] = getTargetNumber(i, j, size);
            }
        }
        return target;
    }

    vector<char> line() {
        vector<char> v;
        for (auto &i: board) {
            for (auto j: i) {
                if (j != '*') v.push_back(j);
            }
        }
        return v;
    }

    int findInversionCount() {
        int inversion = 0;
        vector<char> flat = line();
        for (int i = 0; i < flat.size(); i++) {
            for (int j = i + 1; j < flat.size(); j++) {
                if (flat[i] > flat[j]) inversion++;
            }
        }
        return inversion;
    }

    int getBlankSquareRow() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == '*') return i;
            }
        }
        return -1;
    }

    bool isSolvable() {
        int inversions = findInversionCount();
        if (size % 2 == 1) {
            return inversions % 2 == 0;
        } else {
            int blankSquareRow = getBlankSquareRow();
            return (inversions + blankSquareRow) % 2 == 1;  
        }
    }

    void hammingCost() {
        int distance = 0;
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                if (board[i][j] != '*' && board[i][j] != getTargetNumber(i, j, size)) {
                    distance++;
                }
            }
        }
        cost = depth + distance;
    }

    void manhattanCost() {
        int distance = 0;
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                char target = getTargetNumber(i, j, size);
                if (target != '*') {
                    for (int m = 0; m < board.size(); m++) {
                        for (int n = 0; n < board[0].size(); n++) {
                            if (board[m][n] == target) {
                                distance += abs(i - m) + abs(j - n);
                            }
                        }
                    }
                }
            }
        }
        cost = depth + distance;
    }
};

void findOptimalCost(Board* initialBoard, bool isHamming) {
    int exploredCount = 0;
    int expandedCount = 0;
    vector<Board*> exploredList;
    stack<Board*> path;
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

   
    priority_queue<Board*, vector<Board*>, function<bool(Board*, Board*)>> astarQueue(
        [](Board* a, Board* b) {
            return a->cost > b->cost;
        }
    );

    if (isHamming) {
        initialBoard->hammingCost();
    } else {
        initialBoard->manhattanCost();
    }

    astarQueue.push(initialBoard);
    exploredList.push_back(initialBoard);
    exploredCount++;

    while (!astarQueue.empty()) {
        Board* current = astarQueue.top();
        astarQueue.pop();

        if (current->board == current->getTargetBoard()) {
            cout << "Number of explored nodes: " << exploredCount << endl;
            cout << "Number of expanded nodes: " << expandedCount << endl;
            cout << "Optimal Cost: " << current->cost << endl;

            Board* pathBoard = current;
            while (pathBoard != nullptr) {
                path.push(pathBoard);
                pathBoard = pathBoard->parent;
            }

            while (!path.empty()) {
                Board* tmp = path.top();
                for (int x = 0; x < tmp->board.size(); x++) {
                    for (int y = 0; y < tmp->board[0].size(); y++) {
                        cout << tmp->board[x][y] << " ";
                    }
                    cout << endl;
                }
                cout << "-----------------------------------------" << endl;
                path.pop();
            }

            return;
        }

        int blankRow, blankColumn;
        for (int i = 0; i < current->board.size(); i++) {
            for (int j = 0; j < current->board[0].size(); j++) {
                if (current->board[i][j] == '*') {
                    blankRow = i;
                    blankColumn = j;
                    break;
                }
            }
        }

        for (auto direction : directions) {
            int changedRow = blankRow + direction.first;
            int changedColumn = blankColumn + direction.second;

            if (changedRow >= 0 && changedRow < current->board.size() &&
                changedColumn >= 0 && changedColumn < current->board[0].size()) {

                vector<vector<char>> newBoard = current->board;
                swap(newBoard[blankRow][blankColumn], newBoard[changedRow][changedColumn]);

                bool isExplored = false;
                for (auto exploredBoard : exploredList) {
                    if (newBoard == exploredBoard->board) {
                        isExplored = true;
                        break;
                    }
                }

                if (!isExplored) {
                    Board* child = new Board(current->size, newBoard, current->depth + 1, current);
                    if (isHamming) {
                        child->hammingCost();
                    } else {
                        child->manhattanCost();
                    }
                    astarQueue.push(child);
                    exploredList.push_back(child);
                    exploredCount++;
                }
            }
        }

        if (!current->isExpanded) {
            current->isExpanded = true;
            expandedCount++;
        }
    }
}

int main() {
    int k;
    cout << "Enter the dimension size of your puzzle: ";
    cin >> k;
    vector<vector<char>> puzzle(k, vector<char>(k));

    cout << "Enter the elements sequentially (use * for blank):" << endl;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            cin >> puzzle[i][j];
        }
    }

    Board* board = new Board(k, puzzle, 0);
    if (board->isSolvable()) {
        cout << "Hamming Heuristic:" << endl;
        findOptimalCost(board, true);
        cout << "============================================" << endl;
        cout << "Manhattan Heuristic:" << endl;
        findOptimalCost(board, false);
    } else {
        cout << "The puzzle is not solvable" << endl;
    }

    return 0;
}
