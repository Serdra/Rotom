#pragma once
#include "includes.h"

const int NUM_STARTING_MOVES = 6;
const int SOFT_NODES = 15000;
const int HARD_NODES = 50000;

struct WDL {
    uint64_t whiteWins = 0;
    uint64_t blackWins = 0;
    uint64_t draws = 0;
};

chess::Board generateStartingPosition(xorshift &rng);

PackedBoard packBoard(chess::Board &position);

void generateData(DataWriter &writer, std::mutex &mtx, int &interval, WDL &wdl, xorshift rng);