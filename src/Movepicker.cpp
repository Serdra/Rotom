#include "includes.h"

int Quiet_Score = 35;

int SEScore[] = {0, 36, 72, 108};
int targetScore[] = {0, 6, 12, 18, 24, 30};
int attackerScore[] = {5, 4, 3, 2, 1, 0};

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
        chess::legalmoves(moves, b);
        for(int i = 0; i < moves.size(); i++) {
            if(b.at(moves[i].to()) == chess::Piece::None) moves[i].score = Quiet_Score;
            else moves[i].score = scoreCapture(moves[i], b);
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
};