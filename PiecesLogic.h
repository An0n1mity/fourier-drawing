#ifndef PIECES_LOGIC_H
#define PIECES_LOGIC_H

#include "GameStructures.h"
#include "BoardLogic.h"


// Moves stack primitives
struct Move_s* CreateMove(int row, int col, int previous_row, int previous_col);
void AddMove(struct Move_s** moves_list, struct Move_s* move_to_add);
void FreeMoves(struct Move_s* moves_list);
void AddMoves(struct Move_s** moves_list, struct Move_s* moves_to_add);

struct Move_s* GetPawnMoves(struct Board_s* chess_board, enum Piece_e pawn, int row, int col);
struct Move_s* GetRookMoves(struct Board_s* chess_board, enum Piece_e rook, int row, int col);
struct Move_s* GetBishopMoves(struct Board_s* chess_board, enum Piece_e bishop, int row, int col);
struct Move_s* GetKnightMoves(struct Board_s* chess_board, enum Piece_e knight, int row, int col);
struct Move_s* GetQueenMoves(struct Board_s* chess_board, enum Piece_e queen, int row, int col);
struct Move_s* GetKingMoves(struct Board_s* chess_board, enum Piece_e queen, int row, int col, bool castling);

struct Move_s* GetPawnCaptureMoves(struct Board_s* chess_board, enum Piece_e pawn, int row, int col);
struct Move_s* GetRookCaptureMoves(struct Board_s* chess_board, enum Piece_e rook, int row, int col);
struct Move_s* GetBishopCaptureMoves(struct Board_s* chess_board, enum Piece_e bishop, int row, int col);
struct Move_s* GetKnightCaptureMoves(struct Board_s* chess_board, enum Piece_e knight, int row, int col);
struct Move_s* GetQueenCaptureMoves(struct Board_s* chess_board, enum Piece_e queen, int row, int col);
struct Move_s* GetKingCaptureMoves(struct Board_s* chess_board, enum Piece_e queen, int row, int col);
struct Move_s* GetCastleMoves(struct Board_s* chess_board, enum Piece_e king, int row, int col);
#endif // PIECES_LOGIC_H
