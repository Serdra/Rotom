#include "includes.h"

int SEScore[] = {0, 36, 72, 108};
int targetScore[] = {0, 6, 12, 18, 24, 30};
int attackerScore[] = {5, 4, 3, 2, 1, 0};

int16_t TTScore = 30'000;
int16_t KillerScore = 4'000;
int16_t MaxHistory = 3'000;
int16_t MaxContHistory = 2'000;

struct History {
    int16_t score[2][64][64] = {0};

    void update(chess::Color stm, chess::Move move, int16_t bonus) {
        score[(int)stm][move.from()][move.to()] += bonus - score[(int)stm][move.from()][move.to()] * abs(bonus) / MaxHistory;
    }

    void gravity() {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 64; j++) {
                for(int k = 0; k < 64; k++) {
                    score[i][j][k] /= 10;
                }
            }
        }
    }
    void clear() {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 64; j++) {
                for(int k = 0; k < 64; k++) {
                    score[i][j][k] = 0;
                }
            }
        }
    }
};

struct ContHistory {
    int16_t score[2][6][64][6][64] = {0};
    void update(chess::Color stm, int prevPiece, int prevTarget, int piece, int target, int16_t bonus) {
        score[(int)stm][prevPiece][prevTarget][piece][target] += bonus - score[(int)stm][prevPiece][prevTarget][piece][target] * abs(bonus) / MaxContHistory;
    }
    void gravity() {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 6; j++) {
                for(int k = 0; k < 64; k++) {
                    for(int l = 0; l < 6; l++) {
                        for(int m = 0; m < 64; m++) {
                            score[i][j][k][l][m] /= 10;
                        }
                    }
                }
            }
        }
    }
    void clear() {
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 6; j++) {
                for(int k = 0; k < 64; k++) {
                    for(int l = 0; l < 6; l++) {
                        for(int m = 0; m < 64; m++) {
                            score[i][j][k][l][m] = 0;
                        }
                    }
                }
            }
        }
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

    MovePicker(chess::Board &b, chess::Move ttMove, chess::Move killers[2], History &Hist, ContHistory &ContHist, int piece, int square) {
        chess::legalmoves(moves, b);
        for(int i = 0; i < moves.size(); i++) {
            if(moves[i] == ttMove) moves[i].score = TTScore;
            else if(moves[i] == killers[0]) moves[i].score = KillerScore;
            else if(moves[i] == killers[1]) moves[i].score = KillerScore;
                        else if(b.at(moves[i].to()) == chess::Piece::None) {
                moves[i].score = Hist.score[(int)b.sideToMove()][moves[i].from()][moves[i].to()];
                moves[i].score += ContHist.score[(int)b.sideToMove()][piece][square][(int)b.at(moves[i].from()) % 6][moves[i].to()];
            }
            else moves[i].score = KillerScore + 20 * scoreCapture(moves[i], b);
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