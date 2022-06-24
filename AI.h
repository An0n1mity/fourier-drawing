#ifndef AI_H
#define AI_H

#include "GameStructures.h"
#include "BoardLogic.h"
#include <gtk/gtk.h>
#include <math.h>
#include <inttypes.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

int MinMax(struct Board_s* chess_board, struct Move_s** killer_moves, int depth, int alpha, int beta, int* nb_nodes);
struct Move_s** CreateKillerMovesArray(int depth);


#endif // !AI_H
