#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <mutex>
#include <thread>
#include <cstring>
#include <cassert>

#include "xorshift.h"

#include "NNUE.h"

#include "PokeChess/PokeChess.h"
using namespace pokechess;

#include "NNUE.cpp"

#include "Eval.h"
#include "Eval.cpp"

#include "Movepicker.cpp"

#include "TT.cpp"

#include "Search.h"
#include "Search.cpp"

#include "UGI.cpp"

#include "Writer.h"
#include "Generation.h"
#include "Generation.cpp"