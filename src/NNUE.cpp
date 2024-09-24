#pragma once
#include "includes.h"

void fromScratch(nnue::Accumulator &acc, chess::Board &position) {
    acc.clear();
    chess::Bitboard occ = position.all();
    while(occ) {
        int square = chess::builtin::poplsb(occ);
        int piece = (int)position.at(square);
        int type = (int)position.typeAt(square);
        acc.add_(type, piece, square);
    }
}