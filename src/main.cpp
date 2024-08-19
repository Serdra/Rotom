#include "includes.h"

int main() {
    buildPST();
    srand(time(0));
    std::string input;
    std::cin >> input;

    if (input == "ugi") UGI();
    if (input == "book") {
        printSearchUpdates = false;
        TransTable TT(128);
        while(true) {
            int randomMoves = (rand() % 5) + 6;
            while(true) {
                chess::Board pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 -", true);
                for(int i = 0; i < randomMoves && pos.isGameOver() == chess::GameResult::NONE; i++) {
                    chess::Movelist moves;
                    chess::legalmoves(moves, pos);
                    pos.makeMove(moves[rand() % moves.size()]);
                }
                if(pos.isGameOver() == chess::GameResult::NONE) {
                    std::pair<chess::Move, int> result = IterativeDeepening(pos, StopType::Depth, 8, TT);
                    if(abs(result.second) < 250) {
                        std::cout << pos.getFen() << std::endl;
                        break;
                    }
                }
            }
        }
    }
}