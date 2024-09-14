// I basically stole this entire thing from Disservin's library. Idk shit about licensing so lmk if I did it wrong
// https://github.com/Disservin/chess-library 
#pragma once

#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <array>

namespace pokechess {

namespace pokemon {

enum class Type : int8_t {
    Normal = 0,
    Fire,
    Water,
    Electric,
    Grass,
    Ice,
    Fighting,
    Poison,
    Ground,
    Flying,
    Psychic,
    Bug,
    Rock,
    Ghost,
    Dragon,
    Dark,
    Steel,
    Fairy,
    None
};

enum class Effectiveness : int8_t {
    Immune = 0,
    Resist,
    Neutral,
    SuperEffective
};

std::string typeToStringArray[19] = {
    "NRM",
    "FIR",
    "WTR",
    "ELC",
    "GRS",
    "ICE",
    "FGT",
    "PSN",
    "GRD",
    "FLY",
    "PSY",
    "BUG",
    "RCK",
    "GHT",
    "DRG",
    "DRK",
    "STL",
    "FRY",
    "..."
};

char typeToCharArray[18] = {
    'm',
    'f',
    'w',
    'e',
    'g',
    'i',
    'h',
    'z',
    'o',
    'l',
    's',
    'u',
    'c',
    't',
    'a',
    'd',
    'x',
    'y'
};
// Pokemon type chart
// From https://pokemondb.net/type
int8_t typeEffectivenessChart[18][18] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 2, 2, 1, 2},
    {2, 1, 1, 2, 3, 3, 2, 2, 2, 2, 2, 3, 1, 2, 1, 2, 3, 2},
    {2, 3, 1, 2, 1, 2, 2, 2, 3, 2, 2, 2, 3, 2, 1, 2, 2, 2},
    {2, 2, 3, 1, 1, 2, 2, 2, 0, 3, 2, 2, 2, 2, 1, 2, 2, 2},
    {2, 1, 3, 2, 1, 2, 2, 1, 3, 1, 2, 1, 3, 2, 1, 2, 1, 2},
    {2, 1, 1, 2, 3, 1, 2, 2, 3, 3, 2, 2, 2, 2, 3, 2, 1, 2},
    {3, 2, 2, 2, 2, 3, 2, 1, 2, 1, 1, 1, 3, 0, 2, 3, 3, 1},
    {2, 2, 2, 2, 3, 2, 2, 1, 1, 2, 2, 2, 1, 1, 2, 2, 0, 3},
    {2, 3, 2, 3, 1, 2, 2, 3, 2, 0, 2, 1, 3, 2, 2, 2, 3, 2},
    {2, 2, 2, 1, 3, 2, 3, 2, 2, 2, 2, 3, 1, 2, 2, 2, 1, 2},
    {2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 1, 2, 2, 2, 2, 0, 1, 2},
    {2, 1, 2, 2, 3, 2, 1, 1, 2, 1, 3, 2, 2, 1, 2, 3, 1, 1},
    {2, 3, 2, 2, 2, 3, 1, 2, 1, 3, 2, 3, 2, 2, 2, 2, 1, 2},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 3, 2, 1, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 0},
    {2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 3, 2, 2, 3, 2, 1, 2, 1},
    {2, 1, 1, 1, 2, 3, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 1, 3},
    {2, 1, 2, 2, 2, 2, 3, 1, 2, 2, 2, 2, 2, 2, 3, 3, 1, 2},
};

// Convert this to an index?
std::string typeToString(Type type) {
    return typeToStringArray[(int)type];
}

char typeToChar(Type type) {
    return typeToCharArray[(int)type];
}

Effectiveness lookupMoveEffectiveness(Type Attacking, Type Defending) {
    assert(Attacking != Type::None && Defending != Type::None);
    return (Effectiveness)typeEffectivenessChart[(int)Attacking][(int)Defending];
}
}; // Namespace pokemon

namespace chess {

typedef uint64_t Bitboard;
typedef uint8_t Square;
typedef uint8_t Rank;
typedef uint8_t File;

enum class PieceType : uint8_t { 
    Pawn, 
    Knight, 
    Bishop, 
    Rook, 
    Queen, 
    King, 
    None
};

enum class Piece : uint8_t {
    WhitePawn,
    WhiteKnight,
    WhiteBishop,
    WhiteRook,
    WhiteQueen,
    WhiteKing,
    BlackPawn,
    BlackKnight,
    BlackBishop,
    BlackRook,
    BlackQueen,
    BlackKing,
    None
};

enum class Color : int8_t {
    White, 
    Black
};


enum class CastleSide : uint8_t { KING_SIDE, QUEEN_SIDE };

enum class GameResult { WIN, LOSE, DRAW, NONE };

std::string squareToString[64] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

std::string squareToStringArray[64] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};

char pieceToCharArray[13] = {
    'P',
    'N',
    'B',
    'R',
    'Q',
    'K',
    'p',
    'n',
    'b',
    'r',
    'q',
    'k',
    '.'
};

constexpr Piece charToPiece(char c) {
    switch (c) {
        case 'P':
            return Piece::WhitePawn;
        case 'N':
            return Piece::WhiteKnight;
        case 'B':
            return Piece::WhiteBishop;
        case 'R':
            return Piece::WhiteRook;
        case 'Q':
            return Piece::WhiteQueen;
        case 'K':
            return Piece::WhiteKing;
        case 'p':
            return Piece::BlackPawn;
        case 'n':
            return Piece::BlackKnight;
        case 'b':
            return Piece::BlackBishop;
        case 'r':
            return Piece::BlackRook;
        case 'q':
            return Piece::BlackQueen;
        case 'k':
            return Piece::BlackKing;
        case '.':
            return Piece::None;
        default:
            break;
    }
    return Piece::None;
}

struct Move {
    uint16_t data = 0;
    uint16_t score = 0;

    Move() {}

    Move(uint16_t move) { data = move; }

    Move(Square source, Square target, PieceType promotion=PieceType::Knight, uint16_t flags=0) {
        data = (flags) | (((int)promotion - (int)PieceType::Knight) << 12);
        data |= (target << 6) | (source);
    }

    constexpr Square from() const {
        return data & 0x3f;
    }

    constexpr Square to() const {
        return (data >> 6) & 0x3f;
    }
    
    constexpr uint16_t typeOf() const {
        return (data & (3 << 14));
    }

    PieceType promotionType() const {
        return (PieceType)(((data >> 12) & 3) + (int)PieceType::Knight);
    }

    [[nodiscard]] constexpr uint16_t move() const { return data; }

    bool operator==(const Move &rhs) const { return data == rhs.data; }
    bool operator!=(const Move &rhs) const { return data != rhs.data; }

    static constexpr uint16_t Normal = 0;
    static constexpr uint16_t EnPassant = 1 << 14;
    static constexpr uint16_t Promotion = 2 << 14;
    static constexpr uint16_t Castling = 3 << 14;

    friend std::ostream &operator<<(std::ostream &os, const Move &move);
};

std::string moveToUGI(const Move& move) {
    Square from_sq = move.from();
    Square to_sq = move.to();
    
    if (from_sq == to_sq) {
        return "0000";
    }
    
    std::string result = squareToStringArray[from_sq] + squareToStringArray[to_sq];
    
    if (move.typeOf() == Move::Promotion) {
        result += pieceToCharArray[(int)move.promotionType() + 6];
    }
    
    return result;
}


std::ostream &operator<<(std::ostream &os, const Move &move) {
    Square from_sq = move.from();
    Square to_sq = move.to();

    if(from_sq == to_sq) {
        os << "0000";
        return os;
    }
    os << squareToStringArray[from_sq] << squareToStringArray[to_sq];
    if (move.typeOf() == Move::Promotion) {
        os << pieceToCharArray[(int)move.promotionType() + 6];
    }

    return os;
}

struct Movelist {
    Move moves[256] = {0};
    int num = 0;

    void add(Move move) {
        moves[num] = move;
        num++;
    }

    void clear() {
        num = 0;
    }

    Move operator[](int idx) const {
        return moves[idx];
    }

    Move &operator[](int idx) {
        return moves[idx];
    }

    typedef Move *iterator;
    typedef const Move *const_iterator;

    iterator begin() {
        return moves;
    }

    iterator end() {
        return moves + num;
    }

    const_iterator begin() const {
        return moves;
    }

    const_iterator end() const {
        return moves + num;
    }

    int size() const {
        return num;
    }

    friend std::ostream &operator<<(std::ostream &os, const Movelist &movelist);
};

std::ostream &operator<<(std::ostream &os, const Movelist &movelist) {
    for(int i = 0; i < movelist.size(); i++) {
        os << movelist[i] << " ";
    }
    return os;
}

void printBitboard(Bitboard bb) {
    std::bitset<64> b(bb);
    std::string str_bitset = b.to_string();
    for (int i = 0; i < 64; i += 8) {
        std::string x = str_bitset.substr(i, 8);
        for (int j = 0; j < 8; j++) {
            std::cout << x[j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Completely lifted from Disservin's library
namespace builtin {
/// @brief Get the least significant bit of a U64.
/// @param b
/// @return
inline Square lsb(uint64_t b);

/// @brief Get the most significant bit of a U64.
/// @param b
/// @return
inline Square msb(uint64_t b);

#if defined(__GNUC__)  // GCC, Clang, ICC
inline Square lsb(uint64_t b) {
    return Square(__builtin_ctzll(b));
}

inline Square msb(uint64_t b) {
    return Square(63 ^ __builtin_clzll(b));
}

#elif defined(_MSC_VER)  // MSVC

#ifdef _WIN64  // MSVC, WIN64
#include <intrin.h>
inline Square lsb(U64 b) {
    unsigned long idx;
    _BitScanForward64(&idx, b);
    return (Square)idx;
}

inline Square msb(U64 b) {
    unsigned long idx;
    _BitScanReverse64(&idx, b);
    return (Square)idx;
}

#else  // MSVC, WIN32
#include <intrin.h>
inline Square lsb(U64 b) {
    unsigned long idx;

    if (b & 0xffffffff) {
        _BitScanForward(&idx, int32_t(b));
        return Square(idx);
    } else {
        _BitScanForward(&idx, int32_t(b >> 32));
        return Square(idx + 32);
    }
}

inline Square msb(U64 b) {
    unsigned long idx;

    if (b >> 32) {
        _BitScanReverse(&idx, int32_t(b >> 32));
        return Square(idx + 32);
    } else {
        _BitScanReverse(&idx, int32_t(b));
        return Square(idx);
    }
}

#endif

#else  // Compiler is neither GCC nor MSVC compatible

#error "Compiler not supported."

#endif

/// @brief Count the number of set bits in a U64.
/// @param mask
/// @return
inline int popcount(uint64_t mask) {
#if defined(_MSC_VER) || defined(__INTEL_COMPILER)

    return (uint8_t)_mm_popcnt_u64(mask);

#else  // Assumed gcc or compatible compiler

    return __builtin_popcountll(mask);

#endif
}

/// @brief Get the least significant bit of a U64 and pop it.
/// @param mask
/// @return
inline Square poplsb(Bitboard &mask) {
    Square s = lsb(mask);
    mask &= mask - 1;  // compiler optimizes this to _blsr_u64
    return Square(s);
}

}  // namespace builtin

class BitField16 {
   public:
    BitField16() : value_(0) {}

    // Sets the value of the specified group to the given value
    void setGroupValue(uint16_t group_index, uint16_t group_value) {

        // calculate the bit position of the start of the group you want to set
        const uint16_t startBit = group_index * group_size_;
        const auto setMask = static_cast<uint16_t>(group_value << startBit);

        // clear the bits in the group
        value_ &= ~(0xF << startBit);

        // set the bits in the group
        value_ |= setMask;
    }

    [[nodiscard]] uint16_t getGroup(uint16_t group_index) const {
        uint16_t startBit = group_index * group_size_;
        return (value_ >> startBit) & 0xF;
    }

    void clear() { value_ = 0; }
    [[nodiscard]] uint16_t get() const { return value_; }

   private:
    static constexpr uint16_t group_size_ = 4;  // size of each group
    uint16_t value_;
};

class CastlingRights {
   public:
    template <Color color, CastleSide castle, File rook_file>
    void setCastlingRight() {
        int file = static_cast<uint16_t>(rook_file) + 1;

        castling_rights_.setGroupValue(2 * static_cast<int>(color) + static_cast<int>(castle),
                                       static_cast<uint16_t>(file));
    }

    void setCastlingRight(Color color, CastleSide castle, File rook_file) {
        int file = static_cast<uint16_t>(rook_file) + 1;

        castling_rights_.setGroupValue(2 * static_cast<int>(color) + static_cast<int>(castle),
                                       static_cast<uint16_t>(file));
    }

    void clearAllCastlingRights() { castling_rights_.clear(); }

    void clearCastlingRight(Color color, CastleSide castle) {
        castling_rights_.setGroupValue(2 * static_cast<int>(color) + static_cast<int>(castle), 0);
    }

    void clearCastlingRight(Color color) {
        castling_rights_.setGroupValue(2 * static_cast<int>(color), 0);
        castling_rights_.setGroupValue(2 * static_cast<int>(color) + 1, 0);
    }

    [[nodiscard]] bool isEmpty() const { return castling_rights_.get() == 0; }

    [[nodiscard]] bool hasCastlingRight(Color color) const {
        return castling_rights_.getGroup(2 * static_cast<int>(color)) != 0 ||
               castling_rights_.getGroup(2 * static_cast<int>(color) + 1) != 0;
    }

    [[nodiscard]] bool hasCastlingRight(Color color, CastleSide castle) const {
        return castling_rights_.getGroup(2 * static_cast<int>(color) + static_cast<int>(castle)) !=
               0;
    }

    [[nodiscard]] File getRookFile(Color color, CastleSide castle) const {
        return static_cast<File>(
            castling_rights_.getGroup(2 * static_cast<int>(color) + static_cast<int>(castle)) - 1);
    }

    [[nodiscard]] int getHashIndex() const {
        return hasCastlingRight(Color::White, CastleSide::KING_SIDE) +
               2 * hasCastlingRight(Color::White, CastleSide::QUEEN_SIDE) +
               4 * hasCastlingRight(Color::Black, CastleSide::KING_SIDE) +
               8 * hasCastlingRight(Color::Black, CastleSide::QUEEN_SIDE);
    }

   private:
    /*
     denotes the file of the rook that we castle to
     1248 1248 1248 1248
     0000 0000 0000 0000
     bq   bk   wq   wk
     3    2    1    0    // group index
     */
    BitField16 castling_rights_;
};

struct State {
    uint64_t hash;
    uint8_t half_moves;
};

[[nodiscard]] inline std::vector<std::string> splitString(const std::string &string,
                                                          const char &delimiter) {
    std::stringstream string_stream(string);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(string_stream, segment, delimiter)) seglist.emplace_back(segment);

    return seglist;
}

namespace zobrist {
static constexpr uint64_t RANDOM_ARRAY[781] = {
    0x9D39247E33776D41, 0x2AF7398005AAA5C7, 0x44DB015024623547, 0x9C15F73E62A76AE2,
    0x75834465489C0C89, 0x3290AC3A203001BF, 0x0FBBAD1F61042279, 0xE83A908FF2FB60CA,
    0x0D7E765D58755C10, 0x1A083822CEAFE02D, 0x9605D5F0E25EC3B0, 0xD021FF5CD13A2ED5,
    0x40BDF15D4A672E32, 0x011355146FD56395, 0x5DB4832046F3D9E5, 0x239F8B2D7FF719CC,
    0x05D1A1AE85B49AA1, 0x679F848F6E8FC971, 0x7449BBFF801FED0B, 0x7D11CDB1C3B7ADF0,
    0x82C7709E781EB7CC, 0xF3218F1C9510786C, 0x331478F3AF51BBE6, 0x4BB38DE5E7219443,
    0xAA649C6EBCFD50FC, 0x8DBD98A352AFD40B, 0x87D2074B81D79217, 0x19F3C751D3E92AE1,
    0xB4AB30F062B19ABF, 0x7B0500AC42047AC4, 0xC9452CA81A09D85D, 0x24AA6C514DA27500,
    0x4C9F34427501B447, 0x14A68FD73C910841, 0xA71B9B83461CBD93, 0x03488B95B0F1850F,
    0x637B2B34FF93C040, 0x09D1BC9A3DD90A94, 0x3575668334A1DD3B, 0x735E2B97A4C45A23,
    0x18727070F1BD400B, 0x1FCBACD259BF02E7, 0xD310A7C2CE9B6555, 0xBF983FE0FE5D8244,
    0x9F74D14F7454A824, 0x51EBDC4AB9BA3035, 0x5C82C505DB9AB0FA, 0xFCF7FE8A3430B241,
    0x3253A729B9BA3DDE, 0x8C74C368081B3075, 0xB9BC6C87167C33E7, 0x7EF48F2B83024E20,
    0x11D505D4C351BD7F, 0x6568FCA92C76A243, 0x4DE0B0F40F32A7B8, 0x96D693460CC37E5D,
    0x42E240CB63689F2F, 0x6D2BDCDAE2919661, 0x42880B0236E4D951, 0x5F0F4A5898171BB6,
    0x39F890F579F92F88, 0x93C5B5F47356388B, 0x63DC359D8D231B78, 0xEC16CA8AEA98AD76,
    0x5355F900C2A82DC7, 0x07FB9F855A997142, 0x5093417AA8A7ED5E, 0x7BCBC38DA25A7F3C,
    0x19FC8A768CF4B6D4, 0x637A7780DECFC0D9, 0x8249A47AEE0E41F7, 0x79AD695501E7D1E8,
    0x14ACBAF4777D5776, 0xF145B6BECCDEA195, 0xDABF2AC8201752FC, 0x24C3C94DF9C8D3F6,
    0xBB6E2924F03912EA, 0x0CE26C0B95C980D9, 0xA49CD132BFBF7CC4, 0xE99D662AF4243939,
    0x27E6AD7891165C3F, 0x8535F040B9744FF1, 0x54B3F4FA5F40D873, 0x72B12C32127FED2B,
    0xEE954D3C7B411F47, 0x9A85AC909A24EAA1, 0x70AC4CD9F04F21F5, 0xF9B89D3E99A075C2,
    0x87B3E2B2B5C907B1, 0xA366E5B8C54F48B8, 0xAE4A9346CC3F7CF2, 0x1920C04D47267BBD,
    0x87BF02C6B49E2AE9, 0x092237AC237F3859, 0xFF07F64EF8ED14D0, 0x8DE8DCA9F03CC54E,
    0x9C1633264DB49C89, 0xB3F22C3D0B0B38ED, 0x390E5FB44D01144B, 0x5BFEA5B4712768E9,
    0x1E1032911FA78984, 0x9A74ACB964E78CB3, 0x4F80F7A035DAFB04, 0x6304D09A0B3738C4,
    0x2171E64683023A08, 0x5B9B63EB9CEFF80C, 0x506AACF489889342, 0x1881AFC9A3A701D6,
    0x6503080440750644, 0xDFD395339CDBF4A7, 0xEF927DBCF00C20F2, 0x7B32F7D1E03680EC,
    0xB9FD7620E7316243, 0x05A7E8A57DB91B77, 0xB5889C6E15630A75, 0x4A750A09CE9573F7,
    0xCF464CEC899A2F8A, 0xF538639CE705B824, 0x3C79A0FF5580EF7F, 0xEDE6C87F8477609D,
    0x799E81F05BC93F31, 0x86536B8CF3428A8C, 0x97D7374C60087B73, 0xA246637CFF328532,
    0x043FCAE60CC0EBA0, 0x920E449535DD359E, 0x70EB093B15B290CC, 0x73A1921916591CBD,
    0x56436C9FE1A1AA8D, 0xEFAC4B70633B8F81, 0xBB215798D45DF7AF, 0x45F20042F24F1768,
    0x930F80F4E8EB7462, 0xFF6712FFCFD75EA1, 0xAE623FD67468AA70, 0xDD2C5BC84BC8D8FC,
    0x7EED120D54CF2DD9, 0x22FE545401165F1C, 0xC91800E98FB99929, 0x808BD68E6AC10365,
    0xDEC468145B7605F6, 0x1BEDE3A3AEF53302, 0x43539603D6C55602, 0xAA969B5C691CCB7A,
    0xA87832D392EFEE56, 0x65942C7B3C7E11AE, 0xDED2D633CAD004F6, 0x21F08570F420E565,
    0xB415938D7DA94E3C, 0x91B859E59ECB6350, 0x10CFF333E0ED804A, 0x28AED140BE0BB7DD,
    0xC5CC1D89724FA456, 0x5648F680F11A2741, 0x2D255069F0B7DAB3, 0x9BC5A38EF729ABD4,
    0xEF2F054308F6A2BC, 0xAF2042F5CC5C2858, 0x480412BAB7F5BE2A, 0xAEF3AF4A563DFE43,
    0x19AFE59AE451497F, 0x52593803DFF1E840, 0xF4F076E65F2CE6F0, 0x11379625747D5AF3,
    0xBCE5D2248682C115, 0x9DA4243DE836994F, 0x066F70B33FE09017, 0x4DC4DE189B671A1C,
    0x51039AB7712457C3, 0xC07A3F80C31FB4B4, 0xB46EE9C5E64A6E7C, 0xB3819A42ABE61C87,
    0x21A007933A522A20, 0x2DF16F761598AA4F, 0x763C4A1371B368FD, 0xF793C46702E086A0,
    0xD7288E012AEB8D31, 0xDE336A2A4BC1C44B, 0x0BF692B38D079F23, 0x2C604A7A177326B3,
    0x4850E73E03EB6064, 0xCFC447F1E53C8E1B, 0xB05CA3F564268D99, 0x9AE182C8BC9474E8,
    0xA4FC4BD4FC5558CA, 0xE755178D58FC4E76, 0x69B97DB1A4C03DFE, 0xF9B5B7C4ACC67C96,
    0xFC6A82D64B8655FB, 0x9C684CB6C4D24417, 0x8EC97D2917456ED0, 0x6703DF9D2924E97E,
    0xC547F57E42A7444E, 0x78E37644E7CAD29E, 0xFE9A44E9362F05FA, 0x08BD35CC38336615,
    0x9315E5EB3A129ACE, 0x94061B871E04DF75, 0xDF1D9F9D784BA010, 0x3BBA57B68871B59D,
    0xD2B7ADEEDED1F73F, 0xF7A255D83BC373F8, 0xD7F4F2448C0CEB81, 0xD95BE88CD210FFA7,
    0x336F52F8FF4728E7, 0xA74049DAC312AC71, 0xA2F61BB6E437FDB5, 0x4F2A5CB07F6A35B3,
    0x87D380BDA5BF7859, 0x16B9F7E06C453A21, 0x7BA2484C8A0FD54E, 0xF3A678CAD9A2E38C,
    0x39B0BF7DDE437BA2, 0xFCAF55C1BF8A4424, 0x18FCF680573FA594, 0x4C0563B89F495AC3,
    0x40E087931A00930D, 0x8CFFA9412EB642C1, 0x68CA39053261169F, 0x7A1EE967D27579E2,
    0x9D1D60E5076F5B6F, 0x3810E399B6F65BA2, 0x32095B6D4AB5F9B1, 0x35CAB62109DD038A,
    0xA90B24499FCFAFB1, 0x77A225A07CC2C6BD, 0x513E5E634C70E331, 0x4361C0CA3F692F12,
    0xD941ACA44B20A45B, 0x528F7C8602C5807B, 0x52AB92BEB9613989, 0x9D1DFA2EFC557F73,
    0x722FF175F572C348, 0x1D1260A51107FE97, 0x7A249A57EC0C9BA2, 0x04208FE9E8F7F2D6,
    0x5A110C6058B920A0, 0x0CD9A497658A5698, 0x56FD23C8F9715A4C, 0x284C847B9D887AAE,
    0x04FEABFBBDB619CB, 0x742E1E651C60BA83, 0x9A9632E65904AD3C, 0x881B82A13B51B9E2,
    0x506E6744CD974924, 0xB0183DB56FFC6A79, 0x0ED9B915C66ED37E, 0x5E11E86D5873D484,
    0xF678647E3519AC6E, 0x1B85D488D0F20CC5, 0xDAB9FE6525D89021, 0x0D151D86ADB73615,
    0xA865A54EDCC0F019, 0x93C42566AEF98FFB, 0x99E7AFEABE000731, 0x48CBFF086DDF285A,
    0x7F9B6AF1EBF78BAF, 0x58627E1A149BBA21, 0x2CD16E2ABD791E33, 0xD363EFF5F0977996,
    0x0CE2A38C344A6EED, 0x1A804AADB9CFA741, 0x907F30421D78C5DE, 0x501F65EDB3034D07,
    0x37624AE5A48FA6E9, 0x957BAF61700CFF4E, 0x3A6C27934E31188A, 0xD49503536ABCA345,
    0x088E049589C432E0, 0xF943AEE7FEBF21B8, 0x6C3B8E3E336139D3, 0x364F6FFA464EE52E,
    0xD60F6DCEDC314222, 0x56963B0DCA418FC0, 0x16F50EDF91E513AF, 0xEF1955914B609F93,
    0x565601C0364E3228, 0xECB53939887E8175, 0xBAC7A9A18531294B, 0xB344C470397BBA52,
    0x65D34954DAF3CEBD, 0xB4B81B3FA97511E2, 0xB422061193D6F6A7, 0x071582401C38434D,
    0x7A13F18BBEDC4FF5, 0xBC4097B116C524D2, 0x59B97885E2F2EA28, 0x99170A5DC3115544,
    0x6F423357E7C6A9F9, 0x325928EE6E6F8794, 0xD0E4366228B03343, 0x565C31F7DE89EA27,
    0x30F5611484119414, 0xD873DB391292ED4F, 0x7BD94E1D8E17DEBC, 0xC7D9F16864A76E94,
    0x947AE053EE56E63C, 0xC8C93882F9475F5F, 0x3A9BF55BA91F81CA, 0xD9A11FBB3D9808E4,
    0x0FD22063EDC29FCA, 0xB3F256D8ACA0B0B9, 0xB03031A8B4516E84, 0x35DD37D5871448AF,
    0xE9F6082B05542E4E, 0xEBFAFA33D7254B59, 0x9255ABB50D532280, 0xB9AB4CE57F2D34F3,
    0x693501D628297551, 0xC62C58F97DD949BF, 0xCD454F8F19C5126A, 0xBBE83F4ECC2BDECB,
    0xDC842B7E2819E230, 0xBA89142E007503B8, 0xA3BC941D0A5061CB, 0xE9F6760E32CD8021,
    0x09C7E552BC76492F, 0x852F54934DA55CC9, 0x8107FCCF064FCF56, 0x098954D51FFF6580,
    0x23B70EDB1955C4BF, 0xC330DE426430F69D, 0x4715ED43E8A45C0A, 0xA8D7E4DAB780A08D,
    0x0572B974F03CE0BB, 0xB57D2E985E1419C7, 0xE8D9ECBE2CF3D73F, 0x2FE4B17170E59750,
    0x11317BA87905E790, 0x7FBF21EC8A1F45EC, 0x1725CABFCB045B00, 0x964E915CD5E2B207,
    0x3E2B8BCBF016D66D, 0xBE7444E39328A0AC, 0xF85B2B4FBCDE44B7, 0x49353FEA39BA63B1,
    0x1DD01AAFCD53486A, 0x1FCA8A92FD719F85, 0xFC7C95D827357AFA, 0x18A6A990C8B35EBD,
    0xCCCB7005C6B9C28D, 0x3BDBB92C43B17F26, 0xAA70B5B4F89695A2, 0xE94C39A54A98307F,
    0xB7A0B174CFF6F36E, 0xD4DBA84729AF48AD, 0x2E18BC1AD9704A68, 0x2DE0966DAF2F8B1C,
    0xB9C11D5B1E43A07E, 0x64972D68DEE33360, 0x94628D38D0C20584, 0xDBC0D2B6AB90A559,
    0xD2733C4335C6A72F, 0x7E75D99D94A70F4D, 0x6CED1983376FA72B, 0x97FCAACBF030BC24,
    0x7B77497B32503B12, 0x8547EDDFB81CCB94, 0x79999CDFF70902CB, 0xCFFE1939438E9B24,
    0x829626E3892D95D7, 0x92FAE24291F2B3F1, 0x63E22C147B9C3403, 0xC678B6D860284A1C,
    0x5873888850659AE7, 0x0981DCD296A8736D, 0x9F65789A6509A440, 0x9FF38FED72E9052F,
    0xE479EE5B9930578C, 0xE7F28ECD2D49EECD, 0x56C074A581EA17FE, 0x5544F7D774B14AEF,
    0x7B3F0195FC6F290F, 0x12153635B2C0CF57, 0x7F5126DBBA5E0CA7, 0x7A76956C3EAFB413,
    0x3D5774A11D31AB39, 0x8A1B083821F40CB4, 0x7B4A38E32537DF62, 0x950113646D1D6E03,
    0x4DA8979A0041E8A9, 0x3BC36E078F7515D7, 0x5D0A12F27AD310D1, 0x7F9D1A2E1EBE1327,
    0xDA3A361B1C5157B1, 0xDCDD7D20903D0C25, 0x36833336D068F707, 0xCE68341F79893389,
    0xAB9090168DD05F34, 0x43954B3252DC25E5, 0xB438C2B67F98E5E9, 0x10DCD78E3851A492,
    0xDBC27AB5447822BF, 0x9B3CDB65F82CA382, 0xB67B7896167B4C84, 0xBFCED1B0048EAC50,
    0xA9119B60369FFEBD, 0x1FFF7AC80904BF45, 0xAC12FB171817EEE7, 0xAF08DA9177DDA93D,
    0x1B0CAB936E65C744, 0xB559EB1D04E5E932, 0xC37B45B3F8D6F2BA, 0xC3A9DC228CAAC9E9,
    0xF3B8B6675A6507FF, 0x9FC477DE4ED681DA, 0x67378D8ECCEF96CB, 0x6DD856D94D259236,
    0xA319CE15B0B4DB31, 0x073973751F12DD5E, 0x8A8E849EB32781A5, 0xE1925C71285279F5,
    0x74C04BF1790C0EFE, 0x4DDA48153C94938A, 0x9D266D6A1CC0542C, 0x7440FB816508C4FE,
    0x13328503DF48229F, 0xD6BF7BAEE43CAC40, 0x4838D65F6EF6748F, 0x1E152328F3318DEA,
    0x8F8419A348F296BF, 0x72C8834A5957B511, 0xD7A023A73260B45C, 0x94EBC8ABCFB56DAE,
    0x9FC10D0F989993E0, 0xDE68A2355B93CAE6, 0xA44CFE79AE538BBE, 0x9D1D84FCCE371425,
    0x51D2B1AB2DDFB636, 0x2FD7E4B9E72CD38C, 0x65CA5B96B7552210, 0xDD69A0D8AB3B546D,
    0x604D51B25FBF70E2, 0x73AA8A564FB7AC9E, 0x1A8C1E992B941148, 0xAAC40A2703D9BEA0,
    0x764DBEAE7FA4F3A6, 0x1E99B96E70A9BE8B, 0x2C5E9DEB57EF4743, 0x3A938FEE32D29981,
    0x26E6DB8FFDF5ADFE, 0x469356C504EC9F9D, 0xC8763C5B08D1908C, 0x3F6C6AF859D80055,
    0x7F7CC39420A3A545, 0x9BFB227EBDF4C5CE, 0x89039D79D6FC5C5C, 0x8FE88B57305E2AB6,
    0xA09E8C8C35AB96DE, 0xFA7E393983325753, 0xD6B6D0ECC617C699, 0xDFEA21EA9E7557E3,
    0xB67C1FA481680AF8, 0xCA1E3785A9E724E5, 0x1CFC8BED0D681639, 0xD18D8549D140CAEA,
    0x4ED0FE7E9DC91335, 0xE4DBF0634473F5D2, 0x1761F93A44D5AEFE, 0x53898E4C3910DA55,
    0x734DE8181F6EC39A, 0x2680B122BAA28D97, 0x298AF231C85BAFAB, 0x7983EED3740847D5,
    0x66C1A2A1A60CD889, 0x9E17E49642A3E4C1, 0xEDB454E7BADC0805, 0x50B704CAB602C329,
    0x4CC317FB9CDDD023, 0x66B4835D9EAFEA22, 0x219B97E26FFC81BD, 0x261E4E4C0A333A9D,
    0x1FE2CCA76517DB90, 0xD7504DFA8816EDBB, 0xB9571FA04DC089C8, 0x1DDC0325259B27DE,
    0xCF3F4688801EB9AA, 0xF4F5D05C10CAB243, 0x38B6525C21A42B0E, 0x36F60E2BA4FA6800,
    0xEB3593803173E0CE, 0x9C4CD6257C5A3603, 0xAF0C317D32ADAA8A, 0x258E5A80C7204C4B,
    0x8B889D624D44885D, 0xF4D14597E660F855, 0xD4347F66EC8941C3, 0xE699ED85B0DFB40D,
    0x2472F6207C2D0484, 0xC2A1E7B5B459AEB5, 0xAB4F6451CC1D45EC, 0x63767572AE3D6174,
    0xA59E0BD101731A28, 0x116D0016CB948F09, 0x2CF9C8CA052F6E9F, 0x0B090A7560A968E3,
    0xABEEDDB2DDE06FF1, 0x58EFC10B06A2068D, 0xC6E57A78FBD986E0, 0x2EAB8CA63CE802D7,
    0x14A195640116F336, 0x7C0828DD624EC390, 0xD74BBE77E6116AC7, 0x804456AF10F5FB53,
    0xEBE9EA2ADF4321C7, 0x03219A39EE587A30, 0x49787FEF17AF9924, 0xA1E9300CD8520548,
    0x5B45E522E4B1B4EF, 0xB49C3B3995091A36, 0xD4490AD526F14431, 0x12A8F216AF9418C2,
    0x001F837CC7350524, 0x1877B51E57A764D5, 0xA2853B80F17F58EE, 0x993E1DE72D36D310,
    0xB3598080CE64A656, 0x252F59CF0D9F04BB, 0xD23C8E176D113600, 0x1BDA0492E7E4586E,
    0x21E0BD5026C619BF, 0x3B097ADAF088F94E, 0x8D14DEDB30BE846E, 0xF95CFFA23AF5F6F4,
    0x3871700761B3F743, 0xCA672B91E9E4FA16, 0x64C8E531BFF53B55, 0x241260ED4AD1E87D,
    0x106C09B972D2E822, 0x7FBA195410E5CA30, 0x7884D9BC6CB569D8, 0x0647DFEDCD894A29,
    0x63573FF03E224774, 0x4FC8E9560F91B123, 0x1DB956E450275779, 0xB8D91274B9E9D4FB,
    0xA2EBEE47E2FBFCE1, 0xD9F1F30CCD97FB09, 0xEFED53D75FD64E6B, 0x2E6D02C36017F67F,
    0xA9AA4D20DB084E9B, 0xB64BE8D8B25396C1, 0x70CB6AF7C2D5BCF0, 0x98F076A4F7A2322E,
    0xBF84470805E69B5F, 0x94C3251F06F90CF3, 0x3E003E616A6591E9, 0xB925A6CD0421AFF3,
    0x61BDD1307C66E300, 0xBF8D5108E27E0D48, 0x240AB57A8B888B20, 0xFC87614BAF287E07,
    0xEF02CDD06FFDB432, 0xA1082C0466DF6C0A, 0x8215E577001332C8, 0xD39BB9C3A48DB6CF,
    0x2738259634305C14, 0x61CF4F94C97DF93D, 0x1B6BACA2AE4E125B, 0x758F450C88572E0B,
    0x959F587D507A8359, 0xB063E962E045F54D, 0x60E8ED72C0DFF5D1, 0x7B64978555326F9F,
    0xFD080D236DA814BA, 0x8C90FD9B083F4558, 0x106F72FE81E2C590, 0x7976033A39F7D952,
    0xA4EC0132764CA04B, 0x733EA705FAE4FA77, 0xB4D8F77BC3E56167, 0x9E21F4F903B33FD9,
    0x9D765E419FB69F6D, 0xD30C088BA61EA5EF, 0x5D94337FBFAF7F5B, 0x1A4E4822EB4D7A59,
    0x6FFE73E81B637FB3, 0xDDF957BC36D8B9CA, 0x64D0E29EEA8838B3, 0x08DD9BDFD96B9F63,
    0x087E79E5A57D1D13, 0xE328E230E3E2B3FB, 0x1C2559E30F0946BE, 0x720BF5F26F4D2EAA,
    0xB0774D261CC609DB, 0x443F64EC5A371195, 0x4112CF68649A260E, 0xD813F2FAB7F5C5CA,
    0x660D3257380841EE, 0x59AC2C7873F910A3, 0xE846963877671A17, 0x93B633ABFA3469F8,
    0xC0C0F5A60EF4CDCF, 0xCAF21ECD4377B28C, 0x57277707199B8175, 0x506C11B9D90E8B1D,
    0xD83CC2687A19255F, 0x4A29C6465A314CD1, 0xED2DF21216235097, 0xB5635C95FF7296E2,
    0x22AF003AB672E811, 0x52E762596BF68235, 0x9AEBA33AC6ECC6B0, 0x944F6DE09134DFB6,
    0x6C47BEC883A7DE39, 0x6AD047C430A12104, 0xA5B1CFDBA0AB4067, 0x7C45D833AFF07862,
    0x5092EF950A16DA0B, 0x9338E69C052B8E7B, 0x455A4B4CFE30E3F5, 0x6B02E63195AD0CF8,
    0x6B17B224BAD6BF27, 0xD1E0CCD25BB9C169, 0xDE0C89A556B9AE70, 0x50065E535A213CF6,
    0x9C1169FA2777B874, 0x78EDEFD694AF1EED, 0x6DC93D9526A50E68, 0xEE97F453F06791ED,
    0x32AB0EDB696703D3, 0x3A6853C7E70757A7, 0x31865CED6120F37D, 0x67FEF95D92607890,
    0x1F2B1D1F15F6DC9C, 0xB69E38A8965C6B65, 0xAA9119FF184CCCF4, 0xF43C732873F24C13,
    0xFB4A3D794A9A80D2, 0x3550C2321FD6109C, 0x371F77E76BB8417E, 0x6BFA9AAE5EC05779,
    0xCD04F3FF001A4778, 0xE3273522064480CA, 0x9F91508BFFCFC14A, 0x049A7F41061A9E60,
    0xFCB6BE43A9F2FE9B, 0x08DE8A1C7797DA9B, 0x8F9887E6078735A1, 0xB5B4071DBFC73A66,
    0x230E343DFBA08D33, 0x43ED7F5A0FAE657D, 0x3A88A0FBBCB05C63, 0x21874B8B4D2DBC4F,
    0x1BDEA12E35F6A8C9, 0x53C065C6C8E63528, 0xE34A1D250E7A8D6B, 0xD6B04D3B7651DD7E,
    0x5E90277E7CB39E2D, 0x2C046F22062DC67D, 0xB10BB459132D0A26, 0x3FA9DDFB67E2F199,
    0x0E09B88E1914F7AF, 0x10E8B35AF3EEAB37, 0x9EEDECA8E272B933, 0xD4C718BC4AE8AE5F,
    0x81536D601170FC20, 0x91B534F885818A06, 0xEC8177F83F900978, 0x190E714FADA5156E,
    0xB592BF39B0364963, 0x89C350C893AE7DC1, 0xAC042E70F8B383F2, 0xB49B52E587A1EE60,
    0xFB152FE3FF26DA89, 0x3E666E6F69AE2C15, 0x3B544EBE544C19F9, 0xE805A1E290CF2456,
    0x24B33C9D7ED25117, 0xE74733427B72F0C1, 0x0A804D18B7097475, 0x57E3306D881EDB4F,
    0x4AE7D6A36EB5DBCB, 0x2D8D5432157064C8, 0xD1E649DE1E7F268B, 0x8A328A1CEDFE552C,
    0x07A3AEC79624C7DA, 0x84547DDC3E203C94, 0x990A98FD5071D263, 0x1A4FF12616EEFC89,
    0xF6F7FD1431714200, 0x30C05B1BA332F41C, 0x8D2636B81555A786, 0x46C9FEB55D120902,
    0xCCEC0A73B49C9921, 0x4E9D2827355FC492, 0x19EBB029435DCB0F, 0x4659D2B743848A2C,
    0x963EF2C96B33BE31, 0x74F85198B05A2E7D, 0x5A0F544DD2B1FB18, 0x03727073C2E134B1,
    0xC7F6AA2DE59AEA61, 0x352787BAA0D7C22F, 0x9853EAB63B5E0B35, 0xABBDCDD7ED5C0860,
    0xCF05DAF5AC8D77B0, 0x49CAD48CEBF4A71E, 0x7A4C10EC2158C4A6, 0xD9E92AA246BF719E,
    0x13AE978D09FE5557, 0x730499AF921549FF, 0x4E4B705B92903BA4, 0xFF577222C14F0A3A,
    0x55B6344CF97AAFAE, 0xB862225B055B6960, 0xCAC09AFBDDD2CDB4, 0xDAF8E9829FE96B5F,
    0xB5FDFC5D3132C498, 0x310CB380DB6F7503, 0xE87FBB46217A360E, 0x2102AE466EBB1148,
    0xF8549E1A3AA5E00D, 0x07A69AFDCC42261A, 0xC4C118BFE78FEAAE, 0xF9F4892ED96BD438,
    0x1AF3DBE25D8F45DA, 0xF5B4B0B0D2DEEEB4, 0x962ACEEFA82E1C84, 0x046E3ECAAF453CE9,
    0xF05D129681949A4C, 0x964781CE734B3C84, 0x9C2ED44081CE5FBD, 0x522E23F3925E319E,
    0x177E00F9FC32F791, 0x2BC60A63A6F3B3F2, 0x222BBFAE61725606, 0x486289DDCC3D6780,
    0x7DC7785B8EFDFC80, 0x8AF38731C02BA980, 0x1FAB64EA29A2DDF7, 0xE4D9429322CD065A,
    0x9DA058C67844F20C, 0x24C0E332B70019B0, 0x233003B5A6CFE6AD, 0xD586BD01C5C217F6,
    0x5E5637885F29BC2B, 0x7EBA726D8C94094B, 0x0A56A5F0BFE39272, 0xD79476A84EE20D06,
    0x9E4C1269BAA4BF37, 0x17EFEE45B0DEE640, 0x1D95B0A5FCF90BC6, 0x93CBE0B699C2585D,
    0x65FA4F227A2B6D79, 0xD5F9E858292504D5, 0xC2B5A03F71471A6F, 0x59300222B4561E00,
    0xCE2F8642CA0712DC, 0x7CA9723FBB2E8988, 0x2785338347F2BA08, 0xC61BB3A141E50E8C,
    0x150F361DAB9DEC26, 0x9F6A419D382595F4, 0x64A53DC924FE7AC9, 0x142DE49FFF7A7C3D,
    0x0C335248857FA9E7, 0x0A9C32D5EAE45305, 0xE6C42178C4BBB92E, 0x71F1CE2490D20B07,
    0xF1BCC3D275AFE51A, 0xE728E8C83C334074, 0x96FBF83A12884624, 0x81A1549FD6573DA5,
    0x5FA7867CAF35E149, 0x56986E2EF3ED091B, 0x917F1DD5F8886C61, 0xD20D8C88C8FFE65F,
    0x31D71DCE64B2C310, 0xF165B587DF898190, 0xA57E6339DD2CF3A0, 0x1EF6E6DBB1961EC9,
    0x70CC73D90BC26E24, 0xE21A6B35DF0C3AD7, 0x003A93D8B2806962, 0x1C99DED33CB890A1,
    0xCF3145DE0ADD4289, 0xD0E4427A5514FB72, 0x77C621CC9FB3A483, 0x67A34DAC4356550B,
    0xF8D626AAAF278509};

static constexpr uint64_t castlingKey[16] = {
    0,
    RANDOM_ARRAY[768],
    RANDOM_ARRAY[768 + 1],
    RANDOM_ARRAY[768] ^ RANDOM_ARRAY[768 + 1],
    RANDOM_ARRAY[768 + 2],
    RANDOM_ARRAY[768] ^ RANDOM_ARRAY[768 + 2],
    RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 2],
    RANDOM_ARRAY[768] ^ RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 2],
    RANDOM_ARRAY[768 + 3],
    RANDOM_ARRAY[768] ^ RANDOM_ARRAY[768 + 3],
    RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 3],
    RANDOM_ARRAY[768] ^ RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 3],
    RANDOM_ARRAY[768 + 3] ^ RANDOM_ARRAY[768 + 2],
    RANDOM_ARRAY[768 + 3] ^ RANDOM_ARRAY[768 + 2] ^ RANDOM_ARRAY[768],
    RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 2] ^ RANDOM_ARRAY[768 + 3],
    RANDOM_ARRAY[768 + 1] ^ RANDOM_ARRAY[768 + 2] ^ RANDOM_ARRAY[768 + 3] ^ RANDOM_ARRAY[768]};

static constexpr int MAP_HASH_PIECE[12] = {1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10};

inline uint64_t piece(Piece piece, Square square) {
    return RANDOM_ARRAY[64 * MAP_HASH_PIECE[static_cast<int>(piece)] + square];
}

inline uint64_t enpassant(File file) { return RANDOM_ARRAY[772 + static_cast<int>(file)]; }

inline uint64_t castling(int castling) { return castlingKey[castling]; }

inline uint64_t sideToMove() { return RANDOM_ARRAY[780]; }

}  // namespace zobrist

namespace movegen {

Bitboard Ranks[8] = {
    0xff,         0xff00,         0xff0000,         0xff000000,
    0xff00000000, 0xff0000000000, 0xff000000000000, 0xff00000000000000};

Bitboard Files[8] = {
    0x101010101010101,  0x202020202020202,  0x404040404040404,  0x808080808080808,
    0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
};

// Pawn movegen
void pawnPushes(Square pawn, Bitboard occupied, Color c, Movelist &moves) {
    Bitboard bb = 1ULL << pawn;
    if(c == Color::White) {
        Bitboard singlePush = (bb << 8) & ~occupied;
        if(singlePush && (bb & Ranks[6])) {
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Knight, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Bishop, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Rook, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Queen, Move::Promotion));
        }
        else if (singlePush) {
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush)));
            if(bb & Ranks[1]) {
                Bitboard doublePush = (singlePush << 8) & ~occupied;
                if(doublePush) moves.add(Move(builtin::lsb(bb), builtin::lsb(doublePush)));
            }
        }
    } else {
        Bitboard singlePush = (bb >> 8) & ~occupied;
        if(singlePush && (bb & Ranks[1])) {
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Knight, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Bishop, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Rook, Move::Promotion));
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush), PieceType::Queen, Move::Promotion));
        }
        else if(singlePush) {
            moves.add(Move(builtin::lsb(bb), builtin::lsb(singlePush)));
            if(bb & Ranks[6]) {
                Bitboard doublePush = (singlePush >> 8) & ~occupied;
                if(doublePush) moves.add(Move(builtin::lsb(bb), builtin::lsb(doublePush)));
            }
        }
    }
}

bool pawnPushPseudoLegal(Square pawn, Bitboard occupied, Color c, Bitboard target) {
    Bitboard bb = 1ULL << pawn;
    if(c == Color::White) {
        Bitboard singlePush = (bb << 8) & ~occupied;
        if(singlePush & target) {
            return true;
        }
        else if (singlePush) {
            if(bb & Ranks[1]) {
                Bitboard doublePush = (singlePush << 8) & ~occupied;
                if(doublePush & target) return true;
            }
        }
    } else {
        Bitboard singlePush = (bb >> 8) & ~occupied;
        if(singlePush & target) {
            return true;
        }
        else if(singlePush) {
            if(bb & Ranks[6]) {
                Bitboard doublePush = (singlePush >> 8) & ~occupied;
                if(doublePush & target) return true;
            }
        }
    }
    return false;
}

void pawnCaptures(Square pawn, Bitboard enemy, Color c, Square epSquare, Movelist &moves) {
    Bitboard bb = 1ULL << pawn;
    if((pawn & 7) != (epSquare & 7) && epSquare != 64) enemy |= (1ULL << epSquare);
    if(c == Color::White) {
        Bitboard captures = 0ULL;
        if(bb & ~Files[7]) captures = bb << 9;
        if(bb & ~Files[0]) captures |= bb << 7;
        captures &= enemy;
        if(bb & Ranks[6]) {
               while(captures) {
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Knight, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Bishop, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Rook, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::poplsb(captures), PieceType::Queen, Move::Promotion));
            }
        } else {
            while(captures) {
                Square dest = builtin::poplsb(captures);
                moves.add(Move(builtin::lsb(bb), dest, PieceType::Knight, (dest == epSquare) ? Move::EnPassant : Move::Normal));
            }
        }
    } else {
        Bitboard captures = 0ULL;
        if(bb & ~Files[7]) captures = bb >> 7;
        if(bb & ~Files[0]) captures |= bb >> 9;
        captures &= enemy;
        if(bb & Ranks[1]) {
            while(captures) {
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Knight, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Bishop, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::lsb(captures), PieceType::Rook, Move::Promotion));
                moves.add(Move(builtin::lsb(bb), builtin::poplsb(captures), PieceType::Queen, Move::Promotion));
            }
        } else {
            while(captures) {
                Square dest = builtin::poplsb(captures);
                moves.add(Move(builtin::lsb(bb), dest, PieceType::Knight, (dest == epSquare) ? Move::EnPassant : Move::Normal));
            }
        }
    }
}

bool pawnCapturesPseudoLegal(Square pawn, Bitboard enemy, Color c, Square epSquare, Bitboard target) {
    Bitboard bb = 1ULL << pawn;
    if((pawn & 7) != (epSquare & 7) && epSquare != 64) enemy |= (1ULL << epSquare);
    if(c == Color::White) {
        Bitboard captures = 0ULL;
        if(bb & ~Files[7]) captures = bb << 9;
        if(bb & ~Files[0]) captures |= bb << 7;
        captures &= enemy;
        if(captures & target) return true;
    } else {
        Bitboard captures = 0ULL;
        if(bb & ~Files[7]) captures = bb >> 7;
        if(bb & ~Files[0]) captures |= bb >> 9;
        captures &= enemy;
        if(captures & target) return true;
    }
    return false;
}

Bitboard KnightAttacks[64] = {
    0x0000000000020400, 0x0000000000050800, 0x00000000000A1100, 0x0000000000142200,
    0x0000000000284400, 0x0000000000508800, 0x0000000000A01000, 0x0000000000402000,
    0x0000000002040004, 0x0000000005080008, 0x000000000A110011, 0x0000000014220022,
    0x0000000028440044, 0x0000000050880088, 0x00000000A0100010, 0x0000000040200020,
    0x0000000204000402, 0x0000000508000805, 0x0000000A1100110A, 0x0000001422002214,
    0x0000002844004428, 0x0000005088008850, 0x000000A0100010A0, 0x0000004020002040,
    0x0000020400040200, 0x0000050800080500, 0x00000A1100110A00, 0x0000142200221400,
    0x0000284400442800, 0x0000508800885000, 0x0000A0100010A000, 0x0000402000204000,
    0x0002040004020000, 0x0005080008050000, 0x000A1100110A0000, 0x0014220022140000,
    0x0028440044280000, 0x0050880088500000, 0x00A0100010A00000, 0x0040200020400000,
    0x0204000402000000, 0x0508000805000000, 0x0A1100110A000000, 0x1422002214000000,
    0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000,
    0x0400040200000000, 0x0800080500000000, 0x1100110A00000000, 0x2200221400000000,
    0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000,
    0x0004020000000000, 0x0008050000000000, 0x00110A0000000000, 0x0022140000000000,
    0x0044280000000000, 0x0088500000000000, 0x0010A00000000000, 0x0020400000000000
};

Bitboard KingAttacks[64] = {
    0x0000000000000302, 0x0000000000000705, 0x0000000000000E0A, 0x0000000000001C14,
    0x0000000000003828, 0x0000000000007050, 0x000000000000E0A0, 0x000000000000C040,
    0x0000000000030203, 0x0000000000070507, 0x00000000000E0A0E, 0x00000000001C141C,
    0x0000000000382838, 0x0000000000705070, 0x0000000000E0A0E0, 0x0000000000C040C0,
    0x0000000003020300, 0x0000000007050700, 0x000000000E0A0E00, 0x000000001C141C00,
    0x0000000038283800, 0x0000000070507000, 0x00000000E0A0E000, 0x00000000C040C000,
    0x0000000302030000, 0x0000000705070000, 0x0000000E0A0E0000, 0x0000001C141C0000,
    0x0000003828380000, 0x0000007050700000, 0x000000E0A0E00000, 0x000000C040C00000,
    0x0000030203000000, 0x0000070507000000, 0x00000E0A0E000000, 0x00001C141C000000,
    0x0000382838000000, 0x0000705070000000, 0x0000E0A0E0000000, 0x0000C040C0000000,
    0x0003020300000000, 0x0007050700000000, 0x000E0A0E00000000, 0x001C141C00000000,
    0x0038283800000000, 0x0070507000000000, 0x00E0A0E000000000, 0x00C040C000000000,
    0x0302030000000000, 0x0705070000000000, 0x0E0A0E0000000000, 0x1C141C0000000000,
    0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000,
    0x0203000000000000, 0x0507000000000000, 0x0A0E000000000000, 0x141C000000000000,
    0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000
};

Bitboard pawnAttackSquares(Bitboard pawns, Color stm) {
    Bitboard attacks;
    if(stm == Color::White) {
        attacks = (pawns & ~Files[7]) << 9;
        attacks |= (pawns & ~Files[0]) << 7;
    } else {
        attacks = (pawns & ~Files[7]) >> 7;
        attacks |= (pawns & ~Files[0]) >> 9;
    }
    return attacks;
}

Bitboard knightAttackSquares(Bitboard knights) {
    Bitboard attacks = 0;
    while(knights) {
        Square knight = builtin::poplsb(knights);
        attacks |= KnightAttacks[knight];
    }
    return attacks;
}

Bitboard bishopAttackSquares(Bitboard bishops, Bitboard ours, Bitboard theirs) {
    Bitboard attacks = 0;
    while(bishops) {
        Square bishop = builtin::poplsb(bishops);
        Bitboard bb = 1ULL << bishop;

        while(bb & ~(Ranks[7] | Files[7])) {
            bb <<= 9;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << bishop;
        while(bb & ~(Ranks[7] | Files[0])) {
            bb <<= 7;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << bishop;
        while(bb & ~(Ranks[0] | Files[7])) {
            bb >>= 7;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << bishop;
        while(bb & ~(Ranks[0] | Files[0])) {
            bb >>= 9;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
    }
    return attacks;
}

Bitboard rookAttackSquares(Bitboard rooks, Bitboard ours, Bitboard theirs) {
    Bitboard attacks = 0;
    while(rooks) {
        Square rook = builtin::poplsb(rooks);
        Bitboard bb = 1ULL << rook;
        while(bb & ~Ranks[7]) {
            bb <<= 8;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << rook;
        while(bb & ~Files[7]) {
            bb <<= 1;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << rook;
        while(bb & ~Ranks[0]) {
            bb >>= 8;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
        bb = 1ULL << rook;
        while(bb & ~Files[0]) {
            bb >>= 1;
            if(bb & ours) break;
            attacks |= bb;
            if(bb & theirs) break;
        }
    }
    return attacks;
}

Bitboard kingAttackSquares(Bitboard king) {
    return KingAttacks[builtin::poplsb(king)];
}

// White, Black
// Kingside, Queenside
Bitboard CastleMask[2][2] = {
    {0x0000000000000060, 0x000000000000000e},
    {0x6000000000000000, 0x0e00000000000000}
};

void knightMoves(Square knight, Bitboard ours, Movelist &moves) {
    Bitboard attacks = KnightAttacks[knight] & ~ours;
    while(attacks) {
        moves.add(Move(knight, builtin::poplsb(attacks)));
    }
}

void knightCaptures(Square knight, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard attacks = KnightAttacks[knight] & theirs;
    while(attacks) {
        moves.add(Move(knight, builtin::poplsb(attacks)));
    }
}

void knightQuiets(Square knight, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard attacks = KnightAttacks[knight] & ~(theirs | ours);
    while(attacks) {
        moves.add(Move(knight, builtin::poplsb(attacks)));
    }
}

bool knightPseudoLegal(Square knight, Bitboard ours, Bitboard target) {
    Bitboard attacks = KnightAttacks[knight] & ~ours;
    return attacks & target;
}


void kingMoves(Square king, Bitboard ours, Movelist &moves) {
    Bitboard attacks = KingAttacks[king] & ~ours;
    while(attacks) {
        moves.add(Move(king, builtin::poplsb(attacks)));
    }
}

void kingCaptures(Square king, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard attacks = KingAttacks[king] & theirs;
    while(attacks) {
        moves.add(Move(king, builtin::poplsb(attacks)));
    }
}

void kingQuiets(Square king, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard attacks = KingAttacks[king] & ~(theirs | ours);
    while(attacks) {
        moves.add(Move(king, builtin::poplsb(attacks)));
    }
}

bool kingPseudoLegal(Square king, Bitboard ours, Bitboard target) {
    Bitboard attacks = KingAttacks[king] & ~ours;
    return attacks & target;
}


void bishopMoves(Square bishop, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[7])) {
        bb <<= 9;
        if(bb & ours) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[0])) {
        bb <<= 7;
        if(bb & ours) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[7])) {
        bb >>= 7;
        if(bb & ours) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[0])) {
        bb >>= 9;
        if(bb & ours) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
}

void bishopCaptures(Square bishop, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[7])) {
        bb <<= 9;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(bishop, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[0])) {
        bb <<= 7;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(bishop, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[7])) {
        bb >>= 7;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(bishop, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[0])) {
        bb >>= 9;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(bishop, builtin::lsb(bb)));
            break;
        }
    }
}

void bishopQuiets(Square bishop, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[7])) {
        bb <<= 9;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[0])) {
        bb <<= 7;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[7])) {
        bb >>= 7;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[0])) {
        bb >>= 9;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(bishop, builtin::lsb(bb)));
    }
}

bool bishopPseudoLegal(Square bishop, Bitboard ours, Bitboard theirs, Bitboard target) {
    Bitboard bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[7])) {
        bb <<= 9;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[7] | Files[0])) {
        bb <<= 7;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[7])) {
        bb >>= 7;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << bishop;
    while(bb & ~(Ranks[0] | Files[0])) {
        bb >>= 9;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    return false;
}


void rookMoves(Square rook, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << rook;
    while(bb & ~Ranks[7]) {
        bb <<= 8;
        if(bb & ours) break;
        moves.add(Move(rook, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Files[7]) {
        bb <<= 1;
        if(bb & ours) break;
        moves.add(Move(rook, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Ranks[0]) {
        bb >>= 8;
        if(bb & ours) break;
        moves.add(Move(rook, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Files[0]) {
        bb >>= 1;
        if(bb & ours) break;
        moves.add(Move(rook, builtin::lsb(bb)));
        if(bb & theirs) break;
    }
}

void rookCaptures(Square rook, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << rook;
    while(bb & ~Ranks[7]) {
        bb <<= 8;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(rook, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << rook;
    while(bb & ~Files[7]) {
        bb <<= 1;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(rook, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << rook;
    while(bb & ~Ranks[0]) {
        bb >>= 8;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(rook, builtin::lsb(bb)));
            break;
        }
    }
    bb = 1ULL << rook;
    while(bb & ~Files[0]) {
        bb >>= 1;
        if(bb & ours) break;
        if(bb & theirs) {
            moves.add(Move(rook, builtin::lsb(bb)));
            break;
        }
    }
}

void rookQuiets(Square rook, Bitboard ours, Bitboard theirs, Movelist &moves) {
    Bitboard bb = 1ULL << rook;
    while(bb & ~Ranks[7]) {
        bb <<= 8;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(rook, builtin::lsb(bb)));
    }
    bb = 1ULL << rook;
    while(bb & ~Files[7]) {
        bb <<= 1;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(rook, builtin::lsb(bb)));
    }
    bb = 1ULL << rook;
    while(bb & ~Ranks[0]) {
        bb >>= 8;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(rook, builtin::lsb(bb)));
    }
    bb = 1ULL << rook;
    while(bb & ~Files[0]) {
        bb >>= 1;
        if(bb & ours) break;
        if(bb & theirs) break;
        moves.add(Move(rook, builtin::lsb(bb)));
    }
}

bool rookPseudoLegal(Square rook, Bitboard ours, Bitboard theirs, Bitboard target) {
    Bitboard bb = 1ULL << rook;
    while(bb & ~Ranks[7]) {
        bb <<= 8;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Files[7]) {
        bb <<= 1;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Ranks[0]) {
        bb >>= 8;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    bb = 1ULL << rook;
    while(bb & ~Files[0]) {
        bb >>= 1;
        if(bb & ours) break;
        if(bb & target) return true;
        if(bb & theirs) break;
    }
    return false;
}


void castlingMoves(Bitboard occupied, CastlingRights rights, Color c, Movelist &moves) {
    // Assumes castling rights are correct
    Square kingFrom = (c == Color::White ? 4 : 60);

    for(const auto side : {CastleSide::KING_SIDE, CastleSide::QUEEN_SIDE}) {
        if(!rights.hasCastlingRight(c, side)) continue;
        if(occupied & CastleMask[(int)c][(int)side]) continue;

        Square kingTo = (side == CastleSide::KING_SIDE ? 6 : 2) ^ ((int)c * 56);
        moves.add(Move(kingFrom, kingTo, pokechess::chess::PieceType::Knight, Move::Castling));
    }
}

bool castlingPseudoLegal(Bitboard occupied, CastlingRights rights, Color c, Square target) {
    Square kingFrom = (c == Color::White ? 4 : 60);

    for(const auto side : {CastleSide::KING_SIDE, CastleSide::QUEEN_SIDE}) {
        if(!rights.hasCastlingRight(c, side)) continue;
        if(occupied & CastleMask[(int)c][(int)side]) continue;

        Square kingTo = (side == CastleSide::KING_SIDE ? 6 : 2) ^ ((int)c * 56);
        if(kingTo == target) return true;
    }
    return false;
}

} // namespace movegen

class Board {
private:
    void placePiece(Piece piece, Square sq);
    // void placePiece(Piece piece, Square sq, Accumulator &acc);

    void removePiece(Piece piece, Square sq);
    // void removePiece(Piece piece, Square sq, Accumulator &acc);

    uint64_t pieces_[2][6];

    std::array<Piece, 64> board_{};
    std::array<pokemon::Type, 64> types_{};

    uint64_t hash_ = 0ULL;

    CastlingRights castlingRights;
    uint16_t fullMoves = 2;

    Color sideToMove_ = Color::White;
    uint8_t halfMoves = 0;

    Square seSquare = 64;
    Square epSquare = 64;
    
public:
    Board(std::string fen, bool randomizeTeams);

    void setFen(std::string fen);
    void setFenRandom(std::string fen);
    std::string getFen();

    void makeMove(Move move);
    void makeMove(Move move, nnue::Accumulator &makeMove);

    bool inCheck();

    Bitboard us(Color color) { 
        return pieces(PieceType::Pawn, color) | pieces(PieceType::Knight, color) |
        pieces(PieceType::Bishop, color) | pieces(PieceType::Rook, color) |
        pieces(PieceType::Queen, color) | pieces(PieceType::King, color); 
    };
    Bitboard them(Color color) {
        return us((Color)((int)color ^ 1));
    }

    Bitboard all() {
        return us(Color::White) | us(Color::Black);
    };

    Bitboard pieces(PieceType type, Color color) {
        return pieces_[(int)color][(int)type];
    }

    Piece at(Square sq) {
        return board_[sq];
    }

    Square getEP() {
        return epSquare;
    }
    Square getSE() {
        return seSquare;
    }

    pokemon::Type typeAt(Square sq) {
        return types_[sq];
    }

    uint64_t hash() {
        return hash_;
    }
    Color sideToMove() {
        return sideToMove_;
    }
    int halfMoveClock() {
        return halfMoves;
    }
    int fullMoveClock() {
        return fullMoves;
    }

    CastlingRights getCastlingRights() {
        return castlingRights;
    }

    GameResult isGameOver() {
        if(builtin::popcount(pieces_[0][5] | pieces_[1][5]) == 0) return GameResult::DRAW;
        if(builtin::popcount(pieces(PieceType::King, sideToMove())) == 0) return GameResult::LOSE;
        if(builtin::popcount(pieces(PieceType::King, Color((int)sideToMove_ ^ 1))) == 0) return GameResult::WIN;
        if(halfMoves >= 100) return GameResult::DRAW;
        return GameResult::NONE;
    }

    std::string getCastleString() const;

    uint64_t zobrist();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};

Board::Board(std::string fen, bool randomizeTeams) {
    if(randomizeTeams) setFenRandom(fen);
    else setFen(fen);
}

void Board::setFen(std::string fen) {
    std::fill(std::begin(board_), std::end(board_), Piece::None);
    std::fill(std::begin(types_), std::end(types_), pokemon::Type::None);

    for (const auto c : {Color::White, Color::Black}) {
        for (int i = 0; i < 6; i++) {
            pieces_[(int)c][i] = 0ULL;
        }
    }

    const std::vector<std::string> params = splitString(fen, ' ');

    const std::string &position = params[0];
    const std::string &move_right = params[1];
    const std::string &castling = params[2];
    const std::string &en_passant = params[3];
    const std::string &se_square = params[6];

    halfMoves = std::stoi(params.size() > 4 ? params[4] : "0");
    fullMoves = std::stoi(params.size() > 4 ? params[5] : "1") * 2;

    sideToMove_ = (move_right == "w") ? Color::White : Color::Black;

    auto square = Square(56);
    for (char curr : position) {
        if (charToPiece(curr) != Piece::None) {
            const Piece piece = charToPiece(curr);
            placePiece(piece, square);
        } else if (curr == '/')
            square = Square(square - 16);
        else if (isdigit(curr)) {
            square = Square(square + (curr - '0'));
        }
        // Assuming that this is a type
        else {
            for(int i = 0; i < 18; i++) {
                if(pokemon::typeToChar((pokemon::Type)i) == curr) {
                    types_[square] = (pokemon::Type)i;
                }
            }
            square = Square(square + 1);
        }
    }

    castlingRights.clearAllCastlingRights();

    for (char i : castling) {
        if (i == '-') break;
        if (i == 'K')
            castlingRights.setCastlingRight<Color::White, CastleSide::KING_SIDE, 7>();
        if (i == 'Q')
            castlingRights.setCastlingRight<Color::White, CastleSide::QUEEN_SIDE, 0>();
        if (i == 'k')
            castlingRights.setCastlingRight<Color::Black, CastleSide::KING_SIDE, 7>();
        if (i == 'q')
            castlingRights.setCastlingRight<Color::Black, CastleSide::QUEEN_SIDE, 0>();
    }

    if (en_passant == "-") {
        epSquare = 64;
    } else {
        const char letter = en_passant[0];
        const int file = letter - 96;
        const int rank = en_passant[1] - 48;
        epSquare = Square((rank - 1) * 8 + file - 1);
    }

    if (se_square == "-") {
        seSquare = 64;
    } else {
        const char letter = se_square[0];
        const int file = letter - 96;
        const int rank = se_square[1] - 48;
        seSquare = Square((rank - 1) * 8 + file - 1);
    }

    hash_ = zobrist();
}

void Board::setFenRandom(std::string fen) {
    std::fill(std::begin(board_), std::end(board_), Piece::None);
    std::fill(std::begin(types_), std::end(types_), pokemon::Type::None);

    for (const auto c : {Color::White, Color::Black}) {
        for (int i = 0; i < 6; i++) {
            pieces_[(int)c][i] = 0ULL;
        }
    }

    const std::vector<std::string> params = splitString(fen, ' ');

    const std::string &position = params[0];
    const std::string &move_right = params[1];
    const std::string &castling = params[2];
    const std::string &en_passant = params[3];
    const std::string &se_square = params[6];

    halfMoves = std::stoi(params.size() > 4 ? params[4] : "0");
    fullMoves = std::stoi(params.size() > 4 ? params[5] : "1") * 2;

    sideToMove_ = (move_right == "w") ? Color::White : Color::Black;

    auto square = Square(56);

    bool wType[18] = {false};
    bool bType[18] = {false};

    for (char curr : position) {
        if (charToPiece(curr) != Piece::None) {
            const Piece piece = charToPiece(curr);
            placePiece(piece, square);

            pokemon::Type t = (pokemon::Type)(rand() % 18);
            if((int)piece / 6 == 0) {
                while(wType[(int)t]) t = (pokemon::Type)(rand() % 18);
                wType[(int)t] = true;
            } else {
                while(bType[(int)t]) t = (pokemon::Type)(rand() % 18);
                bType[(int)t] = true;
            }

            types_[square] = t;

            square = Square(square + 1);
        } else if (curr == '/')
            square = Square(square - 16);
        else if (isdigit(curr)) {
            square = Square(square + (curr - '0'));
        }
    }

    castlingRights.clearAllCastlingRights();

    for (char i : castling) {
        if (i == '-') break;
        if (i == 'K')
            castlingRights.setCastlingRight<Color::White, CastleSide::KING_SIDE, 7>();
        if (i == 'Q')
            castlingRights.setCastlingRight<Color::White, CastleSide::QUEEN_SIDE, 0>();
        if (i == 'k')
            castlingRights.setCastlingRight<Color::Black, CastleSide::KING_SIDE, 7>();
        if (i == 'q')
            castlingRights.setCastlingRight<Color::Black, CastleSide::QUEEN_SIDE, 0>();
    }

    if (en_passant == "-") {
        epSquare = 64;
    } else {
        const char letter = en_passant[0];
        const int file = letter - 96;
        const int rank = en_passant[1] - 48;
        epSquare = Square((rank - 1) * 8 + file - 1);
    }

    if (se_square == "-") {
        seSquare = 64;
    } else {
        const char letter = se_square[0];
        const int file = letter - 96;
        const int rank = se_square[1] - 48;
        seSquare = Square((rank - 1) * 8 + file - 1);
    }

    hash_ = zobrist();
}

std::string Board::getFen() {
    std::stringstream ss;

    // Loop through the ranks of the board in reverse order
    for (int rank = 7; rank >= 0; rank--) {
        int free_space = 0;

        // Loop through the files of the board
        for (int file = 0; file < 8; file++) {
            // Calculate the square index
            const int sq = rank * 8 + file;

            // Get the piece at the current square
            const Piece piece = at(Square(sq));

            // If there is a piece at the current square
            if (piece != Piece::None) {
                // If there were any empty squares before this piece,
                // append the number of empty squares to the FEN string
                if (free_space) {
                    ss << free_space;
                    free_space = 0;
                }

                // Append the character representing the piece to the FEN string
                ss << pieceToCharArray[(int)piece];
                ss << pokemon::typeToChar(types_[sq]);
            } else {
                // If there is no piece at the current square, increment the
                // counter for the number of empty squares
                free_space++;
            }
        }

        // If there are any empty squares at the end of the rank,
        // append the number of empty squares to the FEN string
        if (free_space != 0) {
            ss << free_space;
        }

        // Append a "/" character to the FEN string, unless this is the last rank
        ss << (rank > 0 ? "/" : "");
    }

    // Append " w " or " b " to the FEN string, depending on which player's turn it is
    ss << (sideToMove_ == Color::White ? " w " : " b ");

    // Append the appropriate characters to the FEN string to indicate
    // whether castling is allowed for each player
    ss << getCastleString();
    if (castlingRights.isEmpty()) ss << "-";

    // Append information about the en passant square (if any)
    // and the half-move clock and full move number to the FEN string
    if (epSquare == 64)
        ss << " - ";
    else
        ss << " " << squareToString[epSquare] << " ";

    ss << int(halfMoves) << " " << int(fullMoves / 2);

    if(seSquare == 64) {
        ss << " -";
    } else {
        ss << " " << squareToString[seSquare] << " ";
    }

    // Return the resulting FEN string
    return ss.str();
}

uint64_t Board::zobrist() {
    uint64_t hash_key = 0ULL;

    uint64_t wPieces = us(Color::White);
    uint64_t bPieces = us(Color::Black);

    while (wPieces) {
        const Square sq = builtin::poplsb(wPieces);
        hash_key ^= zobrist::piece(at(sq), sq);
    }
    while (bPieces) {
        const Square sq = builtin::poplsb(bPieces);
        hash_key ^= zobrist::piece(at(sq), sq);
    }

    uint64_t ep_hash = 0ULL;
    if (epSquare != 64) ep_hash ^= zobrist::enpassant(epSquare & 7);

    uint64_t side_to_move_hash = 0ULL;
    if (sideToMove_ == Color::White) side_to_move_hash ^= zobrist::sideToMove();

    // Castle hash

    uint64_t castling_hash = 0ULL;
    castling_hash ^= zobrist::castling(castlingRights.getHashIndex());

    return hash_key ^ ep_hash ^ side_to_move_hash ^ castling_hash;
}

void Board::placePiece(Piece piece, Square sq) {
    hash_ ^= zobrist::piece(piece, sq);
    pieces_[static_cast<int>(piece) / 6][static_cast<int>(piece) % 6] |=
        (1ULL << sq);
    board_[sq] = piece;
}

// void Board::placePiece(Piece piece, Square sq, Accumulator &acc) {
//     hash_ ^= zobrist::piece(piece, sq);
//     pieces_[static_cast<int>(piece) / 6][static_cast<int>(piece) % 6] |=
//         (1ULL << sq);
//     board_[sq] = piece;

//     assert((int)piece * 64 + (int)sq < 768);
//     assert(pieceSwap[(int)piece] * 64 + ((int)sq ^ 56) < 768);

//     acc.phase += phaseScore[(int)piece % 6];

//     acc.white_mg += PST_mg[(int)piece * 64 + (int)sq];
//     acc.white_eg += PST_eg[(int)piece * 64 + (int)sq];

//     acc.black_mg += PST_mg[pieceSwap[(int)piece] * 64 + ((int)sq ^ 56)];
//     acc.black_eg += PST_eg[pieceSwap[(int)piece] * 64 + ((int)sq ^ 56)];
// }

void Board::removePiece(Piece piece, Square sq) {
    hash_ ^= zobrist::piece(piece, sq);
    pieces_[static_cast<int>(piece) / 6][static_cast<int>(piece) % 6] ^= (1ULL << sq);
    board_[sq] = Piece::None;
}

// void Board::removePiece(Piece piece, Square sq, Accumulator &acc) {
//     hash_ ^= zobrist::piece(piece, sq);
//     pieces_[static_cast<int>(piece) / 6][static_cast<int>(piece) % 6] ^= (1ULL << sq);
//     board_[sq] = Piece::None;
//
//     assert((int)piece * 64 + (int)sq < 768);
//     assert(pieceSwap[(int)piece] * 64 + ((int)sq ^ 56) < 768);
//    
//     acc.phase -= phaseScore[(int)piece % 6];
//   
//     acc.white_mg -= PST_mg[(int)piece * 64 + (int)sq];
//     acc.white_eg -= PST_eg[(int)piece * 64 + (int)sq];
//
//     acc.black_mg -= PST_mg[pieceSwap[(int)piece] * 64 + ((int)sq ^ 56)];
//     acc.black_eg -= PST_eg[pieceSwap[(int)piece] * 64 + ((int)sq ^ 56)];
// }

inline std::ostream &operator<<(std::ostream &os, const Board &b) {
    for (int i = 63; i >= 0; i -= 8) {
        os << " " << pieceToCharArray[(int)b.board_[i - 7]] << " " << pokemon::typeToString(b.types_[i - 7]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 6]] << " " << pokemon::typeToString(b.types_[i - 6]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 5]] << " " << pokemon::typeToString(b.types_[i - 5]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 4]] << " " << pokemon::typeToString(b.types_[i - 4]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 3]] << " " << pokemon::typeToString(b.types_[i - 3]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 2]] << " " << pokemon::typeToString(b.types_[i - 2]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 1]] << " " << pokemon::typeToString(b.types_[i - 1]) << " ";
        os << " " << pieceToCharArray[(int)b.board_[i - 0]] << " " << pokemon::typeToString(b.types_[i - 0]) << " ";
        os << std::endl << std::endl;;
    }
    os << "Side to move: " << static_cast<int>(b.sideToMove_) << "\n";
    os << "Castling rights: " << b.getCastleString() << "\n";
    os << "Halfmoves: " << static_cast<int>(b.halfMoves) << "\n";
    os << "Fullmoves: " << static_cast<int>(b.fullMoves) / 2 << "\n";
    // os << "EP: " << static_cast<int>(b.epSquare) << "\n";
    // os << "SE: " << static_cast<int>(b.seSquare) << "\n";
    // os << "Hash: " << b.hash_ << "\n";

    // os << std::endl;
    return os;
}

std::string Board::getCastleString() const {
    std::stringstream ss;
    if (castlingRights.hasCastlingRight(Color::White, CastleSide::KING_SIDE)) ss << "K";
    if (castlingRights.hasCastlingRight(Color::White, CastleSide::QUEEN_SIDE)) ss << "Q";
    if (castlingRights.hasCastlingRight(Color::Black, CastleSide::KING_SIDE)) ss << "k";
    if (castlingRights.hasCastlingRight(Color::Black, CastleSide::QUEEN_SIDE)) ss << "q";

    return ss.str();
}

void Board::makeMove(Move move) {
    // Handles the case of a null move (one player skipping their turn during an SE chain)
    if(move.from() == move.to()) {
        nullmove:
        State s;
        s.hash = hash_;
        s.half_moves = halfMoves;

        fullMoves++;

        hash_ ^= zobrist::sideToMove();
        if(epSquare != 64) hash_ ^= zobrist::enpassant(epSquare & 7);
        epSquare = 64;
        seSquare = 64;

        sideToMove_ = (Color)((int)sideToMove_ ^ 1);
        return;
    }

    bool capture = at(move.to()) != Piece::None && move.typeOf() != Move::Castling;
    Piece captured = at(move.to());
    PieceType pt = (PieceType)((int)at(move.from()) % 6);
    assert(pt != PieceType::None);
    pokemon::Effectiveness interaction = pokemon::Effectiveness::Neutral;

    halfMoves++;
    fullMoves++;

    if(epSquare != 64) hash_ ^= zobrist::enpassant(epSquare & 7);

    hash_ ^= zobrist::castling(castlingRights.getHashIndex());

    if(capture) {
        interaction = pokemon::lookupMoveEffectiveness(typeAt(move.from()), typeAt(move.to()));
        if(interaction == pokemon::Effectiveness::Immune) goto nullmove;
        halfMoves = 0;

        removePiece(captured, move.to());
        types_[move.to()] = pokemon::Type::None;

        Rank rank = move.to() >> 3;

        if((PieceType)((int)captured % 6) == PieceType::Rook && 
        ((rank == 0 && sideToMove_ == Color::Black) || 
        (rank == 7 && sideToMove_ == Color::White))) {
            Square kingSq = builtin::lsb(pieces_[(int)sideToMove_ ^ 1][5]);
            auto file = (move.to() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);

            if(castlingRights.getRookFile(Color((int)sideToMove_ ^ 1), file) == ((int)move.to() & 7)) {
                castlingRights.clearCastlingRight(Color((int)sideToMove_ ^ 1), file);
            }
        }
    }
    if(move.typeOf() == Move::EnPassant) {
        interaction = pokemon::lookupMoveEffectiveness(typeAt(move.from()), typeAt(epSquare ^ 8));
        if(interaction == pokemon::Effectiveness::Immune) goto nullmove;
        removePiece(Piece(((int)sideToMove_ ^ 1) * 6 + (int)PieceType::Pawn), Square(int(move.to()) ^ 8));
        types_[Square(int(move.to()) ^ 8)] = pokemon::Type::None;
    }

    if(interaction != pokemon::Effectiveness::SuperEffective) epSquare = 64;

    if(pt == PieceType::King) {
        castlingRights.clearCastlingRight(sideToMove_);
    }
    else if (pt == PieceType::Rook && sideToMove_ == Color::White && (((int)move.from() >> 3) == 0)) {
        Square kingSq = builtin::lsb(pieces_[(int)sideToMove_][5]);
        auto file = (move.from() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);
        if(castlingRights.getRookFile(sideToMove_, file) == ((int)move.from() & 7)) {
            castlingRights.clearCastlingRight(sideToMove_, file);
        }
    }
    else if (pt == PieceType::Rook && sideToMove_ == Color::Black && (((int)move.from() >> 3) == 7)) {
        Square kingSq = builtin::lsb(pieces_[(int)sideToMove_][5]);
        auto file = (move.from() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);
        if(castlingRights.getRookFile(sideToMove_, file) == ((int)move.from() & 7)) {
            castlingRights.clearCastlingRight(sideToMove_, file);
        }
    }
    // If the move was a pawn double move, set EP
    else if (pt == PieceType::Pawn) {
        halfMoves = 0;

        if(std::abs(int(move.to()) - int(move.from())) == 16) {
            epSquare = (int)move.to() ^ 8;
            hash_ ^= zobrist::enpassant(epSquare & 7);
        }
    }

    captureDone:

    if(move.typeOf() == Move::Castling) {
        bool kingSide = move.to() > move.from();
        Square rookTo = (kingSide ? 5 : 3) ^ (56 * (int)sideToMove_);
        Square kingTo = (kingSide ? 6 : 2) ^ (56 * (int)sideToMove_);

        Square rookFrom = (kingSide ? 7 : 0) ^ (56 * (int)sideToMove_);

        assert(at(rookFrom) == Piece::WhiteRook || at(rookFrom) == Piece::BlackRook);
        assert(at(move.from()) == Piece::WhiteKing || at(move.from()) == Piece::BlackKing);

        const auto king = at(move.from());
        const auto rook = at(rookFrom);

        removePiece(king, move.from());
        removePiece(rook, rookFrom);

        placePiece(king, kingTo);
        placePiece(rook, rookTo);

        types_[kingTo] = types_[move.from()];
        types_[rookTo] = types_[rookFrom];

        types_[move.from()] = pokemon::Type::None;
        types_[rookFrom] = pokemon::Type::None;
    } 
    else if (move.typeOf() == Move::Promotion) {
        removePiece(at(move.from()), move.from());
        if(interaction != pokemon::Effectiveness::Resist) {
            placePiece((Piece)((int)move.promotionType() + int(sideToMove_ == Color::White ? 0 : 6)), move.to());
            types_[move.to()] = types_[move.from()];
        }
        types_[move.from()] = pokemon::Type::None;
    }
    else {
        auto piece = at(move.from());

        removePiece(piece, move.from());

        if(interaction != pokemon::Effectiveness::Resist) {
            placePiece(piece, move.to());
            types_[move.to()] = types_[move.from()];
        }
        types_[move.from()] = pokemon::Type::None;
    }

    hash_ ^= zobrist::sideToMove();
    hash_ ^= zobrist::castling(castlingRights.getHashIndex());

    if(interaction == pokemon::Effectiveness::SuperEffective) {
        seSquare = move.to();
    }
    else {
        sideToMove_ = (Color)((int)sideToMove_ ^ 1);
        seSquare = 64;
    }
}

void Board::makeMove(Move move, nnue::Accumulator &acc) {
    // Handles the case of a null move (one player skipping their turn during an SE chain)
    if(move.from() == move.to()) {
        nullmove:
        State s;
        s.hash = hash_;
        s.half_moves = halfMoves;

        fullMoves++;

        hash_ ^= zobrist::sideToMove();
        if(epSquare != 64) hash_ ^= zobrist::enpassant(epSquare & 7);
        epSquare = 64;
        seSquare = 64;

        sideToMove_ = (Color)((int)sideToMove_ ^ 1);
        return;
    }

    bool capture = at(move.to()) != Piece::None && move.typeOf() != Move::Castling;
    Piece captured = at(move.to());
    PieceType pt = (PieceType)((int)at(move.from()) % 6);
    assert(pt != PieceType::None);
    pokemon::Effectiveness interaction = pokemon::Effectiveness::Neutral;

    halfMoves++;
    fullMoves++;

    if(epSquare != 64) hash_ ^= zobrist::enpassant(epSquare & 7);

    hash_ ^= zobrist::castling(castlingRights.getHashIndex());

    if(capture) {
        interaction = pokemon::lookupMoveEffectiveness(typeAt(move.from()), typeAt(move.to()));
        if(interaction == pokemon::Effectiveness::Immune) goto nullmove;
        halfMoves = 0;

        acc.sub((int)types_[move.to()], (int)at(move.to()), move.to());
        removePiece(captured, move.to());
        types_[move.to()] = pokemon::Type::None;

        Rank rank = move.to() >> 3;

        if((PieceType)((int)captured % 6) == PieceType::Rook && 
        ((rank == 0 && sideToMove_ == Color::Black) || 
        (rank == 7 && sideToMove_ == Color::White))) {
            Square kingSq = builtin::lsb(pieces_[(int)sideToMove_ ^ 1][5]);
            auto file = (move.to() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);

            if(castlingRights.getRookFile(Color((int)sideToMove_ ^ 1), file) == ((int)move.to() & 7)) {
                castlingRights.clearCastlingRight(Color((int)sideToMove_ ^ 1), file);
            }
        }
    }
    if(move.typeOf() == Move::EnPassant) {
        interaction = pokemon::lookupMoveEffectiveness(typeAt(move.from()), typeAt(epSquare ^ 8));
        if(interaction == pokemon::Effectiveness::Immune) goto nullmove;
        acc.sub((int)types_[Square(int(move.to()) ^ 8)], (int)Piece(((int)sideToMove_ ^ 1) * 6 + (int)PieceType::Pawn), int(move.to()) ^ 8);
        removePiece(Piece(((int)sideToMove_ ^ 1) * 6 + (int)PieceType::Pawn), Square(int(move.to()) ^ 8));
        types_[Square(int(move.to()) ^ 8)] = pokemon::Type::None;
    }

    if(interaction != pokemon::Effectiveness::SuperEffective) epSquare = 64;

    if(pt == PieceType::King) {
        castlingRights.clearCastlingRight(sideToMove_);
    }
    else if (pt == PieceType::Rook && sideToMove_ == Color::White && (((int)move.from() >> 3) == 0)) {
        Square kingSq = builtin::lsb(pieces_[(int)sideToMove_][5]);
        auto file = (move.from() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);
        if(castlingRights.getRookFile(sideToMove_, file) == ((int)move.from() & 7)) {
            castlingRights.clearCastlingRight(sideToMove_, file);
        }
    }
    else if (pt == PieceType::Rook && sideToMove_ == Color::Black && (((int)move.from() >> 3) == 7)) {
        Square kingSq = builtin::lsb(pieces_[(int)sideToMove_][5]);
        auto file = (move.from() > kingSq ? CastleSide::KING_SIDE : CastleSide::QUEEN_SIDE);
        if(castlingRights.getRookFile(sideToMove_, file) == ((int)move.from() & 7)) {
            castlingRights.clearCastlingRight(sideToMove_, file);
        }
    }
    // If the move was a pawn double move, set EP
    else if (pt == PieceType::Pawn) {
        halfMoves = 0;

        if(std::abs(int(move.to()) - int(move.from())) == 16) {
            epSquare = (int)move.to() ^ 8;
            hash_ ^= zobrist::enpassant(epSquare & 7);
        }
    }

    captureDone:

    if(move.typeOf() == Move::Castling) {
        bool kingSide = move.to() > move.from();
        Square rookTo = (kingSide ? 5 : 3) ^ (56 * (int)sideToMove_);
        Square kingTo = (kingSide ? 6 : 2) ^ (56 * (int)sideToMove_);

        Square rookFrom = (kingSide ? 7 : 0) ^ (56 * (int)sideToMove_);

        assert(at(rookFrom) == Piece::WhiteRook || at(rookFrom) == Piece::BlackRook);
        assert(at(move.from()) == Piece::WhiteKing || at(move.from()) == Piece::BlackKing);

        const auto king = at(move.from());
        const auto rook = at(rookFrom);
        
        acc.sub((int)types_[move.from()], (int)king, move.from());
        acc.sub((int)types_[rookFrom], (int)rook, rookFrom);

        removePiece(king, move.from());
        removePiece(rook, rookFrom);

        acc.add((int)types_[move.from()], (int)king, kingTo);
        acc.add((int)types_[rookFrom], (int)rook, rookTo);

        placePiece(king, kingTo);
        placePiece(rook, rookTo);

        types_[kingTo] = types_[move.from()];
        types_[rookTo] = types_[rookFrom];

        types_[move.from()] = pokemon::Type::None;
        types_[rookFrom] = pokemon::Type::None;
    } 
    else if (move.typeOf() == Move::Promotion) {
        acc.sub((int)types_[move.from()], (int)at(move.from()), move.from());
        removePiece(at(move.from()), move.from());

        if(interaction != pokemon::Effectiveness::Resist) {
            acc.add((int)types_[move.from()], (int)move.promotionType() + int(sideToMove_ == Color::White ? 0 : 6), move.to());
            placePiece((Piece)((int)move.promotionType() + int(sideToMove_ == Color::White ? 0 : 6)), move.to());
            types_[move.to()] = types_[move.from()];
        }
        types_[move.from()] = pokemon::Type::None;
    }
    else {
        auto piece = at(move.from());

        acc.sub((int)types_[move.from()], int(piece), move.from());
        removePiece(piece, move.from());

        if(interaction != pokemon::Effectiveness::Resist) {
            acc.add((int)types_[move.from()], int(piece), move.to());
            placePiece(piece, move.to());
            types_[move.to()] = types_[move.from()];
        }
        types_[move.from()] = pokemon::Type::None;
    }

    hash_ ^= zobrist::sideToMove();
    hash_ ^= zobrist::castling(castlingRights.getHashIndex());

    if(interaction == pokemon::Effectiveness::SuperEffective) {
        seSquare = move.to();
    }
    else {
        sideToMove_ = (Color)((int)sideToMove_ ^ 1);
        seSquare = 64;
    }
}

bool Board::inCheck() {
    Color c = (Color)((int)sideToMove() ^ 1);

    Bitboard pawns = pieces(PieceType::Pawn, c);
    Bitboard bishops = pieces(PieceType::Bishop, c);
    Bitboard knights = pieces(PieceType::Knight, c);
    Bitboard rooks = pieces(PieceType::Rook, c);
    Bitboard queens = pieces(PieceType::Queen, c);
    Bitboard kings = pieces(PieceType::King, c);

    Bitboard ourKing = pieces(PieceType::King, sideToMove());

    Bitboard stm = pawns | bishops | knights | rooks | queens | kings;
    Bitboard nstm = us(sideToMove());
    
    if(ourKing & movegen::pawnAttackSquares(pawns, c)) return true;
    if(ourKing & movegen::knightAttackSquares(knights)) return true;
    if(ourKing & movegen::bishopAttackSquares(bishops, stm, nstm)) return true;
    if(ourKing & movegen::rookAttackSquares(rooks, stm, nstm)) return true;
    if(ourKing & movegen::kingAttackSquares(kings)) return true;

    if(ourKing & movegen::bishopAttackSquares(queens, stm, nstm)) return true;
    if(ourKing & movegen::rookAttackSquares(queens, stm, nstm)) return true;

    return false;
}


void legalmoves(Movelist &movelist, Board &board) {
    movelist.clear();

    Color c = board.sideToMove();

    Bitboard pawns = board.pieces(PieceType::Pawn, board.sideToMove());
    Bitboard bishops = board.pieces(PieceType::Bishop, board.sideToMove());
    Bitboard knights = board.pieces(PieceType::Knight, board.sideToMove());
    Bitboard rooks = board.pieces(PieceType::Rook, board.sideToMove());
    Bitboard queens = board.pieces(PieceType::Queen, board.sideToMove());
    Bitboard kings = board.pieces(PieceType::King, board.sideToMove());

    Bitboard us = pawns | bishops | knights | rooks | queens | kings;
    Bitboard them = board.them(board.sideToMove());
    Bitboard occ = us | them;

    Square EP = board.getEP();
    Square SE = board.getSE();

    if(SE != 64) {
        Bitboard SEPiece = 1ULL << SE;
        if((PieceType)((int)board.at(SE) % 6) == PieceType::Pawn) {
            movegen::pawnPushes(SE, occ, c, movelist);
            movegen::pawnCaptures(SE, them, c, EP, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Knight) {
            movegen::knightMoves(SE, us, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Bishop) {
            movegen::bishopMoves(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Rook) {
            movegen::rookMoves(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Queen) {
            movegen::rookMoves(SE, us, them, movelist);
            movegen::bishopMoves(SE, us, them, movelist);
        } else {
            movegen::kingMoves(SE, us, movelist);
        }
        movelist.add(Move(0, 0));
        return;
    }

    while(pawns) {
        movegen::pawnPushes(builtin::lsb(pawns), occ, c, movelist);
        movegen::pawnCaptures(builtin::poplsb(pawns), them, c, EP, movelist);
    }
    while(knights) {
        movegen::knightMoves(builtin::poplsb(knights), us, movelist);
    }
    while(bishops) {
        movegen::bishopMoves(builtin::poplsb(bishops), us, them, movelist);
    }
    while(rooks) {
        movegen::rookMoves(builtin::poplsb(rooks), us, them, movelist);
    }
    while(queens) {
        movegen::rookMoves(builtin::lsb(queens), us, them, movelist);
        movegen::bishopMoves(builtin::poplsb(queens), us, them, movelist);
    }
    while(kings) {
        movegen::kingMoves(builtin::poplsb(kings), us, movelist);
    }
    movegen::castlingMoves(occ, board.getCastlingRights(), c, movelist);
}

void legalcaptures(Movelist &movelist, Board &board) {
    movelist.clear();

    Color c = board.sideToMove();

    Bitboard pawns = board.pieces(PieceType::Pawn, board.sideToMove());
    Bitboard bishops = board.pieces(PieceType::Bishop, board.sideToMove());
    Bitboard knights = board.pieces(PieceType::Knight, board.sideToMove());
    Bitboard rooks = board.pieces(PieceType::Rook, board.sideToMove());
    Bitboard queens = board.pieces(PieceType::Queen, board.sideToMove());
    Bitboard kings = board.pieces(PieceType::King, board.sideToMove());

    Bitboard us = pawns | bishops | knights | rooks | queens | kings;
    Bitboard them = board.them(board.sideToMove());
    Bitboard occ = us | them;

    Square EP = board.getEP();
    Square SE = board.getSE();

    if(SE != 64) {
        Bitboard SEPiece = 1ULL << SE;
        if((PieceType)((int)board.at(SE) % 6) == PieceType::Pawn) {
            movegen::pawnCaptures(SE, them, c, EP, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Knight) {
            movegen::knightCaptures(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Bishop) {
            movegen::bishopCaptures(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Rook) {
            movegen::rookCaptures(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Queen) {
            movegen::rookCaptures(SE, us, them, movelist);
            movegen::bishopCaptures(SE, us, them, movelist);
        } else {
            movegen::kingCaptures(SE, us, them, movelist);
        }
        // movelist.add(Move(0, 0));
        return;
    }

    while(pawns) {
        movegen::pawnCaptures(builtin::poplsb(pawns), them, c, EP, movelist);
    }
    while(knights) {
        movegen::knightCaptures(builtin::poplsb(knights), us, them, movelist);
    }
    while(bishops) {
        movegen::bishopCaptures(builtin::poplsb(bishops), us, them, movelist);
    }
    while(rooks) {
        movegen::rookCaptures(builtin::poplsb(rooks), us, them, movelist);
    }
    while(queens) {
        movegen::rookCaptures(builtin::lsb(queens), us, them, movelist);
        movegen::bishopCaptures(builtin::poplsb(queens), us, them, movelist);
    }
    while(kings) {
        movegen::kingCaptures(builtin::poplsb(kings), us, them, movelist);
    }
}

void legalquiets(Movelist &movelist, Board &board) {
    Color c = board.sideToMove();

    Bitboard pawns = board.pieces(PieceType::Pawn, board.sideToMove());
    Bitboard bishops = board.pieces(PieceType::Bishop, board.sideToMove());
    Bitboard knights = board.pieces(PieceType::Knight, board.sideToMove());
    Bitboard rooks = board.pieces(PieceType::Rook, board.sideToMove());
    Bitboard queens = board.pieces(PieceType::Queen, board.sideToMove());
    Bitboard kings = board.pieces(PieceType::King, board.sideToMove());

    Bitboard us = pawns | bishops | knights | rooks | queens | kings;
    Bitboard them = board.them(board.sideToMove());
    Bitboard occ = us | them;

    Square EP = board.getEP();
    Square SE = board.getSE();

    if(SE != 64) {
        Bitboard SEPiece = 1ULL << SE;
        if((PieceType)((int)board.at(SE) % 6) == PieceType::Pawn) {
            movegen::pawnPushes(SE, occ, c, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Knight) {
            movegen::knightQuiets(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Bishop) {
            movegen::bishopQuiets(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Rook) {
            movegen::rookQuiets(SE, us, them, movelist);
        } else if((PieceType)((int)board.at(SE) % 6) == PieceType::Queen) {
            movegen::rookQuiets(SE, us, them, movelist);
            movegen::bishopQuiets(SE, us, them, movelist);
        } else {
            movegen::kingQuiets(SE, us, them, movelist);
        }
        movelist.add(Move(0, 0));
        return;
    }

    while(pawns) {
        movegen::pawnPushes(builtin::poplsb(pawns), occ, c, movelist);
    }
    while(knights) {
        movegen::knightQuiets(builtin::poplsb(knights), us, them, movelist);
    }
    while(bishops) {
        movegen::bishopQuiets(builtin::poplsb(bishops), us, them, movelist);
    }
    while(rooks) {
        movegen::rookQuiets(builtin::poplsb(rooks), us, them, movelist);
    }
    while(queens) {
        movegen::rookQuiets(builtin::lsb(queens), us, them, movelist);
        movegen::bishopQuiets(builtin::poplsb(queens), us, them, movelist);
    }
    while(kings) {
        movegen::kingQuiets(builtin::poplsb(kings), us, them, movelist);
    }
    movegen::castlingMoves(occ, board.getCastlingRights(), c, movelist);
}


bool isMovePsuedoLegal(Board &board, Move move) {
    if(board.getSE() != 64 && move.from() == 0 and move.to() == 0) return true;
    if(board.getSE() != 64 && board.getSE() != move.from()) return false;
    Piece moving = board.at(move.from());
    if(moving == Piece::None) return false;
    if((int)moving / 6 != (int)board.sideToMove()) return false;
    PieceType movingType = (PieceType)((int)moving % 6);

    Color c = board.sideToMove();

    Bitboard us = board.us(board.sideToMove());
    Bitboard them = board.them(board.sideToMove());
    Bitboard occ = us | them;

    Bitboard target = 1ULL << move.to();

    if(movingType == PieceType::Pawn) {
        return movegen::pawnCapturesPseudoLegal(move.from(), them, c, board.getEP(), target) ||
        movegen::pawnPushPseudoLegal(move.from(), occ, c, target);
    }
    if(movingType == PieceType::Knight) {
        return movegen::knightPseudoLegal(move.from(), us, target);
    }
    if(movingType == PieceType::Bishop) {
        return movegen::bishopPseudoLegal(move.from(), us, them, target);
    }
    if(movingType == PieceType::Rook) {
        return movegen::rookPseudoLegal(move.from(), us, them, target);
    }
    if(movingType == PieceType::Queen) {
        return movegen::bishopPseudoLegal(move.from(), us, them, target) ||
        movegen::rookPseudoLegal(move.from(), us, them, target);
    }
    if(movingType == PieceType::King) {
        return movegen::kingPseudoLegal(move.from(), us, target) ||
        movegen::castlingPseudoLegal(occ, board.getCastlingRights(), c, move.to());
    }
    return false;
}

Move fromUGI(Board &board, std::string ugi) {
    if(ugi == "0000") return Move(0, 0);
    Square source;
    Square target;
    PieceType promotion = PieceType::Knight;
    uint16_t flags = 0;

    // Extracts source
    int file = ugi[0] - 96;
    int rank = ugi[1] - 48;
    source = (rank - 1) * 8 + file - 1;

    // Extracts target
    file = ugi[2] - 96;
    rank = ugi[3] - 48;
    target = (rank - 1) * 8 + file - 1;

    PieceType piece = PieceType((int)board.at(source) % 6);

    // Castling
    if(piece == PieceType::King && 
    (source == 4 || source == 60) &&
    (target == 2 || target == 6 || target == 58 || target == 62)) {
        flags = Move::Castling;
    }

    else if(piece == PieceType::Pawn && target == board.getEP() && ((target & 7) != (source & 7))) {
        flags = Move::EnPassant;
    }

    else if(piece == PieceType::Pawn && (rank == 1 || rank == 8)) {
        flags = Move::Promotion;
        for(PieceType p = PieceType::Pawn; p != PieceType::King; p = PieceType((int)p + 1)) {
            if(ugi[4] == pieceToCharArray[(int)p + 6]) {
                promotion = p;
            }
        }
    }

    return Move(source, target, promotion, flags);
}


} // namespace chess

}; // Namespace pokechess