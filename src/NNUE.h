#pragma once
#include "includes.h"

int pieceSwap[12] = {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5};

namespace nnue {
    const int size = 32;

    int16_t hiddenWeights[13824][size];
    int16_t hiddenBias[size];

    int16_t outputWeights[size * 2];
    int16_t outputBias;

    struct Accumulator {
        int16_t white[size];
        int16_t black[size];

        Accumulator() {
            memcpy(white, hiddenBias, sizeof(int16_t) * size);
            memcpy(black, hiddenBias, sizeof(int16_t) * size);
        }

        void clear() {
            memcpy(white, hiddenBias, sizeof(int16_t) * size);
            memcpy(black, hiddenBias, sizeof(int16_t) * size);
        }

        void sub_(int type, int piece, int sq) {
            assert(type < 18);
            assert(piece < 12);
            assert(sq < 64);
            for(int i = 0; i < size; i++) {
                white[i] -= hiddenWeights[type * 768 + piece * 64 + sq][i];
            }
            sq ^= 56;
            piece = (piece + 6) % 12;
            for(int i = 0; i < size; i++) {
                black[i] -= hiddenWeights[type * 768 + piece * 64 + sq][i];
            }
        }

        void add_(int type, int piece, int sq) {
            assert(type < 18);
            assert(piece < 12);
            assert(sq < 64);
            for(int i = 0; i < size; i++) {
                white[i] += hiddenWeights[type * 768 + piece * 64 + sq][i];
            }
            sq ^= 56;
            piece = (piece + 6) % 12;
            for(int i = 0; i < size; i++) {
                black[i] += hiddenWeights[type * 768 + piece * 64 + sq][i];
            }
        }

        void addsub(int add, int sub, Accumulator &source) {
            for(int i = 0; i < size; i++) {
                white[i] = source.white[i] + hiddenWeights[add][i] - hiddenWeights[sub][i];
            }

            int type_add = add / 768;
            int sq_add = add % 64;
            int piece_add = (add / 64) % 12;
            sq_add ^= 56;
            piece_add = (piece_add + 6) % 12;
            add = type_add * 768 + piece_add * 64 + sq_add;

            int type_sub = sub / 768;
            int sq_sub = sub % 64;
            int piece_sub = (sub / 64) % 12;
            sq_sub ^= 56;
            piece_sub = (piece_sub + 6) % 12;
            sub = type_sub * 768 + piece_sub * 64 + sq_sub;

            for(int i = 0; i < size; i++) {
                black[i] = source.black[i] + hiddenWeights[add][i] - hiddenWeights[sub][i];
            }
        }

        void addsubsub(int add, int sub1, int sub2, Accumulator &source) {
            for(int i = 0; i < size; i++) {
                white[i] = source.white[i] + hiddenWeights[add][i] - hiddenWeights[sub1][i] - hiddenWeights[sub2][i];
            }

            int type_add = add / 768;
            int sq_add = add % 64;
            int piece_add = (add / 64) % 12;
            sq_add ^= 56;
            piece_add = (piece_add + 6) % 12;
            add = type_add * 768 + piece_add * 64 + sq_add;

            int type_sub1 = sub1 / 768;
            int sq_sub1 = sub1 % 64;
            int piece_sub1 = (sub1 / 64) % 12;
            sq_sub1 ^= 56;
            piece_sub1 = (piece_sub1 + 6) % 12;
            sub1 = type_sub1 * 768 + piece_sub1 * 64 + sq_sub1;

            int type_sub2 = sub2 / 768;
            int sq_sub2 = sub2 % 64;
            int piece_sub2 = (sub2 / 64) % 12;
            sq_sub2 ^= 56;
            piece_sub2 = (piece_sub2 + 6) % 12;
            sub2 = type_sub2 * 768 + piece_sub2 * 64 + sq_sub2;

            for(int i = 0; i < size; i++) {
                black[i] = source.black[i] + hiddenWeights[add][i] - hiddenWeights[sub1][i] - hiddenWeights[sub2][i];
            }
        }

        void addaddsubsub(int add1, int add2, int sub1, int sub2, Accumulator &source) {
            for(int i = 0; i < size; i++) {
                white[i] = source.white[i] + hiddenWeights[add2][i] + hiddenWeights[add1][i] - hiddenWeights[sub1][i] - hiddenWeights[sub2][i];
            }

            int type_add1 = add1 / 768;
            int sq_add1 = add1 % 64;
            int piece_add1 = (add1 / 64) % 12;
            sq_add1 ^= 56;
            piece_add1 = (piece_add1 + 6) % 12;
            add1 = type_add1 * 768 + piece_add1 * 64 + sq_add1;

            int type_add2 = add2 / 768;
            int sq_add2 = add2 % 64;
            int piece_add2 = (add2 / 64) % 12;
            sq_add2 ^= 56;
            piece_add2 = (piece_add2 + 6) % 12;
            add2 = type_add2 * 768 + piece_add2 * 64 + sq_add2;

            int type_sub1 = sub1 / 768;
            int sq_sub1 = sub1 % 64;
            int piece_sub1 = (sub1 / 64) % 12;
            sq_sub1 ^= 56;
            piece_sub1 = (piece_sub1 + 6) % 12;
            sub1 = type_sub1 * 768 + piece_sub1 * 64 + sq_sub1;

            int type_sub2 = sub2 / 768;
            int sq_sub2 = sub2 % 64;
            int piece_sub2 = (sub2 / 64) % 12;
            sq_sub2 ^= 56;
            piece_sub2 = (piece_sub2 + 6) % 12;
            sub2 = type_sub2 * 768 + piece_sub2 * 64 + sq_sub2;

            for(int i = 0; i < size; i++) {
                black[i] = source.black[i] + hiddenWeights[add1][i] + hiddenWeights[add2][i] - hiddenWeights[sub1][i] - hiddenWeights[sub2][i];
            }
        }

        int relu(int16_t x) {
            return std::max(x, (int16_t)0);
        }

        int eval(bool isWhite) {
            int value = outputBias;

            if(isWhite) {
                for(int i = 0; i < size; i++) {
                    value += relu(white[i]) * outputWeights[i];
                }
                for(int i = 0; i < size; i++) {
                    value += relu(black[i]) * outputWeights[size + i];
                }
            } else {
                for(int i = 0; i < size; i++) {
                    value += relu(black[i]) * outputWeights[i];
                }
                for(int i = 0; i < size; i++) {
                    value += relu(white[i]) * outputWeights[size + i];
                }
            }
            return (value * 200) / (128 * 128);
        }

        bool operator!=(Accumulator &rhs) const {
            bool eq = true;
            for(int i = 0; i < size; i++) {
                if(rhs.white[i] != white[i]) eq = false;
                if(rhs.black[i] != black[i]) eq = false;
            };
            return !eq;
        }
    };

    void init(std::string fileName) {
        float temp;
        std::ifstream inFile(fileName, std::ios::in | std::ios::binary);

        for(int i = 0; i < 13824; i++) {
            for(int j = 0; j < size; j++) {
                inFile.read((char*) &temp, sizeof(temp));
                hiddenWeights[i][j] = std::round(128 * temp);
            }
        }

        for(int i = 0; i < size; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            hiddenBias[i] = std::round(128 * temp);
        }

        for(int i = 0; i < size*2; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            outputWeights[i] = std::round(128 * temp);
        }

        inFile.read((char*) &temp, sizeof(temp));
        outputBias = std::round(128 * 128 * temp);
    }
};