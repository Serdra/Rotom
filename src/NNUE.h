#pragma once
#include "includes.h"

int pieceSwap[12] = {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5};

namespace nnue {
    const int size = 32;
    const int Q1 = 181;
    const int Q2 = 255; 

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

        void sub(int type, int piece, int sq) {
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

        void add(int type, int piece, int sq) {
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

        int screlu(int16_t x) {
            return std::clamp(x, (int16_t)0, (int16_t)Q1) * std::clamp(x, (int16_t)0, (int16_t)Q1);
        }

        int eval(bool isWhite) {
            int value = outputBias;

            if(isWhite) {
                for(int i = 0; i < size; i++) {
                    value += screlu(white[i]) * outputWeights[i];
                }
                for(int i = 0; i < size; i++) {
                    value += screlu(black[i]) * outputWeights[size + i];
                }
            } else {
                for(int i = 0; i < size; i++) {
                    value += screlu(black[i]) * outputWeights[i];
                }
                for(int i = 0; i < size; i++) {
                    value += screlu(white[i]) * outputWeights[size + i];
                }
            }
            // Careful order of operation to not lose precision or overflow
            return 200 * (value / (Q1 * Q1)) / Q2;
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
                hiddenWeights[i][j] = std::round(Q1 * temp);
            }
        }

        for(int i = 0; i < size; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            hiddenBias[i] = std::round(Q1 * temp);
        }

        for(int i = 0; i < size*2; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            outputWeights[i] = std::round(Q2 * temp);
        }

        inFile.read((char*) &temp, sizeof(temp));
        outputBias = std::round(Q1 * Q1 * Q2 * temp);
    }
};