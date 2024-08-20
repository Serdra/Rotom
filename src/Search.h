#pragma once
#include "includes.h"

const int MAX_DEPTH = 64;
const int MAX_PLY = 256;
const int INF = 999999;
const int MATE = 10256;

const float SOFT_LIMIT_MULT = 0.68;
const float HARD_LIMIT_MULT = 1.32;

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

std::pair<chess::Move, int> IterativeDeepening(
    chess::Board position, StopType stop, int stopValue, TransTable &TT
);

int Negamax(
    chess::Board &position, int depth, int alpha, int beta, int ply, bool isPV, PV &pv, SearchSettings &settings, TransTable &TT, uint64_t &nodes
);

int QSearch(
    chess::Board &position, int alpha, int beta, int ply, uint64_t &nodes
);