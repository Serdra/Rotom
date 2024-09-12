#pragma once
#include "includes.h"

int pieceSwap[12] = {6, 7, 8, 9, 10, 11, 0, 1, 2, 3, 4, 5};

namespace nnue {
    const int size = 16;

    float hiddenWeights[13824][size];
    float hiddenBias[size];

    float outputWeights[size * 2];
    float outputBias;

    struct Accumulator {
        float white[size];
        float black[size];

        Accumulator() {
            memcpy(white, hiddenBias, sizeof(float) * size);
            memcpy(black, hiddenBias, sizeof(float) * size);
        }

        void clear() {
            memcpy(white, hiddenBias, sizeof(float) * size);
            memcpy(black, hiddenBias, sizeof(float) * size);
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

        float relu(float x) {
            return std::max(x, (float)0);
        }

        float eval(bool isWhite) {
            float value = outputBias;

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
                hiddenWeights[i][j] =  (128 * temp);
            }
        }

        for(int i = 0; i < size; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            hiddenBias[i] =  (128 * temp);
        }

        for(int i = 0; i < size*2; i++) {
            inFile.read((char*) &temp, sizeof(temp));
            outputWeights[i] =  (128 * temp);
        }

        inFile.read((char*) &temp, sizeof(temp));
        outputBias =  (128 * 128 * temp);
    }
};