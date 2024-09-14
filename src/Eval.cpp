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
    nnue::Accumulator acc;
    fromScratch(acc, position);
    return acc.eval(position.sideToMove() == chess::Color::White);
}