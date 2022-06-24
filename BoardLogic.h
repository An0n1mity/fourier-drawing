#ifndef BOARD_LOGIC_H
#define BOARD_LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "GameStructures.h"
#include "PiecesLogic.h"

// Return a board created based by FEN notation
struct Board_s* CreateBoardFEN(const char* fen_notation);

// Return if the pieces clicked by the player is owned by him
bool isPlayerPiece(struct Board_s* chess_board, int player_row, int player_col);
bool isEnemy(struct Board_s* chess_board, int row, int col);
// Get list of piece moves
struct Move_s* GetPieceMoves(struct Board_s* chess_board, enum Piece_e piece, int row, int col, bool castling);
struct Move_s* GetPieceCaptureMoves(struct Board_s* chess_board, enum Piece_e piece, int row, int col);
bool isMoveLegal(struct Board_s* chess_board, struct Move_s* moves_list, struct Move_s* move);
bool isMoveOutside(int row, int col);
bool isFriend(struct Board_s* chess_board, int row, int col);
struct Move_s* GetEnemyMoves(struct Board_s* chess_board);
bool isKingChecked(struct Board_s* chess_board);
bool isKingCheckedMated(struct Board_s* chess_board, struct Move_s* enememy_moves);
bool isEmpty(struct Board_s* chess_board, int row, int col);
enum Piece_e MakeMove(struct Move_s* move, struct Board_s* chess_board, int previous_row, int previous_col);
void UndoMove(enum Piece_e captured, struct Move_s* move, struct Board_s* chess_board, int previous_row, int previous_col);
bool isCasltingLegal(struct Board_s* chess_board, enum Flag_e castle_flag);
bool isMoveIllegal(struct Board_s* chess_board, struct Move_s* move);
#endif