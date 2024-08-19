#include "includes.h"

std::pair<chess::Move, int> IterativeDeepening(chess::Board position, StopType stop, int stopValue, TransTable &TT) {
    auto start = std::chrono::high_resolution_clock::now();

    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    endTime = start + std::chrono::milliseconds(stopValue);

    SearchSettings settings;

    settings.stop = stop;
    settings.nodes = stopValue;
    settings.endTime = endTime;
    settings.timeout = false;

    int depth = 1;
    uint64_t nodes = 0;

    PV bestPV;
    int evaluation;

    while(true) {
        PV newPV;
        
        int result = Negamax(position, depth, -INF, INF, 0, newPV, settings, TT, nodes);

        if(!settings.timeout) {
            bestPV = newPV;
            evaluation = result;

            if(printSearchUpdates) {
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                int u = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

                std::cout << "info depth " << depth << " score cp " << result;
                std::cout << " time " << u << " nodes " << nodes;
                if(u >= 2) std::cout << " nps " << (int)(nodes / (u / 1000.f));
                std::cout << " pv ";
                for(auto move : bestPV.moves) std::cout << move << " ";
                std::cout << std::endl;
            }
        }

        if(settings.timeout) break;
        
        if(stop == StopType::Time && std::chrono::high_resolution_clock::now() >= endTime) break;
        if(stop == StopType::Nodes && nodes >= stopValue) break;
        if(stop == StopType::Depth && depth >= stopValue) break;
        if(depth > MAX_DEPTH) break;

        depth++;
    }

    return {bestPV.moves[0], evaluation};
}

int Negamax(chess::Board &position, int depth, int alpha, int beta, int ply, PV &pv, SearchSettings &settings, TransTable &TT, uint64_t &nodes) {
    // Step one is to check if the game is over. This is fast and perfectly accurate so we do it first
    if(position.isGameOver() != chess::GameResult::NONE) {
        if(position.isGameOver() == chess::GameResult::WIN) return MATE - ply;
        if(position.isGameOver() == chess::GameResult::LOSE) return -(MATE - ply);
        return 0;
    }

    // If the position is not game over, we check the base case (this is a recursive algorithm after all)
    if(depth <= 0 || ply > MAX_PLY) return QSearch(position, alpha, beta, ply, nodes);

    // Next we check if the time is up
    if(settings.stop == StopType::Time && std::chrono::high_resolution_clock::now() >= settings.endTime) {
        settings.timeout = true;
        return 0;
    } 
    if(settings.stop == StopType::Nodes && nodes >= settings.nodes) {
        settings.timeout = true;
        return 0;
    }

    TTEntry entry = TT.probe(position.hash());

    // Searches each move in turn
    MovePicker moves(position, entry.bestMove);
    chess::Move move;

    int result;
    int bestMoveValue = -INF;

    while(moves.next(move, position)) {

        // We create a new PV from this position so that only the best PV is continued
        PV newPV;

        // I prefer copy-make to make-unmake
        chess::Board newPosition = position;
        newPosition.makeMove(move);

        nodes++;

        // We only do a full width search on the first move by default
        // All other moves are assumed to be significantly worse and we do a zero width search instead
        // If that search fails high, then we do a full width search
        if(moves.curr == 1) {
            if(newPosition.sideToMove() != position.sideToMove()) {
                result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, newPV, settings, TT, nodes);
            } else {
                result = Negamax(newPosition, depth - 1, alpha, beta, ply + 1, newPV, settings, TT, nodes);
            }
        } else {
            if(newPosition.sideToMove() != position.sideToMove()) {
                result = -Negamax(newPosition, depth - 1, -alpha - 1, -alpha, ply + 1, newPV, settings, TT, nodes);
            } else {
                result = Negamax(newPosition, depth - 1, alpha, alpha+1, ply + 1, newPV, settings, TT, nodes);
            }
            if(result > alpha && result < beta) {
                if(newPosition.sideToMove() != position.sideToMove()) {
                    result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, newPV, settings, TT, nodes);
                } else {
                    result = Negamax(newPosition, depth - 1, alpha, beta, ply + 1, newPV, settings, TT, nodes);
                }
            }
        }
        if(settings.timeout) return 0;

        if(result > bestMoveValue) {
            // Update the PV
            pv.moves.clear();
            pv.moves.push_back(move);

            // If there is a continuation after the bestmove, update it
            if(newPV.moves.size() != 0) {
                pv.moves.insert(pv.moves.end(), newPV.moves.begin(), newPV.moves.end());
            }
            bestMoveValue = result;
        }
        alpha = std::max(alpha, bestMoveValue);
        if(alpha >= beta) break;
    }
    
    entry.hash = position.hash();
    if(pv.moves.size() != 0) {
        entry.bestMove = pv.moves[0];
        TT.insert(entry);
    }
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