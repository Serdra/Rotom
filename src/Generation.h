#pragma once
#include "includes.h"

const int NUM_STARTING_MOVES = 6;
const int SOFT_NODES = 15000;
const int HARD_NODES = 100000;

chess::Board generateStartingPosition(xorshift &rng);

PackedBoard packBoard(chess::Board &position);

void generateData(DataWriter &writer, std::mutex &mtx, int &interval, xorshift rng);