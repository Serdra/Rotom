#include "includes.h"

void UGI() {
    std::string command;

    std::cout << "id name Rotom" << std::endl;
    std::cout << "id author Serdra" << std::endl;
    std::cout << "ugiok" << std::endl;

    // Deals with option setting
    while(true) {
        if(!getline(std::cin, command)) return;
        std::vector<std::string> split = chess::splitString(command, ' ');
        if(split.size() == 0) continue;

        if(split[0] == "isready") {
            std::cout << "readyok" << std::endl;
            break;
        }
    }

    chess::Board pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 -", true);
    while(true) {
        if(!getline(std::cin, command)) return;
        std::vector<std::string> split = pokechess::chess::splitString(command, ' ');
        if(split.size() == 0) continue;

        if(split[0] == "isready") {
            std::cout << "readyok\n";
        }
        
        else if(split[0] == "position") {
            int i = 1;
            if(split[i] == "fen") {
                std::string fen;
                i = 2;
                while(split.size() > i && split[i] != "moves") {
                    fen.append(split[i]);
                    fen.append(" ");
                    i++;
                }
                pos = chess::Board(fen, false);
            }
            if(split.size() > i && split[i] == "moves") {
                i++;
                while(split.size() > i) {
                    pos.makeMove(chess::fromUGI(pos, split[i]));
                    i++;
                }
            }
            // std::cout << "info string fen " << pos.getFen() << std::endl;
        }
        
        else if(split[0] == "uginewgame") continue;
        
        else if (split[0] == "query") {
            if(split[1] == "p1turn") {
                std::cout << "response " << (pos.sideToMove() == chess::Color::White ? "true" : "false") << std::endl;
            }
            if(split[1] == "gameover") {
                std::cout << "response " << (pos.isGameOver() == chess::GameResult::NONE ? "false" : "true") << std::endl;
            }
            if(split[1] == "result") {
                if(pos.isGameOver() == chess::GameResult::NONE) std::cout << "response none" << std::endl;

                if(pos.isGameOver() == chess::GameResult::WIN && pos.sideToMove() == chess::Color::White) std::cout << "response p1win" << std::endl;
                if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::Black) std::cout << "response p1win" << std::endl;

                if(pos.isGameOver() == chess::GameResult::WIN && pos.sideToMove() == chess::Color::Black) std::cout << "response p2win" << std::endl;
                if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::White) std::cout << "response p2win" << std::endl;

                if(pos.isGameOver() == chess::GameResult::DRAW) std::cout << "response draw" << std::endl;
            }
        }

        // Not in the UGI spec but a *very* useful tool for debugging nonetheless
        else if(split[0] == "display") {
            std::cout << pos << std::endl;
        }

        else if(split[0] == "go") {
            chess::Movelist legalMoves;
            chess::legalmoves(legalMoves, pos);
            std::cout << "bestmove " << legalMoves[rand() % legalMoves.size()] << std::endl;
        }

        else if(split[0] == "quit") {
            break;
        }
    }
    
}