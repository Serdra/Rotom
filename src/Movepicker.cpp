#include "includes.h"

int SEScore[] = {0, 36, 72, 108};
int targetScore[] = {0, 6, 12, 18, 24, 30};
int attackerScore[] = {5, 4, 3, 2, 1, 0};

int TTScore = 18'000;
int KillerScore = 2'500;
int16_t MaxHistory = 2'000;

struct History {
    int16_t score[64][64] = {0};

    void update(chess::Move move, int16_t bonus) {
        int clamped = std::clamp(bonus, (int16_t)-MaxHistory, MaxHistory);
        score[move.from()][move.to()] += clamped - score[move.from()][move.to()] * abs(clamped) / MaxHistory;
    }
};

int scoreCapture(chess::Move move, chess::Board &position) {
    return 
        attackerScore[(int)position.at(move.from()) % 6] + 
        targetScore[(int)position.at(move.to()) % 6] + 
        SEScore[(int)pokemon::lookupMoveEffectiveness(position.typeAt(move.from()), position.typeAt(move.to()))];
}

struct MovePicker {
    int curr = 0;
    chess::Movelist moves;

    MovePicker(chess::Board &b) {
        chess::legalcaptures(moves, b);
        for(int i = 0; i < moves.size(); i++) {
            if(b.at(moves[i].to()) == chess::Piece::None) moves[i].score = 0;
            else moves[i].score = scoreCapture(moves[i], b);
        }
    }

    MovePicker(chess::Board &b, chess::Move ttMove, chess::Move killers[2], History &Hist) {
        chess::legalmoves(moves, b);
        for(int i = 0; i < moves.size(); i++) {
            if(moves[i] == ttMove) moves[i].score = TTScore;
            else if(moves[i] == killers[0]) moves[i].score = KillerScore;
            else if(moves[i] == killers[1]) moves[i].score = KillerScore;
            else if(b.at(moves[i].to()) == chess::Piece::None) moves[i].score = Hist.score[moves[i].from()][moves[i].to()];
            else moves[i].score = 50 * scoreCapture(moves[i], b);
        }
    }

    bool next(chess::Move &move, chess::Board &b) {
        if(curr == moves.size()) return false;

        int bestMoveIdx = curr;

        for(int i = curr + 1; i < moves.size(); i++) {
            if(moves[i].score > moves[bestMoveIdx].score) bestMoveIdx = i;
        }

        move = moves[bestMoveIdx];
        std::swap(moves[curr], moves[bestMoveIdx]);
        curr++;
        return true;
    }

    bool nextCapture(chess::Move &move, chess::Board &b) {
        if(curr == moves.size()) return false;

        int bestMoveIdx = curr;

        for(int i = curr + 1; i < moves.size(); i++) {
            if(moves[i].score > moves[bestMoveIdx].score) bestMoveIdx = i;
        }

        move = moves[bestMoveIdx];
        std::swap(moves[curr], moves[bestMoveIdx]);
        curr++;
        return true;
    }
};