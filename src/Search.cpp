#include "includes.h"

chess::Move IterativeDeepening(chess::Board position, StopType stop, int stopValue) {
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

    while(true) {
        PV newPV;
        
        int result = Negamax(position, depth, 0, -INF, INF, newPV, settings, nodes);

        if(!settings.timeout) {
            bestPV = newPV;
            depth++;

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
    }

    return bestPV.moves[0];
}

int Negamax(chess::Board &position, int depth, int alpha, int beta, int ply, PV &pv, SearchSettings &settings, uint64_t &nodes) {
    // Step one is to check if the game is over. This is fast and perfectly accurate so we do it first
    if(position.isGameOver() != chess::GameResult::NONE) {
        if(position.isGameOver() == chess::GameResult::WIN) return MATE - ply;
        if(position.isGameOver() == chess::GameResult::LOSE) return -(MATE - ply);
        return 0;
    }

    // If the position is not game over, we check the base case (this is a recursive algorithm after all)
    if(depth <= 0 || ply > MAX_PLY) return eval(position);

    // Next we check if the time is up
    if(settings.stop == StopType::Time && std::chrono::high_resolution_clock::now() >= settings.endTime) {
        settings.timeout = true;
        return 0;
    } 
    if(settings.stop == StopType::Nodes && nodes >= settings.nodes) {
        settings.timeout = true;
        return 0;
    }

    chess::Movelist Moves;
    chess::legalmoves(Moves, position);

    int result;
    int bestMoveValue = -INF;

    for(int i = 0; i < Moves.size(); i++) {

        // We create a new PV from this position so that only the best PV is continued
        PV newPV;

        // I prefer copy-make to make-unmake
        chess::Board newPosition = position;
        newPosition.makeMove(Moves[i]);

        nodes++;

        if(newPosition.sideToMove() != position.sideToMove()) {
            result = -Negamax(newPosition, depth - 1, -beta, -alpha, ply + 1, newPV, settings, nodes);
        } else {
            result = Negamax(newPosition, depth - 1, alpha, beta, ply + 1, newPV, settings, nodes);
        }

        if(settings.timeout) return 0;

        if(result > bestMoveValue) {
            // Update the PV
            pv.moves.clear();
            pv.moves.push_back(Moves[i]);

            // If there is a continuation after the bestmove, update it
            if(newPV.moves.size() != 0) {
                pv.moves.insert(pv.moves.end(), newPV.moves.begin(), newPV.moves.end());
            }
            bestMoveValue = result;
            alpha = std::max(alpha, bestMoveValue);
            if(alpha >= beta) break;
        }
    }

    return bestMoveValue;
}