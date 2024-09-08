#include "includes.h"

std::pair<chess::Move, int> IterativeDeepening(chess::Board position, StopType stop, int stopValue, TransTable &TT, History &Hist) {
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 64; j++) {
            for(int k = 0; k < 64; k++) {
                Hist.score[i][j][k] /= 10;
            }
        }
    }

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
            result = Negamax(position, depth, -INF, +INF, 0, stack, settings, TT, Hist, nodes);
        else {
            result = Negamax(position, depth, evaluation - 30, evaluation + 30, 0, stack, settings, TT, Hist, nodes);
            if(result <= (evaluation - 30) || result >= (evaluation + 30)) {
                result = Negamax(position, depth, -INF, +INF, 0, stack, settings, TT, Hist, nodes);
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

int Negamax(chess::Board &position, int depth, int alpha, int beta, int ply, Stack* stack, SearchSettings &settings, TransTable &TT, History &Hist, uint64_t &nodes) {
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

    if(!stack[ply].isPV && !position.inCheck() && static_eval - RFP_BOUND * depth >= beta) return static_eval - RFP_BOUND * depth;

    if(!stack[ply].isPV && stack[ply].canDoNullMove && depth > 2 && !position.inCheck() && static_eval >= beta) {
        stack[ply+1].canDoNullMove = false;
        stack[ply+1].isPV = false;

        chess::Board newPosition = position;
        newPosition.makeMove(chess::Move(0, 0));
        nodes++;

        int value = -Negamax(newPosition, depth - 3, -beta, -beta + 1, ply + 1, stack, settings, TT, Hist, nodes);
        stack[ply].pv.moves.clear();
        stack[ply+1].pv.moves.clear();
        stack[ply+1].canDoNullMove = true;
        if(value >= beta) {
            return value;
        }
    }

    // Searches each move in turn
    MovePicker moves(position, entry.bestMove, stack[ply].killers, Hist);
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
                result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, stack, settings, TT, Hist, nodes);
            } else {
                result = Negamax(newPosition, depth, alpha, beta, ply + 1, stack, settings, TT, Hist, nodes);
            }
        } else {
            stack[ply+1].isPV = false;
            int reduce = reduction(depth, moves.curr, stack[ply].isPV, position.at(move.to()) != chess::Piece::None, position.inCheck());
            if(newPosition.sideToMove() != position.sideToMove()) {
                result = -Negamax(newPosition, depth - 1 - reduce, -alpha - 1, -alpha, ply + 1, stack, settings, TT, Hist, nodes);
            } else {
                result = Negamax(newPosition, depth - reduce, alpha, alpha+1, ply + 1, stack, settings, TT, Hist, nodes);
            }
            if(result > alpha && result < beta) {
                if(newPosition.sideToMove() != position.sideToMove()) {
                    result = -Negamax(newPosition, depth - 1, -alpha - 1, -alpha, ply + 1, stack, settings, TT, Hist, nodes);
                } else {
                    result = Negamax(newPosition, depth, alpha, alpha+1, ply + 1, stack, settings, TT, Hist, nodes);
                }
            }
            if(result > alpha && result < beta) {
                if(newPosition.sideToMove() != position.sideToMove()) {
                    result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, stack, settings, TT, Hist, nodes);
                } else {
                    result = Negamax(newPosition, depth, alpha, beta, ply + 1, stack, settings, TT, Hist, nodes);
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
        if(alpha >= beta) {
            if(position.at(move.to()) == chess::Piece::None) {
                Hist.update(position.sideToMove(), move, depth * depth);
                if(move != stack[ply].killers[0]) stack[ply].killers[1] = stack[ply].killers[0];
                stack[ply].killers[0] = move;
            }
            break;
        }
        // if(position.at(move.to()) == chess::Piece::None) {
        //     Hist.update(move, -1 * depth * depth);
        // }
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

        if(position.at(move.to()) != chess::Piece::None 
            && pokemon::lookupMoveEffectiveness(position.typeAt(move.from()), position.typeAt(move.to())) == pokemon::Effectiveness::Immune) continue;
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
    chess::legalcaptures(moves, position);
    chess::legalquiets(moves, position);

    // if(depth == 1) {
    //     nodes += moves.size();
    //     return;
    // }

    for(int i = 0; i < moves.size(); i++) {
        if(!chess::isMovePsuedoLegal(position, moves[i])) continue;
        chess::Board nPos = position;
        nPos.makeMove(moves[i]);
        perft(nPos, depth-1, nodes);
    }
}