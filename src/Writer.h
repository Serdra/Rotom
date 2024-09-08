#pragma once
#include "includes.h"

// TODO: Add more flags or condense wdl and flags
struct PackedBoard {
    // Pieces are not stm relative
    chess::Bitboard occupied = 0;

    // piece / 12 = pokemon type
    // piece % 12 = piece type
    uint8_t pieces[16] = {0}; 

    // Eval is not stm relative
    int16_t eval = 0;

    // 0 if draw, 1 if white win, -1 if black win
    int8_t wdl = 0;

    // 0b00000001 stm, 0 if white 1 if black
    // 0b00000010 in check
    uint8_t flags = 0; 
};

// Not thread safe
class DataWriter {
   private:
    std::ofstream outFile;

   public:
    uint64_t positionsWritten = 0;

    DataWriter(std::string fileName) {
        outFile = std::ofstream(fileName, std::ios::out | std::ios::binary);
    }

    void writeData(std::vector<PackedBoard> vec) {
        for(int i = 0; i < vec.size(); i++) {
            outFile.write(reinterpret_cast<char*>(&vec[i]), sizeof(vec[i]));
            positionsWritten++;
        }
    }

};