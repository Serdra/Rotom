#include "includes.h"

int main() {
    nnue::init("master.nnue");
    srand(time(0));
    std::string input;
    std::getline(std::cin, input);

    if (input == "ugi") UGI();
    if (input == "book") {
        int seed = rand();
        printSearchUpdates = false;
        std::mutex mtx;
        int numThreads = 8;

        std::vector<std::thread> threads;
        for(int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(generateBook, std::ref(mtx), xorshift(seed + i)));
        }
        for(int i = 0; i < numThreads; i++) {
            threads[i].join();
        }
    }
    if (input == "analyze") {
        printSearchUpdates = false;
        TransTable TT(256);
        History Hist;
        ContHistory ContHist;

        std::string startingFen;
        std::string moveStrings;

        std::cout << "Enter the starting fen: ";
        std::getline(std::cin, startingFen);
        std::cout << "Enter all moves (seperated by a space): ";
        std::getline(std::cin, moveStrings);

        chess::Board position(startingFen, false);

        std::vector<std::string> moves = chess::splitString(moveStrings, ' ');

        int moveTime = 1000;
        std::pair<chess::Move, int> prevResult = IterativeDeepening(position, StopType::Time, moveTime, moveTime*2, TT, Hist, ContHist);

        // Validate moves
        for(int i = 0; i < moves.size() && position.isGameOver() == chess::GameResult::NONE; i++) {
            // Check if the move is legal
            chess::Movelist legalMoves;
            chess::legalmoves(legalMoves, position);

            bool valid = false;
            for(int j = 0; j < legalMoves.size(); j++) {
                if(chess::fromUGI(position, moves[i]) == legalMoves[j]) valid = true;
            }
            if(!valid) {
                std::cout << "Illegal move " << moves[i] << std::endl;
                return 0;
            }
            position.makeMove(chess::fromUGI(position, moves[i]));
        }


        position = chess::Board(startingFen, false);
        for(int i = 0; i < moves.size() && position.isGameOver() == chess::GameResult::NONE; i++) {
            position.makeMove(chess::fromUGI(position, moves[i]));
            std::pair<chess::Move, int> result = IterativeDeepening(position, StopType::Time, moveTime, moveTime * 2, TT, Hist, ContHist);
            if(-result.second < (prevResult.second - 80) && (position.getSE() == 64) && prevResult.first != chess::fromUGI(position, moves[i])) {
                std::cout << "Potential blunder " << moves[i] << " best move was " << prevResult.first << " loss " << prevResult.second + result.second << std::endl;
            }
            else if(result.second < (prevResult.second - 80) && (position.getSE() != 64) && prevResult.first != chess::fromUGI(position, moves[i])) {
                std::cout << "Potential blunder " << moves[i] << " best move was " << prevResult.first << " loss " << prevResult.second - result.second << std::endl;
            }
            prevResult = result;
        }
    }
    if (input == "suite") {
        std::vector<std::string> fens = {
            "rcngbtqekubynxri/pzpophpmpfplpwpa/8/8/8/8/PtPhPcPxPmPwPzPg/RdNyBsQaKoBfNuRi w KQkq - 0 1 -",
            "rmnmbmqmkmbmnmrm/pmpmpmpmpmpmpmpm/8/8/8/8/PmPmPmPmPmPmPmPm/RmNmBmQmKmBmNmRm w KQkq - 0 1 -",
            "ru2qx1rcke1/ptpipw2pgpz1/5nd1pm/4nh3/6bs1/PyBf2BlNa2/2Pd2PsPoPm/Rg2QtKx2Rh w KQ - 0 1 b3",
            "ru1byqtklbzngrw/phpxpa1pspcpipm/nf7/1Pf1pdPa3/8/8/Pz1PePd1PxPoPm/RiNuBwQsKyBtNlRg w KQkq d6 0 1 -",
            "rw1byqsktbinura/pcphpg1pfpeplpx/nz7/1Ps1pmPc3/8/8/Pd1PlPh1PzPuPx/RfNmByQaKwBoNeRg w KQkq d6 0 1 e5",
            "re3kt2ro/ps1pcpzqaplbx1/bunh2pgnmpy1/3PsNz3/1pf2Pe3/2Nh2Ql1pd/PaPtPcBoByPuPxPg/Rw3Kf2Rd w KQkq - 0 1 -",
            "rc3ka2ru/pf1pypzqwplbs1/bdnm2ptnipg1/3PdNt3/1ph2Po3/2Nh2Qf1pe/PlPzPmBcBxPwPePs/Ri3Ku2Ry w KQkq - 0 1 f3",
            "rs3ki2rw/pc1phpeqaplbd1/bfnz2pontpy1/3PgNi3/1pg2Po3/2Nw2Qf1pm/PdPuPxBcBsPtPlPz/Ry3Kh2Ra b KQkq - 0 1 -",
            "rw3kg2re/ps1pfpzqypubo1/blnh2pmncpi1/3PaNx3/1pa2Pm3/2Ni2Qw1pd/PsPdPhByBgPfPePl/Rc3Ku2Ro b KQkq - 0 1 b6",
            "1rf4rokx/pg2Rh2nspu/1pz3pmqi1/8/Pc3QuPm2/5Ne1Pg/1BlPz2Ki2/8 w - - 0 1 -",
            "1rh4rckt/pz2Rt2nypw/1pf3peqg1/8/Pu3QfPm2/5No1Ps/1BcPx2Kl2/8 b - - 0 1 -",
            "rt3kh2re/1plpspxpcpopz1/8/8/8/8/1PhPfPtPzPsPo1/Rt3Ki2Ra w KQkq - 0 1 -",
            "rm3kw2rz/1PipdpopfpaPx1/8/8/8/8/1phPsPtPmPopu1/Rc3Ke2Ry w KQkq - 0 1 -",
            "rw3ks2rg/1PwpypmptpaPf1/8/8/8/8/1pmPuPmPaPfpo1/Rt3Kh2Ro b KQkq - 0 1 -"
        };

        for(std::string fen : fens) {
            chess::Board board(fen, false);
            uint64_t nodes = 0;
            perft(board, 6, nodes);
            std::cout << fen << " : " << nodes << std::endl;
        }
    }
    if (input == "datagen") {
        int seed = rand();
        printSearchUpdates = false;

        DataWriter writer("Iteration1_32.bin");
        std::mutex mtx;
        int interval = 1;
        int numThreads = 8;
        WDL wdl;

        std::vector<std::thread> threads;
        for(int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(generateData, std::ref(writer), std::ref(mtx), std::ref(interval), std::ref(wdl), xorshift(seed+i)));
        }
        for(int i = 0; i < numThreads; i++) {
            threads[i].join();
        }
    } 
    if (input == "selfplay") {
        printSearchUpdates = false;
        int moveTime = 300'000;
        TransTable TT(1024);
        History Hist;
        ContHistory ContHist;

        std::string startingFen;

        std::cout << "Enter the starting fen: ";
        std::getline(std::cin, startingFen);

        chess::Board position(startingFen, false);

        while(position.isGameOver() == chess::GameResult::NONE) {
            std::pair<chess::Move, int> result = IterativeDeepening(position, StopType::Time, moveTime * 0.75, moveTime * 1.2, TT, Hist, ContHist);
            std::cout << "bestmove " << result.first << " eval " << result.second << std::endl;
            position.makeMove(result.first);
            std::cout << position << std::endl << std::endl << std::endl;
        }
    }
    if(input == "teams") {
        int seed = rand();
        printSearchUpdates = false;

        std::mutex mtx;
        int numThreads = 8;

        std::vector<std::thread> threads;
        for(int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(generateTeamData, std::ref(mtx), xorshift(seed+i), 750000, 1200000));
        }
        for(int i = 0; i < numThreads; i++) {
            threads[i].join();
        }
    } else {
        chess::Board pos("rmnmbm1kmbmnmrm/pmpmpmpmpmpmpmpm/8/8/8/8/PmPmPmPmPmPmPmPm/RmNmBmQmKmBmNmRm w - - 0 1 -", false);
        nnue::Accumulator acc;
        fromScratch(acc, pos);
        std::cout << acc.eval(true) << std::endl;

        pos = chess::Board("renfbtqikmbgnlru/pzpspcpwpopxpapd/8/8/8/8/PdPfPmPyPwPuPzPc/RiNaBtQgKxBoNlRl w - - 0 1 -", false);
        fromScratch(acc, pos);
        std::cout << acc.eval(true) << std::endl;

        pos = chess::Board("3rirw1ko1/5pz2/bdnfqx1pmQfph1/pe1pg1Px2pa/Pepl5Ps/1BoPc1NzNyKl1/5PmPh1/3Rd4 w - - 0 1 -", false);
        fromScratch(acc, pos);
        std::cout << acc.eval(true) << std::endl;
    }
}