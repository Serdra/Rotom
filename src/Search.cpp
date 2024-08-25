#include "includes.h"

std::pair<chess::Move, int> IterativeDeepening(chess::Board position, StopType stop, int stopValue, TransTable &TT) {
    auto start = std::chrono::high_resolution_clock::now();

    std::chrono::time_point<std::chrono::high_resolution_clock> hardTime, softTime;
    softTime = start + std::chrono::milliseconds((int)(stopValue * SOFT_LIMIT_MULT));
    hardTime = start + std::chrono::milliseconds((int)(stopValue * HARD_LIMIT_MULT));

    SearchSettings settings;

    settings.stop = stop;
    settings.nodes = stopValue;
    settings.endTime = hardTime;
    settings.timeout = false;

    int depth = 1;
    uint64_t nodes = 0;

    int evaluation;
    int result;

    chess::Move previousBestMove;

    while(true) {
        Stack stack[256];
        if(depth == 1) 
            result = Negamax(position, depth, -INF, +INF, 0, stack, settings, TT, nodes);
        else {
            result = Negamax(position, depth, evaluation - 30, evaluation + 30, 0, stack, settings, TT, nodes);
            if(result <= (evaluation - 30) || result >= (evaluation + 30)) {
                result = Negamax(position, depth, -INF, +INF, 0, stack, settings, TT, nodes);
            }
        }

        if(!settings.timeout) {
            previousBestMove = stack[0].pv.moves[0];
            evaluation = result;

            if(printSearchUpdates) {
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                int u = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

                std::cout << "info depth " << depth << " score cp " << result;
                std::cout << " time " << u << " nodes " << nodes;
                if(u >= 2) std::cout << " nps " << (int)(nodes / (u / 1000.f));
                std::cout << " pv ";
                for(auto move : stack[0].pv.moves) std::cout << move << " ";
                std::cout << std::endl;
            }
        }

        if(settings.timeout) break;
        
        if(stop == StopType::Time && std::chrono::high_resolution_clock::now() >= softTime) break;
        if(stop == StopType::Nodes && nodes >= stopValue) break;
        if(stop == StopType::Depth && depth >= stopValue) break;
        if(depth > MAX_DEPTH) break;

        depth++;
    }

    return {previousBestMove, evaluation};
}

int Negamax(chess::Board &position, int depth, int alpha, int beta, int ply, Stack* stack, SearchSettings &settings, TransTable &TT, uint64_t &nodes) {
    // Step one is to check if the game is over. This is fast and perfectly accurate so we do it first
    if(position.isGameOver() != chess::GameResult::NONE) {
        if(position.isGameOver() == chess::GameResult::WIN) return MATE - ply;
        if(position.isGameOver() == chess::GameResult::LOSE) return -(MATE - ply);
        return 0;
    }

    // If the position is not game over, we check the base case (this is a recursive algorithm after all)
    if(depth <= 0 || ply > MAX_PLY) return QSearch(position, alpha, beta, ply, nodes);

    TTEntry entry = TT.probe(position.hash());
    if(entry.hash == position.hash() && entry.depth >= depth && !stack[ply].isPV) {
        if(entry.flag == TTFlags::Exact) {
            stack[ply].pv.moves.push_back(entry.bestMove);
            return entry.score;
        }
        if(entry.flag == TTFlags::Upper) {
            beta = std::min((int)entry.score, beta);
        }
        else if(entry.flag == TTFlags::Lower) {
            alpha = std::max((int)entry.score, alpha);
        }

        if(alpha >= beta) {
            stack[ply].pv.moves.push_back(entry.bestMove);
            return entry.score;
        }
    }

    // Next we check if the time is up
    if(settings.stop == StopType::Time && std::chrono::high_resolution_clock::now() >= settings.endTime) {
        settings.timeout = true;
        return 0;
    } 
    if(settings.stop == StopType::Nodes && nodes >= settings.nodes) {
        settings.timeout = true;
        return 0;
    }

    int static_eval = eval(position);

    // if(!stack[ply].isPV && stack[ply].canDoNullMove && depth > 2 && !position.inCheck() && static_eval >= beta) {
    //     stack[ply+1].canDoNullMove = false;
    //     stack[ply+1].isPV = false;

    //     chess::Board newPosition = position;
    //     newPosition.makeMove(chess::Move(0, 0));
    //     nodes++;

    //     int value = -Negamax(newPosition, -beta, -beta + 1, depth - 3, ply + 1, stack, settings, TT, nodes);
    //     stack[ply].pv.moves.clear();
    //     stack[ply+1].pv.moves.clear();
    //     stack[ply+1].canDoNullMove = true;
    //     if(value >= beta) {
    //         return value;
    //     }
    // }

    // Searches each move in turn
    MovePicker moves(position, entry.bestMove);
    chess::Move move;

    int result;
    int bestMoveValue = -INF;
    int alphaOrig = alpha;

    while(moves.next(move, position)) {
        stack[ply+1].pv.moves.clear();
        // I prefer copy-make to make-unmake
        chess::Board newPosition = position;
        newPosition.makeMove(move);

        nodes++;

        // We only do a full width search on the first move by default
        // All other moves are assumed to be worse and we do a zero width search instead
        // If that search fails high, then we do a full width search
        if(moves.curr == 1) {
            stack[ply+1].isPV = true;

            if(newPosition.sideToMove() != position.sideToMove()) {
                result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, stack, settings, TT, nodes);
            } else {
                result = Negamax(newPosition, depth, alpha, beta, ply + 1, stack, settings, TT, nodes);
            }
        } else {
            stack[ply+1].isPV = false;

            if(newPosition.sideToMove() != position.sideToMove()) {
                result = -Negamax(newPosition, depth - 1, -alpha - 1, -alpha, ply + 1, stack, settings, TT, nodes);
            } else {
                result = Negamax(newPosition, depth, alpha, alpha+1, ply + 1, stack, settings, TT, nodes);
            }
            if(result > alpha && result < beta) {
                if(newPosition.sideToMove() != position.sideToMove()) {
                    result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, stack, settings, TT, nodes);
                } else {
                    result = Negamax(newPosition, depth, alpha, beta, ply + 1, stack, settings, TT, nodes);
                }
            }
        }
        if(settings.timeout) return 0;

        if(result > bestMoveValue) {
            // Update the PV
            stack[ply].pv.moves.clear();
            stack[ply].pv.moves.push_back(move);

            // If there is a continuation after the bestmove, update it
            if(stack[ply+1].pv.moves.size() != 0) {
                stack[ply].pv.moves.insert(stack[ply].pv.moves.end(), stack[ply+1].pv.moves.begin(), stack[ply+1].pv.moves.end());
            }
            bestMoveValue = result;
        }
        alpha = std::max(alpha, bestMoveValue);
        if(alpha >= beta) break;
    }
    
    entry.hash = position.hash();
    if(stack[ply].pv.moves.size() != 0) entry.bestMove = stack[ply].pv.moves[0];
    entry.score = bestMoveValue;
    entry.depth = depth;

    if(alpha >= beta) entry.flag = TTFlags::Lower;
    else if(bestMoveValue <= alphaOrig) entry.flag = TTFlags::Upper;
    else entry.flag = TTFlags::Exact;

    TT.insert(entry);
    return bestMoveValue;
}

int QSearch(chess::Board &position, int alpha, int beta, int ply, uint64_t &nodes) {
    if(position.isGameOver() != chess::GameResult::NONE) {
        if(position.isGameOver() == chess::GameResult::WIN) return MATE - ply;
        if(position.isGameOver() == chess::GameResult::LOSE) return -(MATE - ply);
        return 0;
    }

    int static_eval = eval(position);
    if(static_eval >= beta) return static_eval;
    alpha = std::max(alpha, static_eval);

    MovePicker moves(position);
    chess::Move move;

    while(moves.nextCapture(move, position)) {
        chess::Board newPosition = position;
        newPosition.makeMove(move);
        nodes++;
        int value;

        if(newPosition.sideToMove() != position.sideToMove()) {
            value = -QSearch(newPosition, -beta, -alpha, ply + 1, nodes);
        } else {
            value = QSearch(newPosition, alpha, beta, ply + 1, nodes);
        }

        if(value >= beta) return beta;
        if(value > alpha) alpha = value;
    }
    return alpha;
}


void perft(chess::Board &position, int depth, uint64_t &nodes) {
    if(depth == 0) {
        nodes++;
        return;
    }
    if(position.isGameOver() != chess::GameResult::NONE) return;

    chess::Movelist moves;
    chess::legalmoves(moves, position);

    for(int i = 0; i < moves.size(); i++) {
        chess::Board nPos = position;
        nPos.makeMove(moves[i]);
        perft(nPos, depth-1, nodes);
    }
}