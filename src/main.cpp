#include "includes.h"

int main() {
    buildPST();
    srand(time(0));
    std::string input = "analyze";
    std::getline(std::cin, input);

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
    if (input == "analyze") {
        printSearchUpdates = false;
        TransTable TT(256);

        std::string startingFen;
        std::string moveStrings;

        std::cout << "Enter the starting fen: ";
        std::getline(std::cin, startingFen);
        std::cout << "Enter all moves (seperated by a space): ";
        std::getline(std::cin, moveStrings);

        chess::Board position(startingFen, false);

        std::vector<std::string> moves = chess::splitString(moveStrings, ' ');

        int moveTime = 1000;
        std::pair<chess::Move, int> prevResult = IterativeDeepening(position, StopType::Time, moveTime, TT);

        // Validate moves
        for(int i = 0; i < moves.size() && position.isGameOver() == chess::GameResult::NONE; i++) {
            // Check if the move is legal
            chess::Movelist legalMoves;
            chess::legalmoves(legalMoves, position);

            bool valid = false;
            for(int j = 0; j < legalMoves.size(); j++) {
                if(chess::fromUGI(position, moves[i]) == legalMoves[j]) valid = true;
            }
            if(!valid) {
                std::cout << "Illegal move " << moves[i] << std::endl;
                return 0;
            }
            position.makeMove(chess::fromUGI(position, moves[i]));
        }


        position = chess::Board(startingFen, false);
        for(int i = 0; i < moves.size() && position.isGameOver() == chess::GameResult::NONE; i++) {
            position.makeMove(chess::fromUGI(position, moves[i]));
            std::pair<chess::Move, int> result = IterativeDeepening(position, StopType::Time, moveTime, TT);
            if(-result.second < (prevResult.second - 80) && (position.getSE() == 64) && prevResult.first != chess::fromUGI(position, moves[i])) {
                std::cout << "Potential blunder " << moves[i] << " best move was " << prevResult.first << " loss " << prevResult.second + result.second << std::endl;
            }
            else if(result.second < (prevResult.second - 80) && (position.getSE() != 64) && prevResult.first != chess::fromUGI(position, moves[i])) {
                std::cout << "Potential blunder " << moves[i] << " best move was " << prevResult.first << " loss " << prevResult.second - result.second << std::endl;
            }
            prevResult = result;
        }
    }
}