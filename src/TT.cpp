#include "includes.h"

enum TTFlags : uint8_t {
    Exact = 0,
    Upper,
    Lower
};

struct TTEntry {
    uint64_t hash = 0;
    chess::Move bestMove = chess::Move(0, 0);
    int16_t score = 0;
    uint8_t depth = 0;
    TTFlags flag = TTFlags::Exact;
};

struct TransTable {
   private:
    std::vector<TTEntry> table;
    uint32_t numEntries;
   public:
    TransTable(uint32_t s = 0) {
        numEntries = ((s * 1024) / sizeof(TTEntry)) * 1024;
        if(s != 0) {
            table.resize(numEntries);
        };
    }

    TTEntry probe(uint64_t hash) {
        return table[hash % numEntries];
    }

    void insert(TTEntry entry) {
        table[entry.hash % numEntries] = entry;
    }

    void clear() {
        for(int i = 0; i < numEntries; i++) {
            table[i] = TTEntry();
        }
    }
};