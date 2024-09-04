#pragma once
#include "includes.h"

const int MAX_DEPTH = 64;
const int MAX_PLY = 256;
const int INF = 999999;
const int MATE = 10256;

const float SOFT_LIMIT_MULT = 0.68;
const float HARD_LIMIT_MULT = 1.32;

const float LMR_DIV = 3.5;
const float LMR_BASE = 0.20;

bool printSearchUpdates = true;

// Reasons to terminate the search
enum StopType {
    Infinite,
    Time,
    Nodes,
    Depth
};

struct PV {
    std::vector<chess::Move> moves;
};

struct SearchSettings {
    StopType stop;
    int nodes; // If stop == StopType::Nodes, the number of nodes to stop at
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool timeout = false; 
};

struct Stack {
    bool isPV = true;
    bool canDoNullMove = true;
    PV pv;
    chess::Move killers[2];
};

std::pair<chess::Move, int> IterativeDeepening(
    chess::Board position, StopType stop, int stopValue, TransTable &TT, History &Hist
);

int Negamax(
    chess::Board &position, int depth, int alpha, int beta, int ply, Stack* stack, SearchSettings &settings, TransTable &TT, History &Hist, uint64_t &nodes
);

int QSearch(
    chess::Board &position, int alpha, int beta, int ply, uint64_t &nodes
);

int reduction(int depth, int moveNum) {
    if(moveNum < 4 || depth < 3) return 0;
    return std::min(depth - 2, (int)round(log(depth) * log(moveNum) / LMR_DIV + LMR_BASE));
}


void perft(chess::Board &position, int depth, uint64_t &nodes);