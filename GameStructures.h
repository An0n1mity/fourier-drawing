#ifndef GAME_STRUCTURES_H
#define GAME_STRUCTURES_H

#include <stdbool.h>

struct Piece_Position_s{
    int row;
    int col;
    bool promoted;
    bool checking;
    struct Piece_Position_s* next_position;
};

struct Move_s{
    int row;
    int col;
    int previous_row;
    int previous_col;
    int score;
    struct Move_s* next_move;
};

enum Piece_e {
    E,
    r, n, b, q, k, p, pm,
    R, N, B, Q, K, P, Pm
};

enum Flag_e {
    WHITE_KING_CASTLE = 1, WHITE_QUEEN_CASTLE = 2,
    BLACK_KING_CASTLE = 4, BLACK_QUEEN_CASTLE = 8
};

enum Protected_e
{
    WHITE_PROTECTED = 1, BLACK_PROTECTED = 2
};

struct Board_s {
    // Array of chess pieces
    enum Piece_e* squares;
    // Array of protected squares
    enum Protected_e* protected_squares;
    // Castle flags
    int castle_flag;
    // Array containing pieces positions
    struct Piece_Position_s* white_pieces_position;
    struct Piece_Position_s* black_pieces_position;
    // Kings positions
    struct Piece_Position_s* white_king_position;
    struct Piece_Position_s* black_king_position;
    // Pawns positions
    struct Piece_Position_s* white_pawns_positions;
    struct Piece_Position_s* black_pawns_positions;
    // Rooks positions 
    struct Piece_Position_s* white_rooks_positions;
    struct Piece_Position_s* black_rooks_positions;
    // Player turn either black or white
    bool white_turn; 
    enum Piece_e holded_piece;
    // Player interactions with the board
    int player_row;
    int player_col;
};

#endif