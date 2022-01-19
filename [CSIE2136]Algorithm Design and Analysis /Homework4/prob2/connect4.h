// Comment out the next line to disable game board output
#define CONNECT4_DEBUG

// Uncomment the next line to interactively play with your own code
// #define CONNECT4_INTERACTIVE

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace connect4_judge {

constexpr char serv_ip[] = "140.112.30.36";
constexpr int serv_port = 8877;

static const std::pair<int, int> board_size = {6, 7}; // row * column

class Game {
  public:
    enum Result {
        ONGOING,
        TIE,
        P1_WIN,
        P2_WIN,
        INVALID_MOVE,
    };

    std::vector<std::vector<int>> board; // 0: empty, 1: P1, 2: P2
    int turn;
    Result status;

    Game(): board(board_size.first, std::vector<int>(board_size.second, 0)), turn(1), status(ONGOING) {}
    Result play(int col) {
        if (status != ONGOING)
            return INVALID_MOVE; // Already ended
        if (col < 0 || col >= board_size.second)
            return INVALID_MOVE; // Not a valid column
        if (board[0][col] != 0)
            return INVALID_MOVE; // Top out

        int row = board_size.first - 1;
        while (board[row][col] != 0)
            row--;
        board[row][col] = turn;

        auto check_single = [&](int x, int y, int dx, int dy) -> int {
            if (!board[x][y] || x + dx * 3 >= board_size.first ||
                    y + dy * 3 >= board_size.second || y + dy * 3 < 0)
                return 0;
            for (int i = 1; i <= 3; i++)
                if (board[x + dx * i][y + dy * i] != board[x][y])
                    return 0;
            return board[x][y];
        };

        bool any_empty = false;
        for (int i = 0; i < board_size.first; i++)
            for (int j = 0; j < board_size.second; j++) {
                any_empty |= (board[i][j] == 0);
                int dx[4] = {0, 1, 1, 1}, dy[4] = {1, 0, 1, -1};
                for (int k = 0; k < 4; k++) {
                    int res = check_single(i, j, dx[k], dy[k]);
                    if (res == 1)
                        return status = P1_WIN;
                    else if (res == 2)
                        return status = P2_WIN;
                }
            }
        if (!any_empty)
            return status = TIE;
        turn = 3 - turn; // switch turn
        return ONGOING;
    }
    friend std::ostream& operator<<(std::ostream &ss, const Game &game) {
        ss << "\n";
        switch (game.status) {
            case P1_WIN: ss << "P1 Wins!\n"; break;
            case P2_WIN: ss << "P2 Wins!\n"; break;
            case TIE:    ss << "TIE!\n"; break;
            default:     ss << "Turn: P" << game.turn << "\n";
        }
        ss << "p1:O p2:X\n";
        for (int i = -1; i <= board_size.first; i++) {
            ss << (i == -1 || i == board_size.first ? "+" : "|");
            for (int j = 0; j < board_size.second; j++) {
                if (i == -1)
                    ss << j;
                else if (i == board_size.first)
                    ss << "-";
                else if (game.board[i][j] == 0)
                    ss << " ";
                else
                    ss << (game.board[i][j] == 1 ? "O" : "X");
            }
            ss << (i == -1 || i == board_size.first ? "+" : "|") << "\n";
        }
        ss.flush();
        return ss;
    }
};

class YP {
  public:
    YP(int as_player) : side(as_player) {
        _socket_init();
    }
    ~YP() {
        _socket_fin();
    }
    int operator()(const Game &game, double random_p) {
        assert(game.turn == side && game.status == Game::ONGOING); 
        char buf[100];
        std::string board_s = _board_to_str(game.board);
        sprintf(buf, "%s %f\n", board_s.c_str(), random_p);
        // send
        if (send(sockfd, buf, strlen(buf), 0) == -1) {
            perror("send");
            std::cerr << "send() fail" << std::endl;
            exit(1);
        }
        // recv
        int n;
        n = (int)recv(sockfd, buf, 1, 0);
        if (n <= 0) {
            if (n < 0)
                perror("read");
            std::cerr << "read() fail" << std::endl;
            exit(1); 
        }
        int col = buf[0] - '0';
        if (col < 0 || col >= board_size.second) {
            std::cerr << "something went wrong QQ" << std::endl;
            exit(1);
        }
        return col;
    }
  private:
    int side;
    int sockfd;

    void _socket_init() {
        #ifdef _WIN32
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR) {
            std::cerr << "WSAStartup() fail" << std::endl;
            exit(1);
        }
        #endif

        if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            std::cerr << "socket() fail" << std::endl;
            exit(1);
        }

        sockaddr_in serveraddr;
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(serv_port);
        serveraddr.sin_addr.s_addr=inet_addr(serv_ip);

        if (connect(sockfd, (sockaddr*)&serveraddr ,sizeof(serveraddr)) == -1) {
            std::cerr << "connect() fail" << std::endl;
            exit(1);
        }
    }
    void _socket_fin() {
        #ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
        #endif
    }

    std::string _board_to_str(const std::vector<std::vector<int>> &board) {
        std::string res(board_size.first * board_size.second, '0');
        for (int i = 0; i < board_size.first; i++)
            for (int j = 0; j < board_size.second; j++)
                res[i * board_size.second + j] = (board[i][j] + '0');
        return res;
    }
};
} // namespace connect4_judge

int decide(int);

int main() {
    using connect4_judge::Game;
    using connect4_judge::YP;

#ifdef CONNECT4_INTERACTIVE
    std::cout << "Playing interactively!" << std::endl;
    std::cout << "You are Player 2 (X), while your code is Player 1 (O)." << std::endl;
    std::cout << "When it's your turn, please input a digit between 0 and 6 then press <ENTER>." << std::endl;
    #define CONNECT4_DEBUG // Always show game board when playing interacitvely
    int n_games = 1; // Only play single game when playing interactively
#else
    double p;
    std::cout << "p (A real number between 0 and 1, which is the probability YP moves seriously. Or '-1' for sample) = ";
    std::cout.flush();
    if (!(std::cin >> p) || p > 1.0) {
        std::cout << "Input p Failed!" << std::endl;
        exit(1);
    }

    int n_games;
    std::cout << "number of games = ";
    std::cout.flush();
    if (!(std::cin >> n_games) || n_games < 0) {
        std::cout << "Input # games Failed!" << std::endl;
        exit(1);
    }

    YP yp(2);

    int N_WIN = 0, N_TIE = 0, N_LOSE = 0;
#endif

    long long total_time_us = 0;

    for (int round = 0; round < n_games; ++round) {
        Game game;   
#ifdef CONNECT4_DEBUG
#ifndef CONNECT4_INTERACTIVE
        std::cerr << "=== Start of game " << round + 1 << " ===" << std::endl;
#endif
        std::cerr << game; 
#endif
        int last_yp_move = -1;
        while (game.status == Game::ONGOING) {
            Game::Result res = Game::INVALID_MOVE;
            int choice;
            if (game.turn == 1) {
                auto time_start = std::chrono::steady_clock::now();
                choice = decide(last_yp_move);
                auto time_end = std::chrono::steady_clock::now();
                total_time_us += std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start).count();
            }
            else {
#ifdef CONNECT4_INTERACTIVE
                std::cout << "Your move> ";
                std::cout.flush();
                if (!(std::cin >> choice)) {
                    std::cout << "Input move failed!" << std::endl;
                    exit(1);
                }
                last_yp_move = choice;
#else
                choice = last_yp_move = yp(game, p);
#endif
            }
            res = game.play(choice);
            if (res == Game::INVALID_MOVE) {
                std::cerr << "Invalid Move (move = " << choice << ") QQ..." << std::endl;
                exit(1);
            }
#ifdef CONNECT4_DEBUG
            std::cerr << game;
#endif
        }
#ifndef CONNECT4_INTERACTIVE
        if (game.status == Game::P1_WIN)
            N_WIN += 1;
        else if (game.status == Game::P2_WIN)
            N_LOSE += 1;
        else if (game.status == Game::TIE)
            N_TIE += 1;
#endif
    }

    std::cout << "Your code finished in " << std::fixed << std::setprecision(3) << (double)total_time_us / 1e6 << " seconds (Not including YP's thinking time)." << std::endl;

#ifndef CONNECT4_INTERACTIVE
    std::cout << "Done! Played " << n_games << " games." << std::endl;
    std::cout << "The result is " << N_WIN << " Win(s), " << N_TIE << " Tie(s), and " << N_LOSE << " Lose(s)." << std::endl;
    std::cout << "The total score is " << N_WIN * 2 + N_TIE << "." << std::endl;
#endif
}
