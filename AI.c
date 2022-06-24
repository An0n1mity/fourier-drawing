#include "AI.h"
struct Move_s* GetCaptureMoves(struct Board_s* chess_board);
struct Move_s* GetEnemyPawnDangerMap(struct Board_s* chess_board);
bool isSameMove(struct Move_s* A, struct Move_s* B);

const int black_pawn_squares[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, -20, -20, 10, 10,  5,
    5, -5, -10,  0,  0, -10, -5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0, 0, 0, 0, 0, 0, 0, 0
};

const int white_pawn_squares[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5, -10,  0,  0, -10, -5,  5,
    5, 10, 10, -20, -20, 10, 10,  5,
    0, 0, 0, 0, 0, 0, 0, 0
};

const int knight_squares[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};

const int black_bishop_squares[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

const int white_bishop_squares[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,  
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};

const int black_rook_squares[64] = {
    0, 0, 0, 5, 5, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    5, 10, 10, 10, 10, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
};

const int white_rook_squares[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0,
};

const int queen_squares[64] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20
};

const int black_king_squares[64] = {
    20, 30, 10, 0, 0, 10, 30, 20,
    20, 20, 0, 0, 0, 0, 20, 20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};

const int white_king_squares[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, 0, 0, 0, 0, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20,
};

const int black_king_endgame_squares[64] = {
    50, -30, -30, -30, -30, -30, -30, -50,
    -30, -30,  0,  0,  0,  0, -30, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -20, -10,  0,  0, -10, -20, -30,
    -50, -40, -30, -20, -20, -30, -40, -50
};

const int white_king_endgame_squares[64] = {
    -50, -40, -30, -20, -20, -30, -40, -50
    -30, -20, -10,  0,  0, -10, -20, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -30,  0,  0,  0,  0, -30, -30,
    50, -30, -30, -30, -30, -30, -30, -50,
};

__attribute__((always_inline)) inline int GetPieceValue(enum Piece_e piece)
{
    switch (piece)
    {
    case p:
        return 100;
    case P:
        return 100;
    case n:
        return 320;
    case N:
        return 320;
    case b:
        return 330;
    case B:
        return 330;
    case r:
        return 500;
    case R:
        return 500;
    case q:
        return 900;
    case Q:
        return 900;
    case k:
        return 20000;
    case K:
        return 20000;
    
    default:
        return 0;
    }
}

struct Move_s* GetPiecesLegalsMoves(struct Board_s* chess_board, struct Move_s** killer_moves, int depth)
{
    struct Move_s* legals_moves = NULL;
    struct Piece_Position_s* pieces_positions;
    struct Piece_Position_s* pawns_positions;
    struct Piece_Position_s* king_position;

    if(chess_board->white_turn){
        pieces_positions = chess_board->white_pieces_position;
        pawns_positions = chess_board->white_pawns_positions;
        king_position = chess_board->white_king_position;
    }
    else{
        pieces_positions = chess_board->black_pieces_position;
        pawns_positions = chess_board->black_pawns_positions;
        king_position = chess_board->black_king_position;
    }
    
    struct Move_s* moves;
    struct Move_s* cursor;
    struct Move_s* legal_move;
    while(pieces_positions)
    {
                moves = GetPieceMoves(chess_board, chess_board->squares[pieces_positions->row*8 + pieces_positions->col], pieces_positions->row, pieces_positions->col, true);
                cursor = moves;
                chess_board->holded_piece = chess_board->squares[pieces_positions->row*8 + pieces_positions->col];
                while(cursor)
                {
                        legal_move = CreateMove(cursor->row, cursor->col, pieces_positions->row, pieces_positions->col);
                        MoveScore(chess_board, legal_move, killer_moves, depth);
                        AddMoveInOrder(&legals_moves, legal_move);
                 
                    cursor = cursor->next_move;
                }
                FreeMoves(moves);
        
        pieces_positions = pieces_positions->next_position;
    }

    // Get Pawns moves
    while(pawns_positions)
    {
        moves = GetPieceMoves(chess_board, chess_board->squares[pawns_positions->row*8 + pawns_positions->col], pawns_positions->row, pawns_positions->col, false);
        cursor = moves;
        while(cursor)
        {
            legal_move = CreateMove(cursor->row, cursor->col, pawns_positions->row, pawns_positions->col);
            MoveScore(chess_board, legal_move, killer_moves, depth);
            AddMoveInOrder(&legals_moves, legal_move);

            cursor = cursor->next_move;
        }
        FreeMoves(moves);
        pawns_positions = pawns_positions->next_position;
    }

    // Get king moves
    moves = GetPieceMoves(chess_board, chess_board->squares[king_position->row*8 + king_position->col], king_position->row, king_position->col, false);
    cursor = moves;
    while(cursor)
    {
        legal_move = CreateMove(cursor->row, cursor->col, king_position->row, king_position->col);
        MoveScore(chess_board, legal_move, killer_moves, depth);
        AddMoveInOrder(&legals_moves, legal_move);

        cursor = cursor->next_move;
    }
    FreeMoves(moves);

    return legals_moves;
}

__attribute__((always_inline)) inline int GetPositionScoreV2(struct Board_s* chess_board)
{
    int actual_white_score = 0;
    int actual_black_score = 0;
    int white_mobility = 0;
    int black_mobility = 0;
    int row, col;
    struct Piece_Position_s* white_pieces_positions = chess_board->white_pieces_position;
    struct Piece_Position_s* black_pieces_positions = chess_board->black_pieces_position;
    
    while(white_pieces_positions)
    {
        row = white_pieces_positions->row;
        col = white_pieces_positions->col;
        actual_white_score += GetPieceValue(chess_board->squares[row * 8 + col]);
        //white_mobility += EvaluationPiecePosition(chess_board->squares[row * 8 + col], row, col);
        
        white_pieces_positions = white_pieces_positions->next_position;
    }

    while(black_pieces_positions)
    {
        row = black_pieces_positions->row;
        col = black_pieces_positions->col;
        actual_black_score += GetPieceValue(chess_board->squares[row * 8 + col]);
        //black_mobility += EvaluationPiecePosition(chess_board->squares[row * 8 + col], row, col);
        
        black_pieces_positions = black_pieces_positions->next_position;
    }
    
    int evaluation = (actual_white_score + white_mobility) - (actual_black_score + black_mobility);
    int c = (chess_board->white_turn) ? 1: -1;
    
    return evaluation * c;

}

int EvaluateMobility(struct Move_s* move, enum Piece_e piece)
{
    switch (piece)
    {
        case p:
            return black_pawn_squares[move->row*8+move->col] - black_pawn_squares[move->previous_row*8+move->previous_col];
        case P:
            return white_pawn_squares[move->row*8+move->col] - white_pawn_squares[move->previous_row*8+move->previous_col];
        case r:
            return black_rook_squares[move->row*8+move->col] - black_rook_squares[move->previous_row*8+move->previous_col];
        case R:
            return white_rook_squares[move->row*8+move->col] - white_rook_squares[move->previous_row*8+move->previous_col];
        case n:
            return knight_squares[move->row*8+move->col] - knight_squares[move->previous_row*8+move->previous_col];
        case N:
            return knight_squares[move->row*8+move->col] - knight_squares[move->previous_row*8+move->previous_col];
        case b:
            return black_bishop_squares[move->row*8+move->col] - black_bishop_squares[move->previous_row*8+move->previous_col];
        case B:
            return white_bishop_squares[move->row*8+move->col] - white_bishop_squares[move->previous_row*8+move->previous_col];
        case q:
            return queen_squares[move->row*8+move->col] - queen_squares[move->previous_row*8+move->previous_col];
        case Q:
            return queen_squares[move->row*8+move->col] - queen_squares[move->previous_row*8+move->previous_col];
        case k:
            return black_king_squares[move->row*8+move->col] - black_king_squares[move->previous_row*8+move->previous_col];
        case K:
            return white_king_squares[move->row*8+move->col] - white_king_squares[move->previous_row*8+move->previous_col];

        default:
            break;
    }
}

int EvaluationPiecePosition(enum Piece_e piece, int row, int col)
{
    switch (piece)
    {
        case p:
            return black_pawn_squares[row*8+col];
        case P:
            return white_pawn_squares[row*8+col];
        case r:
            return black_rook_squares[row*8+col];
        case R:
            return white_rook_squares[row*8+col];
        case n:
            return knight_squares[row*8+col];
        case N:
            return knight_squares[row*8+col];
        case b:
            return black_bishop_squares[row*8+col];
        case B:
            return white_bishop_squares[row*8+col];
        case q:
            return queen_squares[row*8+col];
        case Q:
            return queen_squares[row*8+col];
        case k:
            return black_king_squares[row*8+col];
        case K:
            return white_king_squares[row*8+col];
    }
}

__attribute__((always_inline)) inline void MoveScore(struct Board_s* chess_board, struct Move_s* restrict move, struct Move_s** restrict killer_moves, int depth)
{
    int move_score = 0;
    // Mobility
    //move_score += EvaluateMobility(move, chess_board->squares[move->row*8 + move->col]);

    // MMVP-LVP
    if(!isEmpty(chess_board, move->row, move->col))
    {
        move_score += 10 * GetPieceValue(chess_board->squares[move->row*8 + move->col]) - GetPieceValue(chess_board->squares[move->previous_row*8 + move->previous_col]);
        //move_score += 10000;
    }    
    else{
         // Killer move
        if(killer_moves && (depth >= 0))
        {
            if(isSameMove(killer_moves[depth*2], move))
                move_score = 9000;
            else if(isSameMove(killer_moves[depth*2+1], move))
                move_score = 8000;
            else
                move_score = EvaluateMobility(move, chess_board->squares[move->previous_row*8 + move->previous_col]); 
        }    
    }
    
    // Check if a pawn can capture
    if(chess_board->white_turn && (!isMoveOutside(move->row-1, move->col-1) && chess_board->squares[(move->row-1)*8 + move->col-1] == p) || (!isMoveOutside(move->row-1, move->col+1) && chess_board->squares[(move->row-1)*8 + move->col+1] == p))
            move_score -= GetPieceValue(chess_board->squares[move->row*8 + move->col]);
    else if(!(chess_board->white_turn && !isMoveOutside(move->row+1, move->col-1) && chess_board->squares[(move->row+1)*8 + move->col-1] == p) || (!isMoveOutside(move->row+1, move->col+1) && chess_board->squares[(move->row+1)*8 + move->col+1] == p))
            move_score -= GetPieceValue(chess_board->squares[move->row*8 + move->col]);

/*
      struct Move_s* pawn_danger_map = GetEnemyPawnDangerMap(chess_board);
    struct Move_s* cursor = pawn_danger_map;
    while(cursor)
    {
        if((cursor->row == move->row) && (cursor->col == move->col) )
            move_score -= GetPieceValue(chess_board->squares[move->row*8 + move->col]);

        cursor = cursor->next_move;
    }
    FreeMoves(pawn_danger_map);*/
    
    // Update move score
    move->score = move_score;
}

void AddMoveInOrder(struct Move_s** restrict moves, struct Move_s* restrict move)
{
    // Calculate move score 
    if(!moves || !moves)
        return;

    else if(!(*moves) || (*moves)->score < move->score)
    {
        move->next_move = *moves;
        *moves = move;
        return;
    }

    struct Move_s* cursor = *moves;
    while (cursor->next_move != NULL && cursor->next_move->score > cursor->score) {
                cursor = cursor->next_move;
    }
    move->next_move = cursor->next_move;
    cursor->next_move = move;

}

int EvaluationV2(struct Board_s* chess_board, struct Move_s* move)
{
    return GetPositionScoreV2(chess_board);
}

struct Move_s** CreateKillerMovesArray(int depth)
{
    struct Move_s** killer_moves_array = (struct Move_s**)calloc(depth*2, sizeof(struct Move_s*));
    return killer_moves_array;
}

__attribute__((always_inline)) inline bool isSameMove(struct Move_s* A, struct Move_s* B)
{
    if(!A || !B)
        return false;
    if((A->row == B->row) && (A->col == B->col))
        return true;
    return false;
}

void AddKillerMove(struct Move_s** restrict killer_moves, struct Move_s* restrict killer_move, int depth)
{
    // Check if the killer move has already been seek
    if((!isSameMove(killer_moves[depth*2], killer_move)) && (killer_moves[depth*2+1] && !isSameMove(killer_moves[depth]->next_move, killer_move)))
    {
        // Free the last move
        FreeMoves(killer_moves[depth*2+1]->next_move);
        // Add new move
        struct Move_s* new_killer_move = CreateMove(killer_move->row, killer_move->col, killer_move->previous_row, killer_move->previous_col);
        new_killer_move->score = killer_move->score;
        killer_moves[depth*2+1] = killer_moves[depth*2];
        killer_moves[depth*2] = killer_move;
    }
}

int QuiescenceSearch(struct Board_s* chess_board, struct Move_s* move, int alpha, int beta, int* nb_nodes)
{ 
    int evaluation = GetPositionScoreV2(chess_board);
    if(evaluation >= beta)
        return beta;
    alpha = max(alpha, evaluation);

    struct Move_s* capture_moves = GetCaptureMoves(chess_board);
    if(!capture_moves)
    {
        // Check for legal moves
        struct Move_s* legal_moves = GetPiecesLegalsMoves(chess_board, NULL, -1);
        if(!legal_moves)
            return INT_MIN;
        free(capture_moves);
    }
        //return INT_MIN;
    struct Move_s* cursor = capture_moves;
    enum Piece_e captured;
    while(cursor)
    {
        captured = MakeMove(cursor, chess_board, cursor->previous_row, cursor->previous_col);
        chess_board->white_turn = !chess_board->white_turn;
        (*nb_nodes)++;
        evaluation = -1 * QuiescenceSearch(chess_board, cursor, -1 * beta, -1 * alpha, nb_nodes);
        chess_board->white_turn = !chess_board->white_turn;
        UndoMove(captured, cursor, chess_board, cursor->previous_row, cursor->previous_col);

        if(evaluation >= beta)
        {
            FreeMoves(capture_moves);
            return beta;
        }
        alpha = max(alpha, evaluation);
        cursor = cursor->next_move;
    }
    FreeMoves(capture_moves);
    return alpha;
}

int MinMax(struct Board_s* chess_board, struct Move_s** killer_moves, int depth, int alpha, int beta, int* nb_nodes)
{

    if(depth <= 0)
        return QuiescenceSearch(chess_board, NULL, alpha, beta, nb_nodes);

    // Get all the legals moves
    struct Move_s* legals_moves = GetPiecesLegalsMoves(chess_board, killer_moves[depth], depth);
    if(!legals_moves)
    {
       return INT_MIN;
    }
    // Try each of these moves
    struct Move_s* cursor = legals_moves;
    int best_score = INT_MIN;

    struct Move_s best_move;
    int evaluation = INT_MIN;

    enum Piece_e holded;
    enum Piece_e captured;
    while(cursor)
    {
        // Make the move
        holded = chess_board->squares[cursor->previous_row*8 + cursor->previous_col];
        captured = MakeMove(cursor, chess_board, cursor->previous_row, cursor->previous_col);
        // Go deeper
        (*nb_nodes)++;
        chess_board->white_turn = !chess_board->white_turn;
        evaluation = max(evaluation, -1 * MinMax(chess_board, killer_moves, depth - 1, -1 * beta, -1 * alpha, nb_nodes));
        chess_board->white_turn = !chess_board->white_turn;
        // Undo the move
        UndoMove(captured, cursor, chess_board, cursor->previous_row, cursor->previous_col);       
        // Get the best move possible for maximizing player
        if(evaluation > best_score)
        {
            best_move.row = cursor->row;
            best_move.col = cursor->col;
            best_move.previous_row = cursor->previous_row;
            best_move.previous_col = cursor->previous_col;
            best_score = evaluation;
        }
        alpha = max(alpha, evaluation);

        if(alpha >= beta)
        {
            // Non capture move provoking beta cutoff = Killer move
            if(isEmpty(chess_board, cursor->row, cursor->col)){
                AddKillerMove(killer_moves, cursor, depth);
            }
            FreeMoves(legals_moves);
            return evaluation;
        }
        cursor = cursor->next_move;
    }

    if(depth == 4)
    {
        MakeMove(&best_move, chess_board, best_move.previous_row, best_move.previous_col);
    }
    FreeMoves(legals_moves);

    return evaluation;

}

struct Move_s* GetPieceLegalsMoves(struct Board_s* chess_board, enum Piece_e piece, int row, int col)
{
    struct Move_s* legals_moves = NULL;
    // Get all moves for that piece
    struct Move_s* pieces_moves = GetPieceMoves(chess_board, chess_board->squares[row*8 + col], row, col, true);
    struct Move_s* cursor = pieces_moves;
    while(cursor)
    {
        // Check for legals moves
        if(isMoveLegal(chess_board, pieces_moves, cursor))
            AddMove(&legals_moves, CreateMove(cursor->row, cursor->col, row, col));
        cursor = cursor->next_move;
    }
    FreeMoves(pieces_moves);

    return legals_moves;
    
}

// Get all moves resulting in a capture for the current player piece
struct Move_s* GetCaptureMoves(struct Board_s* chess_board)
{
    struct Move_s* capture_moves = NULL;
    struct Piece_Position_s* pieces_positions;
    struct Piece_Position_s* king_position;
    struct Piece_Position_s* pawns_positions;

    if(chess_board->white_turn){
        pieces_positions = chess_board->white_pieces_position;
        pawns_positions = chess_board->white_pawns_positions;
        king_position = chess_board->white_king_position;
    }
    else{
        pieces_positions = chess_board->black_pieces_position;
        pawns_positions = chess_board->black_king_position;
        king_position = chess_board->black_king_position;
    }
    // Get the pieces moves
    struct Move_s* piece_moves;
    struct Move_s* move;
    struct Move_s* cursor;
    while(pieces_positions)
    {
        int row = pieces_positions->row;
        int col = pieces_positions->col;
                piece_moves = GetPieceCaptureMoves(chess_board, chess_board->squares[row*8+col], row, col);
                cursor = piece_moves;
                while(cursor)
                {
               
                        move = CreateMove(cursor->row, cursor->col, cursor->previous_row, cursor->previous_col);
                        MoveScore(chess_board, move, NULL, -1);
                        AddMoveInOrder(&capture_moves, move);
                    
                    cursor = cursor->next_move;
                }
                FreeMoves(piece_moves);     
        pieces_positions = pieces_positions->next_position;
        
    }
    
    // Get Pawns moves
    while(pawns_positions)
    {
        piece_moves = GetPieceMoves(chess_board, chess_board->squares[pawns_positions->row*8 + pawns_positions->col], pawns_positions->row, pawns_positions->col, false);
        cursor = piece_moves;
        while(cursor)
        {
            move = CreateMove(cursor->row, cursor->col, pawns_positions->row, pawns_positions->col);
            MoveScore(chess_board, move, NULL, -1);
            AddMoveInOrder(&capture_moves, move);

            cursor = cursor->next_move;
        }
        FreeMoves(piece_moves);
        pawns_positions = pawns_positions->next_position;
    }


    // Get king's moves
    piece_moves = GetPieceMoves(chess_board, chess_board->squares[king_position->row*8 + king_position->col], king_position->row, king_position->col, false);
    cursor = piece_moves;
    while(cursor)
    {
        move = CreateMove(cursor->row, cursor->col, king_position->row, king_position->col);
        MoveScore(chess_board, move, NULL, -1);
        AddMoveInOrder(&capture_moves, move);

        cursor = cursor->next_move;
    }
    FreeMoves(piece_moves);
       
    return capture_moves;
}

__attribute__((always_inline)) inline bool isEnemyPawn(enum Piece_e pawn, bool white_turn)
{
    if(white_turn && ((pawn == p) || (pawn == pm)))
        return true;
    else if(!white_turn && ((pawn == P) || (pawn == Pm)))
        return true;
    return false;
}

struct Move_s* GetEnemyPawnDangerMap(struct Board_s* chess_board)
{
    struct Move_s* pawns_moves = NULL;
    struct Move_s* pawn_moves = NULL;
    int pawn_row;
    int pawn_col;
    struct Piece_Position_s* pawns_positions;

    if(!chess_board->white_turn)
        pawns_positions = chess_board->white_pawns_positions;
    else
        pawns_positions = chess_board->black_pawns_positions;
    
    struct Move_s* cursor;
    enum Piece_e pawn;
    while(pawns_positions)   
    {  
            pawn_row = pawns_positions->row;
            pawn_col = pawns_positions->col;
            pawn = chess_board->squares[pawn_row * 8 + pawn_col];
            pawn_moves = GetPawnMoves(chess_board, pawn, pawn_row, pawn_col);
            // Check if move is Legal for the opponent
            cursor = pawn_moves;
            while(cursor)
            {
                AddMove(&pawns_moves, CreateMove(cursor->row, cursor->col, pawn_row, pawn_col));
                cursor = cursor->next_move;
            }
            FreeMoves(pawn_moves);
            pawns_positions = pawns_positions->next_position;
    }
    return pawns_moves;
}

