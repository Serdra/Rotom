#include "includes.h"

chess::Bitboard flip(chess::Bitboard x) {
    x = ((x >> 1) & 0x5555555555555555) | ((x & 0x5555555555555555) << 1);
    x = ((x >> 2) & 0x3333333333333333) | ((x & 0x3333333333333333) << 2);
    x = ((x >> 4) & 0x0f0f0f0f0f0f0f0f) | ((x & 0x0f0f0f0f0f0f0f0f) << 4);
    return x;
} 

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

PackedBoard reversePackBoard(chess::Board &position) {
    PackedBoard ret;
    ret.occupied = flip(position.all());
    int i = 0;
    while(ret.occupied) {
        int square = chess::builtin::poplsb(ret.occupied);
        square ^= 7;
        pokemon::Type pieceType = position.typeAt(square);
        chess::Piece piece = position.at(square);
        ret.pieces[i] = (int)pieceType * 12 + (int)piece;
        i++;
    }
    if(position.sideToMove() == chess::Color::Black) ret.flags ^= 0b00000001;
    ret.occupied = flip(position.all());
    return ret;
}

void generateData(DataWriter &writer, std::mutex &mtx, int &interval, WDL &wdl, xorshift rng) {
    TransTable table(16);
    History hist;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<PackedBoard> gameData;
    gameData.reserve(300);

    chess::Board pos = generateStartingPosition(rng);
    std::pair<chess::Move, int> searchResult;
    searchResult = IterativeDeepening(pos, StopType::Nodes, SOFT_NODES, HARD_NODES, table, hist);
    while(abs(searchResult.second) > 160) {
        chess::Board pos = generateStartingPosition(rng);
        searchResult = IterativeDeepening(pos, StopType::Nodes, SOFT_NODES, HARD_NODES, table, hist);
    }


    // TODO: Add adjudication?
    while(true) {
        if(pos.isGameOver() != chess::GameResult::NONE || pos.fullMoveClock() > 200) {
            int8_t result = 0;

            if(pos.isGameOver() == chess::GameResult::WIN  && pos.sideToMove() == chess::Color::White) result =  1;
            if(pos.isGameOver() == chess::GameResult::WIN  && pos.sideToMove() == chess::Color::Black) result = -1;
            if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::White) result = -1;
            if(pos.isGameOver() == chess::GameResult::LOSE && pos.sideToMove() == chess::Color::Black) result =  1;

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
            searchResult = IterativeDeepening(pos, StopType::Nodes, SOFT_NODES, HARD_NODES, table, hist);
            while(abs(searchResult.second) > 160) {
                chess::Board pos = generateStartingPosition(rng);
                searchResult = IterativeDeepening(pos, StopType::Nodes, SOFT_NODES, HARD_NODES, table, hist);
            }
        }

        searchResult = IterativeDeepening(pos, StopType::Nodes, SOFT_NODES, HARD_NODES, table, hist);
        PackedBoard pb = packBoard(pos);
        if(abs(searchResult.second) < 10000 && pos.at(searchResult.first.to()) == chess::Piece::None)  {
            pb.eval = searchResult.second;
            gameData.push_back(pb);
            if(pos.getCastlingRights().isEmpty()) {
                pb = reversePackBoard(pos);
                pb.eval = searchResult.second;
                gameData.push_back(pb);
            }
        }
        pos.makeMove(searchResult.first);
    }
}

void generateBook(std::mutex &mtx, xorshift rng) {
    TransTable TT(128);
    History Hist;
    while(true) {
        int randomMoves = (rng.rand() % 7) + 4;

        while(true) {
            TT.clear();
            chess::Board pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 -", true);
            for(int i = 0; i < randomMoves && pos.isGameOver() == chess::GameResult::NONE; i++) {
                chess::Movelist moves;
                chess::legalmoves(moves, pos);
                pos.makeMove(moves[rng.rand() % moves.size()]);
            }
            if(pos.isGameOver() != chess::GameResult::NONE) continue;
            std::pair<chess::Move, int> result = IterativeDeepening(pos, StopType::Time, 1750, 2250, TT, Hist);
            if(abs(result.second) < 150) {
                mtx.lock();
                std::cout << pos.getFen() << std::endl;
                mtx.unlock();
                break;
            }
        }
    }
}