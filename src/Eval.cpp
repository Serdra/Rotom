#include "includes.h"

void buildPST() {
    for(int i = 384; i < 768; i++) {
        int square = i % 64;
        int piece = (i - 384 - square) / 64;
        square ^= 56;
        int feature = (piece * 64) + square;
        PST_mg[i] = -PST_mg[feature];
        PST_eg[i] = -PST_eg[feature];
    }
}

int eval(chess::Board &position) {
    int score_mg = 0;
    int score_eg = 0;
    int phase = 0;
    chess::Bitboard occupied = position.all();

    while(occupied) {
        int square = chess::builtin::poplsb(occupied);
        int piece = (int)position.at(square);

        if(position.sideToMove() == chess::Color::Black) {
            square ^= 56;
            piece = pieceSwap[piece];
        }

        phase += phaseScore[piece % 6];
        score_mg += PST_mg[piece * 64 + square];
        score_eg += PST_eg[piece * 64 + square];
    }

    return ((score_mg * phase) + (score_eg * (24 - phase))) / 24 + (position.hash() % 15 - 7);
}