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
            int timeRemaining = -1;
            int incTime = -1;
            int mTime = -1;
            int depth = -1;
            int nodes = -1;
            StopType stopType = StopType::Infinite;

            int i = 1;
            while(split.size() > i) {
                if(split[i] == "p1time" && pos.sideToMove() == chess::Color::White) {
                    timeRemaining = std::stoi(split[i + 1]);
                    stopType = StopType::Time;
                }
                else if(split[i] == "p2time" && pos.sideToMove() == chess::Color::Black) {
                    timeRemaining = std::stoi(split[i + 1]);
                    stopType = StopType::Time;
                }
                else if(split[i] == "p1inc" && pos.sideToMove() == chess::Color::White) {
                    incTime = std::stoi(split[i + 1]);
                    stopType = StopType::Time;
                }
                else if(split[i] == "p2inc" && pos.sideToMove() == chess::Color::Black) {
                    incTime = std::stoi(split[i + 1]);
                    stopType = StopType::Time;
                }

                else if(split[i] == "nodes") {
                    nodes = std::stoi(split[i + 1]);
                    stopType = StopType::Nodes;
                }

                else if(split[i] == "time" || split[i] == "movetime") {
                    mTime = std::stoi(split[i + 1]);
                    stopType = StopType::Time;
                }

                else if(split[i] == "depth") {
                    depth = std::stoi(split[i + 1]);
                    stopType = StopType::Depth;
                }
                i++;
            }

            chess::Move bestMove;

            if(stopType == StopType::Infinite) bestMove = IterativeDeepening(pos, stopType, 0);
            if(stopType == StopType::Time) {
                if(mTime == -1) {
                    bestMove = IterativeDeepening(pos, stopType, std::min(timeRemaining / 2, (timeRemaining / 30) + incTime));
                } else {
                    bestMove = IterativeDeepening(pos, stopType, mTime);
                }
            }
            if(stopType == StopType::Nodes) bestMove = IterativeDeepening(pos, stopType, nodes);
            if(stopType == StopType::Depth) bestMove = IterativeDeepening(pos, stopType, depth);

            std::cout << "bestmove " << bestMove << std::endl;
        }

        else if(split[0] == "quit") {
            break;
        }
    }
    
}