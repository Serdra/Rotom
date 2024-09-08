#pragma once
#include "includes.h"

const int MAX_DEPTH = 64;
const int MAX_PLY = 256;
const int INF = 999999;
const int MATE = 10256;

float SOFT_LIMIT_MULT = 0.703;
float HARD_LIMIT_MULT = 1.331;

float LMR_DIV = 3.299;
float LMR_BASE = 0.211;

int LMR_MIN_DEPTH = 4;
int LMR_MIN_MOVES = 3;

float LMR_PV = -0.2;
float LMR_CAP = -0.3;
float LMR_CHECK = -0.2;

int RFP_BOUND = 80;

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

int reduction(int depth, int moveNum, bool isPV, bool isCap, bool inCheck) {
    if(moveNum < LMR_MIN_MOVES || depth < LMR_MIN_DEPTH) return 0;
    float reduce = log(depth) * log(moveNum) / LMR_DIV + LMR_BASE;
    // if(isPV) reduce += LMR_PV;
    // if(isCap) reduce += LMR_CAP;
    // if(inCheck) reduce += LMR_CHECK;
    
    return std::clamp((int)round(reduce), 0, depth - 2);
}


void perft(chess::Board &position, int depth, uint64_t &nodes);