# Rotom
Rotom is a free and strong Pokemon Chess engine


## Strength
As of 3/10/2025, Rotom is the strongest available Pokemon Chess engine. It is far stronger than even the best human players, though it is not perfect. In some specifically designed endgames, it may not find the optimal moves, however this does not impact the final strength significantly.


## Features
Rotom uses a well-optimized search based on Alpha Beta pruning. Enhancements include transposition tables, move histories, and late move reductions. For the full development history of search features and their corresponding strength improvements, see the [Changelog](Changelog.md)

For the evaluation, Rotom uses an extremely fast and lightweight NNUE. This can evaluate several million positions per second per thread on a modern CPU, allowing Rotom to search several moves in the future. To view the history of this evaluation function, check out the [NNUE Changelog](NNUE_Changelog.md).
