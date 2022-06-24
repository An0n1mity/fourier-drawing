#include "TT.h"

uint64_t get64rand() {
    return
    (((uint64_t) rand() <<  0) & 0x000000000000FFFFull) |
    (((uint64_t) rand() << 16) & 0x00000000FFFF0000ull) |
    (((uint64_t) rand() << 32) & 0x0000FFFF00000000ull) |
    (((uint64_t) rand() << 48) & 0xFFFF000000000000ull);
}

uint64_t* RandomArray()
{
    uint64_t* random_array = (uint64_t*)malloc(sizeof(uint64_t)*64*12);
    srand(time(NULL));
    for (int i = 0; i < 64*12; i++)
    {
        random_array[i] = get64rand();
    }
    
    return random_array;
}

int GetIndexOfPiece(enum Piece_e piece)
{
    switch (piece)
    {
        case P:
            return 0;
        case Pm:
            return 0;
        case R:
            return 1;
        case N:
            return 2;
        case B:
            return 3;
        case Q:
            return 4;
        case K:
            return 5;
        case p:
            return 6;
        case pm:
            return 7;
        case r:
            return 8;
        case n:
            return 9;
        case b:
            return 10;
        case q:
            return 11;
        case k:
            return 12;
    }
}

uint64_t ZobristHash(struct Board_s* chess_board, uint64_t* random_array)
{
    uint64_t hash = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(!isEmpty(chess_board, i, j))
            {
                hash ^= random_array[(i*8+j) + GetIndexOfPiece(chess_board->squares[i*8+j])];
            }
        }
        
    }
    return hash;
}