// online reference: https://github.com/KeithGalli/Connect4-Python/blob/master/connect4_with_ai.py?fbclid=IwAR1lZbBoWsbVU-GhsWCqjW7YlSukyNk8tsXHT8_CXacWBeyPSHMtWxgkNFw
// discussed with b06303131 b08501098
#include "connect4.h"
using namespace std;
#define ME 1
#define YP 2
#define EMPTY 0
#define COLNUM 7
#define ROWNUM 6
const long long inf = 2e18;
// The columns are numbered from 0 to 6
vector<vector<int>> board(ROWNUM, vector<int>(COLNUM));

bool winning_move(vector<vector<int>> &board, int player){
    // check horizontal locations for win
    for(int c = 0; c < COLNUM - 3; ++c){
        for(int r = 0; r < ROWNUM; ++r){
            if(board[r][c] == player && board[r][c+1] == player && board[r][c+2] == player && board[r][c+3] == player){
                return 1;
            }
        }
    }
    // check vertical locations for win
    for(int c = 0; c < COLNUM; ++c){
        for(int r = 0; r < ROWNUM - 3; ++r){
            if(board[r][c] == player && board[r+1][c] == player && board[r+2][c] == player && board[r+3][c] == player){
                return 1;
            }
        }
    }
    // check positively sloped diagonals
    for(int c = 0; c < COLNUM - 3; ++c){
        for(int r = 0; r < ROWNUM - 3; ++r){
            if(board[r][c] == player && board[r+1][c+1] == player && board[r+2][c+2] == player && board[r+3][c+3] == player){
                return 1;
            }
        }
    }
    // check negatively sloped diagonals
    for(int c = 0; c < COLNUM - 3; ++c){
        for(int r = 3; r < ROWNUM; ++r){
            if(board[r][c] == player && board[r-1][c+1] == player && board[r-2][c+2] == player && board[r-3][c+3] == player){
                return 1;
            }
        }
    }
    return 0;
}
int get_vacant_row(vector<vector<int>> &board, int c){
    for(int r = 0; r < ROWNUM; ++r){
        if(!board[r][c]) { return r; }
    }
    return -1;
}
long long count(vector<int> &v, int p){
    long long c = 0;
    for(auto it : v){
        c += (it == p) ? 1 : 0;
    }
    return c;
}
long long evaluate_window(vector<int> &window, int player){
    long long score = 0;
    int op = (player == ME) ? YP : ME;
    
    long long count1 = count(window, player), count2 = count(window, op), count3 = count(window, EMPTY);
    if(count1 == 4) { score += 111; }
    else if(count1 == 3 && count3 == 1) { score += 88; }
    else if(count1 == 2 && count3 == 2) { score += 33; }

    if(count2 == 3 && count3 == 1) { score -= 17; }
    if(count2 == 4) { score -= 111; }
    return score;
}
long long score_position(vector<vector<int>> &board, int player){
    long long score = 0;

    // central column
    vector<int> center_array;
    for(int r = 0; r < ROWNUM; ++r){
        center_array.push_back(board[r][COLNUM/2]);
    }
    long long center_count = count(center_array, player);
    score += 3 * center_count;

    // horizontal
    vector<int> rowArray(COLNUM);
    for(int r = 0; r < ROWNUM; ++r){
        for(int c = 0; c < COLNUM; ++c) { rowArray[c] = board[r][c]; }
        for(int c = 0; c < COLNUM - 3; ++c){
            vector<int> window;
            window.assign(rowArray.cbegin() + c, rowArray.cbegin() + c + 4);
            score += evaluate_window(window, player);
        }
    }
    // vertical
    vector<int> colArray(ROWNUM);
    for(int c = 0; c < COLNUM; ++c){
        for(int r = 0; r < ROWNUM; ++r) { colArray[r] = board[r][c]; }
        for(int r = 0; r < ROWNUM - 3; ++r){
            vector<int> window;
            window.assign(colArray.cbegin() + r, colArray.cbegin() + r + 4);
            score += evaluate_window(window, player);
        }
    }
    // positively sloped diagonal
    for(int r = 0; r < ROWNUM - 3; ++r){
        for(int c = 0; c < COLNUM - 3; ++c){
            vector<int> window;
            for(int i = 0; i < 4; ++i){
                window.push_back(board[r+i][c+i]);
            }
            score += evaluate_window(window, player);
        }
    }
    // negatively sloped diagonal
    for(int r = 0; r < ROWNUM - 3; ++r){
        for(int c = 0; c < COLNUM - 3; ++c){
            vector<int> window;
            for(int i = 0; i < 4; ++i){
                window.push_back(board[r+3-i][c+i]);
            }
            score += evaluate_window(window, player);
        }
    }
    return score;
}
bool is_valid_location(vector<vector<int>> &board, int col){
    return (!board[ROWNUM-1][col]);
}
vector<int> get_valid_location(vector<vector<int>> &board){
    vector<int> valid_col;
    for(int c = 0; c < COLNUM; ++c){
        if(is_valid_location(board, c)) valid_col.push_back(c);
    }
    return valid_col;
}
bool is_terminal_node(vector<vector<int>> &board){
    return winning_move(board, YP) || winning_move(board, ME) || !get_valid_location(board).size();
}
pair<int, long long> mini_max(vector<vector<int>> &board, int dep, long long alph, long long beta, bool to_maximize){
    // printf("enter the minimax function\n");
    vector<int> valid_col = get_valid_location(board);
    bool isTerminal = is_terminal_node(board);
    if(!dep || isTerminal){
        if(isTerminal){
            if(winning_move(board, ME)) { return make_pair(-1, inf); }
            if(winning_move(board, YP)) { return make_pair(-1, -inf); }
            return make_pair(-1, 0);
        }
        return make_pair(-1, score_position(board, ME));
    }
    if(to_maximize){
        // printf("hello1\n");
        long long val = -inf;
        int col = valid_col[rand() % valid_col.size()];
        for(int c : valid_col){
            int row = get_vacant_row(board, c);
            vector<vector<int>> board2;
            board2.assign(board.cbegin(), board.cend());
            board2[row][c] = ME;
            long long newScore = mini_max(board2, dep-1, alph, beta, 0).second;
            if(newScore > val){
                val = newScore;
                col = c;
            }
            alph = max(alph, val);
            if(alph >= beta) { break; }
        }
        return make_pair(col, val);
    }
    else{
        long long val = inf;
        int col = valid_col[rand() % valid_col.size()];
        for(int c : valid_col){
            int row = get_vacant_row(board, c);
            vector<vector<int>> board2;
            board2.assign(board.cbegin(), board.cend());
            board2[row][c] = YP;
            long long newScore = mini_max(board2, dep-1, alph, beta, 1).second;
            if(newScore < val){
                val = newScore;
                col = c;
            }
            beta = min(beta, val);
            if(alph >= beta) { break; }
        }
        return make_pair(col, val);
    }
}

int decide(int yp_move) {
    // printf("enter the decide function, yp_move = %d\n", yp_move);
    if (yp_move == -1) {
        // A new game starts
        // TODO: Please remember to reset everything here (if needed)
        srand(time(NULL));
        for(int i = 0; i < ROWNUM; ++i){
            for(int j = 0; j < COLNUM; ++j){
                board[i][j] = EMPTY;
            }
        }
        board[0][3] = ME;
        return 3;
    }
    else {
        // YP placed his piece at column `yp_move` 
        int i = 0;
        while(board[i][yp_move]){
            ++i;
        }
        board[i][yp_move] = YP;
        // printf("before mini_max function\n");
        int col = mini_max(board, 6, -inf, inf, 1).first;
        if(col != -1 && is_valid_location(board, col)){
            int row = get_vacant_row(board, col);
            board[row][col] = ME;
        }
        return col;
    }
    return 6; // always place at the rightmost column
}