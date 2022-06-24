#ifndef TT_H
#define TT_H

#include "GameStructures.h"
#include <inttypes.h>

enum TT_Flag_e{
    EXACT,
    LOWERBOUND,
    UPPERBOUND
};

struct TTEntry_s{
    int depth;
    int evaluation;
    enum TT_Flag_e flag;
    struct Move_s* best_move;
};


uint64_t* RandomArray();
uint64_t ZobristHash(struct Board_s* chess_board, uint64_t* random_array);

#endif // !TT_H