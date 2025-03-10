# Rotom
Rotom is a free and strong Pokemon Chess engine


## Strength
As of 3/10/2025, Rotom is the strongest available Pokemon Chess engine. It is far stronger than even the best human players, though it is not perfect. In some specifically designed endgames, it may not find the optimal moves, however this does not impact the final strength significantly.


## Features
Rotom uses a well-optimized search based on Alpha Beta pruning. Enhancements include transposition tables, move histories, and late move reductions. For the full development history of search features and their corresponding strength improvements, see the [Changelog](Changelog.md)

For the evaluation, Rotom uses an extremely fast and lightweight NNUE. This can evaluate several million positions per second per thread on a modern CPU, allowing Rotom to search several moves in the future. To view the history of this evaluation function, check out the [NNUE Changelog](NNUE_Changelog.md).


## Support
Most of the public discussion about Rotom is on the official Pokemon Chess [Discord](https://discord.com/invite/fp5bcCqg8q). If you have questions or would like support, reach out to me on Github or join the discord for a quicker response.


## Limitations
Rotom is capable of analyzing any position, including random or specifically designed teams, at a very high level. That being said, Rotom does not currently support critical hits and misses, and there is no plan to do so in the future. As such, when playing with random effects enabled, the analysis may not be optimal. There may be a seperate engine designed specifically for these games in the future, however there are no immediate plans for one.
