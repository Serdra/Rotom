#include "includes.h"

chess::Board generateStartingPosition(xorshift &rng) {
    while(true) {
        chess::Board b("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 -", true);

        for(int i = 0; i < NUM_STARTING_MOVES && b.isGameOver() == chess::GameResult::NONE; i++) {
            chess::Movelist moves;
            chess::legalmoves(moves, b);
            b.makeMove(moves[rng.rand() % moves.size()]);
        }
        if(b.isGameOver() == chess::GameResult::NONE) {
            if(rng.rand() % 2 == 0) b.makeMove(chess::Move(0, 0));
            return b;
        }
    }
}

PackedBoard packBoard(chess::Board &position) {
    PackedBoard ret;
    ret.occupied = position.all();
    chess::Bitboard occ = position.all();
    int i = 0;
    while(occ) {
        pokemon::Type pieceType = position.typeAt(chess::builtin::lsb(occ));
        chess::Piece piece = position.at(chess::builtin::poplsb(occ));
        ret.pieces[i] = (int)pieceType * 12 + (int)piece;
        i++;
    }
    if(position.sideToMove() == chess::Color::Black) ret.flags ^= 0b00000001;

    return ret;
}

void generateData(DataWriter &writer, std::mutex &mtx, int &interval, WDL &wdl, xorshift rng) {
    TransTable table(16);
    History hist;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<PackedBoard> gameData;
    gameData.reserve(300);

    chess::Board pos = generateStartingPosition(rng);
    std::pair<chess::Move, int> searchResult = IterativeDeepening(pos, StopType::Nodes, HARD_NODES, table, hist);
    while(abs(searchResult.second) > 200) {
        pos = generateStartingPosition(rng);
        searchResult = IterativeDeepening(pos, StopType::Nodes, HARD_NODES, table, hist);
    }

    // TODO: Add adjudication?
    while(true) {
        if(pos.isGameOver() != chess::GameResult::NONE || pos.fullMoveClock() > 400) {
            int8_t result = 0;

            if(pos.isGameOver() == chess::GameResult::WIN  && pos.sideToMove() == chess::Color::White) result = 1;
            if(pos.isGameOver() == chess::GameResult::WIN  && pos.sideToMove() == chess::Color::Black) result = -1;
            if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::White) result = -1;
            if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::Black) result = 1;

            for(int i = 0; i < gameData.size(); i++) {
                gameData[i].wdl = result;
            }

            mtx.lock();
            if(result == 1) wdl.whiteWins++;
            else if(result == -1) wdl.blackWins++;
            else wdl.draws++;

            writer.writeData(gameData);
            if(writer.positionsWritten >= (interval * 10000)) {
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                float u = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

                std::cout << "Positions generated: " << writer.positionsWritten << std::endl;
                std::cout << "Time Taken: " << u/1000 << "s" << std::endl;
                std::cout << "Speed: " << (writer.positionsWritten) / (u/1000) << "p/s" << std::endl;
                std::cout << "White wins: " << wdl.whiteWins << std::endl;
                std::cout << "Black wins: " << wdl.blackWins << std::endl;
                std::cout << "Draws: " << wdl.draws << std::endl << std::endl;
                interval++;
            }
            mtx.unlock();

            gameData.clear();

            pos = generateStartingPosition(rng);
            while(abs(searchResult.second) > 200) {
                pos = generateStartingPosition(rng);
                searchResult = IterativeDeepening(pos, StopType::Nodes, HARD_NODES, table, hist);
            }
        }

        searchResult = IterativeDeepening(pos, StopType::Nodes, HARD_NODES, table, hist);
        PackedBoard pb = packBoard(pos);
        if(abs(searchResult.second) < 10000 && pos.at(searchResult.first.to()) == chess::Piece::None) gameData.push_back(pb);
        pos.makeMove(searchResult.first);
    }
}