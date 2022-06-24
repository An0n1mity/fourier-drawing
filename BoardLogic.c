#include "BoardLogic.h"
struct Piece_Position_s* InitPiecesPositions(struct Board_s* chess_board, bool white);
#define DEBU

void AddMove(struct Move_s** moves_list, struct Move_s* move_to_add);

struct Board_s* CreateBoardFEN(const char* fen_notation_c)
{
    struct Board_s* chess_board = (struct Board_s*)calloc(1, sizeof(struct Board_s));
    chess_board->squares = (enum Piece_e*)malloc(sizeof(enum Piece_e) * 64);
    bool board = true;

    enum Piece_e piece;
    int row = 0, col = 0;

    char* fen_notation = _strdup(fen_notation_c);
    fen_notation = strtok(fen_notation, " ");
    int idx = 0;
    while(fen_notation)
    {
        char* string = strdup(fen_notation);
        printf("%s\n", string);
        // Board representation
        if(idx == 0)
        {
            for (int i = 0; string[i] != 0; i++)
            {
                // Cheking for chess board pieces
                if(board){
                    switch (string[i])
                    {
                    case 'r':
                        piece = r;
                        break;
                    case 'n':
                        piece = n;
                        break;
                    case 'b':
                        piece = b;
                        break;
                    case 'q':
                        piece = q;
                        break;
                    case 'k':
                        piece = k;
                        break;
                    case 'p':
                        piece = p;
                        break;
                    case 'R':
                        piece = R;
                        break;
                    case 'N':
                        piece = N;
                        break;
                    case 'B':
                        piece = B;
                        break;
                    case 'Q':
                        piece = Q;
                        break;
                    case 'K':
                        piece = K;
                        break;
                    case 'P':
                        piece = P;
                        break;
                    case '/':
                        col = 0;
                        row++;
                        continue;
                    case ' ':
                        board = false;
                        continue;
                    default:
                        int nb = string[i] - '\0';
                        for (int j = col; j < nb-1; j++)
                        {
                            if(row * 8 + col + j > 64)
                                continue;
                            chess_board->squares[row * 8 + col + j] = E;
                        }
                        continue;   
                    }
                    
                    chess_board->squares[row * 8 + col] = piece;
                    col++;
                    }
            }
        }

        else if(idx == 1)
        {
            switch (string[0])
            {
            case 'w':
                chess_board->white_turn = true;
                break;
            case 'b':
                chess_board->white_turn = false;
                break;
            default:
                break;
            }
        }

        else if(idx == 2)
        {
            for (int i = 0; string[i] != '\0'; i++)
            {
                switch (string[i])
                {
                    case 'K':
                        chess_board->castle_flag |= WHITE_KING_CASTLE;
                        break;
                    case 'Q':
                        chess_board->castle_flag |= WHITE_QUEEN_CASTLE;
                        break;
                     case 'k':
                        chess_board->castle_flag |= BLACK_KING_CASTLE;
                        break;
                    case 'q':
                        chess_board->castle_flag |= BLACK_QUEEN_CASTLE;
                        break;
                default:
                    break;
                }
            }
            
        }

        idx++;
        fen_notation = strtok(NULL, " ");
    }

    
    #ifdef DEBUG
        fprintf(stdout, "[INFO] Board was created\n");
        if(chess_board->white_turn)
            fprintf(stdout, "[INFO] White start the game\n");
        else
            fprintf(stdout, "[INFO] Black start the game\n");

    #endif // DEBUG

    // Get Pieces positions 
    chess_board->white_pieces_position = InitPiecesPositions(chess_board, true);
    chess_board->black_pieces_position = InitPiecesPositions(chess_board, false);

    // Get pieces protected squares
    InitProtectedSquares(chess_board);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%d ", chess_board->protected_squares[i*8+j]);
        }
        printf("\n");
        
    }
    

    return chess_board;
}

struct Piece_Position_s* CreatePiecePosition(int row, int col)
{
    struct Piece_Position_s* piece_position = (struct Piece_Position_s*)calloc(1, sizeof(struct Piece_Position_s));
    piece_position->row = row;
    piece_position->col = col;

    return piece_position;
}

void AddPiecePosition(struct Piece_Position_s** restrict positions, struct Piece_Position_s* restrict position)
{
    if(!*positions && position)
    {
        *positions = position;
        return;
    }

    position->next_position = *positions;
    *positions = position;
}

void DeletePiecePosition(struct Piece_Position_s** positions, int row, int col)
{
    struct Piece_Position_s* temp = *positions;
    struct Piece_Position_s* prev = NULL;
     
    if (temp != NULL && (temp->row == row) && (temp->col == col))
    {
        *positions = temp->next_position; // Changed head
        free(temp);            // free old head
        return;
    }
 
    else
    {
    while (temp != NULL && ((temp->row != row) || (temp->col != col)))
    {
        prev = temp;
        temp = temp->next_position;
    }
 
    if (temp == NULL)
        return;
 
    // Unlink the node from linked list
    prev->next_position = temp->next_position;
 
    // Free memory
    free(temp);
    }
}

void UpdatePiecePosition(struct Piece_Position_s** pieces_position, struct Move_s* move, bool promoted)
{
    struct Piece_Position_s* cursor = *pieces_position;
    while(cursor)
    {
        if((cursor->row == move->previous_row) && (cursor->col == move->previous_col))
        {
            // Update the piece position
            cursor->row = move->row;
            cursor->col = move->col;
            cursor->promoted = promoted; 
            return;
        }
        cursor = cursor->next_position;
    }
}

void UpdatePieceProtectedSquares(struct Board_s* chess_board, struct Move_s* move, enum Piece_e piece)
{
    if(piece == P)
    {
        // New protection after displacement
        GetPawnProtectedSquares(chess_board, piece, move->row, move->col);
    }
}

struct Piece_Position_s* InitPiecesPositions(struct Board_s* chess_board, bool white)
{
    struct Piece_Position_s* pieces_positions = NULL;
    bool tmp = chess_board->white_turn;
    chess_board->white_turn = white;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(isFriend(chess_board, i, j))
            {
                switch (chess_board->squares[i*8+j])
                {
                case K:
                    chess_board->white_king_position = CreatePiecePosition(i, j);
                    break;
                case k:
                    chess_board->black_king_position = CreatePiecePosition(i, j);
                    break;
                case P:
                    AddPiecePosition(&chess_board->white_pawns_positions, CreatePiecePosition(i, j));
                    break;
                case p:
                    AddPiecePosition(&chess_board->black_pawns_positions, CreatePiecePosition(i, j));
                    break;
                default:
                    AddPiecePosition(&pieces_positions, CreatePiecePosition(i, j));
                    break;
                }
            } 
        }
        
    }
    chess_board->white_turn = tmp;
    return pieces_positions;
}

void InitProtectedSquares(struct Board_s* chess_board)
{
    chess_board->protected_squares = (enum Protected_e*)calloc(64, sizeof(enum Protected_e));
    struct Piece_Position_s* pieces_position;
    // Get white protected squares
    pieces_position = chess_board->white_pieces_position;
    while(pieces_position)
    {
        GetPieceProtectedSquares(chess_board, chess_board->squares[pieces_position->row*8 + pieces_position->col], pieces_position->row, pieces_position->col);
        pieces_position = pieces_position->next_position;
    }

    // Get black protected squares
    pieces_position = chess_board->black_pieces_position;
    while(pieces_position)
    {
        GetPieceProtectedSquares(chess_board, chess_board->squares[pieces_position->row*8 + pieces_position->col], pieces_position->row, pieces_position->col);
        pieces_position = pieces_position->next_position;
    }
}

bool isPlayerPiece(struct Board_s* chess_board, int player_row, int player_col)
{
    if((chess_board->white_turn && (
        (chess_board->squares[player_row*8 + player_col] == R )||
        (chess_board->squares[player_row*8 + player_col] == N )||
        (chess_board->squares[player_row*8 + player_col] == B )||
        (chess_board->squares[player_row*8 + player_col] == Q )||
        (chess_board->squares[player_row*8 + player_col] == K )||
        (chess_board->squares[player_row*8 + player_col] == P )||
        (chess_board->squares[player_row*8 + player_col] == Pm )))
        || 
        (!chess_board->white_turn && (
        (chess_board->squares[player_row*8 + player_col] == r )||
        (chess_board->squares[player_row*8 + player_col] == n )||
        (chess_board->squares[player_row*8 + player_col] == b )||
        (chess_board->squares[player_row*8 + player_col] == q )||
        (chess_board->squares[player_row*8 + player_col] == k )||
        (chess_board->squares[player_row*8 + player_col] == p) ||
        (chess_board->squares[player_row*8 + player_col] == pm )))
    )
    {
        #ifdef DEBUG
            fprintf(stdout, "[INFO] Player picked his piece\n");
        #endif // DEBUG7
         return true;
    }   
    else 
    {
        #ifdef DEBUG
            fprintf(stdout, "[INFO] Player do not picked piece\n");        
        #endif // DEBUG
        return false;
    }
}

// Check if the square is occupied by the opponent pieces
bool isEnemy(struct Board_s* chess_board, int row, int col)
{
    // If white turn check for black opponent pieces
    if(chess_board->white_turn && 
        (
        (chess_board->squares[row*8 + col] == k ) ||
        (chess_board->squares[row*8 + col] == q ) ||
        (chess_board->squares[row*8 + col] == b ) ||
        (chess_board->squares[row*8 + col] == n ) ||
        (chess_board->squares[row*8 + col] == r ) ||
        (chess_board->squares[row*8 + col] == p ) 
        )
    )
    {
        return true;
    }
    else if(!chess_board->white_turn && 
        (
        (chess_board->squares[row*8 + col] == K ) ||
        (chess_board->squares[row*8 + col] == Q ) ||
        (chess_board->squares[row*8 + col] == B ) ||
        (chess_board->squares[row*8 + col] == N ) ||
        (chess_board->squares[row*8 + col] == R ) ||
        (chess_board->squares[row*8 + col] == P ) 
        )
    )
    {
        return true;
    }
    return false;
}

__attribute__((always_inline)) inline bool isEmpty(struct Board_s* chess_board, int row, int col)
{
    if(chess_board->squares[row*8+col] == E)
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isFriend(struct Board_s* chess_board, int row, int col)
{
    if(!isEnemy(chess_board, row, col) && !isEmpty(chess_board, row, col))
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isMoveOutside(int row, int col)
{
    if((row < 0) || (row > 7) || (col < 0) || (col > 7))
        return true;
    return false; 
}

// Check if a player piece is in capture
bool isInCapture(struct Move_s* enemy_moves, int row, int col)
{
    struct Move_s* cursor = enemy_moves;
    while(cursor)
    {
        if((cursor->row == row) && (cursor->col == col))
            return true;
        cursor = cursor->next_move;
    }

    return false;
}

bool isCasltingLegal(struct Board_s* chess_board, enum Flag_e castle_flag)
{
    // Get the enemy moves 
    struct Move_s* enemy_moves = GetEnemyMoves(chess_board);

        switch (castle_flag)
        {
            case WHITE_KING_CASTLE:
                if(((chess_board->squares[7*8+4] != K) && (chess_board->holded_piece != K)) || (chess_board->squares[7*8+7] != R))
                {
                    return false;
                }
                // Check if no piece in between king and rook
                if(isInCapture(enemy_moves, 7, 4))
                    return false;
                for(int row = 7, col = 5; col <= 6; col++){
                    if(!isEmpty(chess_board, row, col))
                        return false;
                    else{
                        // If no pieces between check if moving the king is legal
                        if(isInCapture(enemy_moves, row, col))
                            return false;
                    }
                }
                    FreeMoves(enemy_moves);

                return true;
            
            case WHITE_QUEEN_CASTLE:
                if(((chess_board->squares[7*8+4] != K) && (chess_board->holded_piece != K)) || (chess_board->squares[7*8+0] != R))
                {
                    return false;
                }
                // Check if no piece in between king and rook
                if(isInCapture(enemy_moves, 7, 4))
                    return false;
                for(int row = 7, col = 3; col >= 1; col--){
                    if(!isEmpty(chess_board, row, col))
                        return false;
                    else{
                        // If no pieces between check if moving the king is legal
                        if(isInCapture(enemy_moves, row, col))
                            return false;
                    }
                }
                    FreeMoves(enemy_moves);

                return true;

            case BLACK_KING_CASTLE:
                if((chess_board->squares[0*8+4] != k) || (chess_board->squares[0*8+7] != r))
                {
                    return false;
                }
                // Check if no piece in between king and rook
                if(isInCapture(enemy_moves, 0, 4))
                    return false;
                for(int row = 0, col = 5; col <= 6; col++){
                    if(!isEmpty(chess_board, row, col))
                        return false;
                    else{
                        // If no pieces between check if moving the king is legal
                        if(isInCapture(enemy_moves, row, col))
                            return false;
                    }
                }
                    FreeMoves(enemy_moves);

                return true;
            
            case BLACK_QUEEN_CASTLE:
                if((chess_board->squares[0*8+4] != k) || (chess_board->squares[0*8+0] != r))
                {
                    return false;
                }
                if(isInCapture(enemy_moves, 0, 4))
                    return false;
                // Check if no piece in between king and rook
                for(int row = 0, col = 3; col >= 1; col--){
                    if(!isEmpty(chess_board, row, col))
                        return false;
                    else{
                        // If no pieces between check if moving the king is legal
                        if(isInCapture(enemy_moves, row, col))
                            return false;
                    }
                }
                    FreeMoves(enemy_moves);
                return true;

            default:
                FreeMoves(enemy_moves);
                return false;
        }

    FreeMoves(enemy_moves);
}

struct Move_s* GetPieceMoves(struct Board_s* restrict chess_board, enum Piece_e piece, int row, int col, bool castling)
{
    switch (piece)
    {
        case P:
            return GetPawnMoves(chess_board, piece, row, col);
        case Pm:
            return GetPawnMoves(chess_board, piece, row, col);
        case p:
            return GetPawnMoves(chess_board, piece, row, col);
        case pm:
            return GetPawnMoves(chess_board, piece, row, col);
        case R:
            return GetRookMoves(chess_board, piece, row, col);
        case r:
            return GetRookMoves(chess_board, piece, row, col);
        case B:
            return GetBishopMoves(chess_board, piece, row, col);
        case b:
            return GetBishopMoves(chess_board, piece, row, col);
        case N:
            return GetKnightMoves(chess_board, piece, row, col);
        case n:
            return GetKnightMoves(chess_board, piece, row, col);
        case Q:
            return GetQueenMoves(chess_board, piece, row, col);
        case q:
            return GetQueenMoves(chess_board, piece, row, col);
        case K:
            return GetKingMoves(chess_board, piece, row, col, castling);
        case k:
            return GetKingMoves(chess_board, piece, row, col, castling);

        default:
            return NULL;
    }
}

struct Move_s* GetPieceCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e piece, int row, int col)
{
    switch (piece)
    {
    case P:
        return GetPawnCaptureMoves(chess_board, piece, row, col);
    case Pm:
        return GetPawnCaptureMoves(chess_board, piece, row, col);
    case p:
        return GetPawnCaptureMoves(chess_board, piece, row, col);
    case pm:
        return GetPawnCaptureMoves(chess_board, piece, row, col);
    case R:
        return GetRookCaptureMoves(chess_board, piece, row, col);
    case r:
        return GetRookCaptureMoves(chess_board, piece, row, col);
    case B:
        return GetBishopCaptureMoves(chess_board, piece, row, col);
    case b:
        return GetBishopCaptureMoves(chess_board, piece, row, col);
    case N:
        return GetKnightCaptureMoves(chess_board, piece, row, col);
    case n:
        return GetKnightCaptureMoves(chess_board, piece, row, col);
    case Q:
        return GetQueenCaptureMoves(chess_board, piece, row, col);
    case q:
        return GetQueenCaptureMoves(chess_board, piece, row, col);
    case K:
        return GetKingCaptureMoves(chess_board, piece, row, col);
    case k:
        return GetKingCaptureMoves(chess_board, piece, row, col);

    default:
        return NULL;
    }
}

void GetPieceProtectedSquares(struct Board_s* chess_board, enum Piece_e piece, int row, int col)
{
    switch (piece)
    {
    
    case P:
        return GetPawnProtectedSquares(chess_board, piece, row, col);
    case p:
        return GetPawnProtectedSquares(chess_board, piece, row, col);
    case R:
        return GetRookProtectedSquares(chess_board, piece, row, col);
    case r:
        return GetRookProtectedSquares(chess_board, piece, row, col);
    case B:
        return GetBishopProtectedSquares(chess_board, piece, row, col);
    case b:
        return GetBishopProtectedSquares(chess_board, piece, row, col);
    case N:
        return GetKnightProtectedSquares(chess_board, piece, row, col);
    case n:
        return GetKnightProtectedSquares(chess_board, piece, row, col);
    case Q:
        return GetQueenProtectedSquares(chess_board, piece, row, col);
    case q:
        return GetQueenProtectedSquares(chess_board, piece, row, col);
    case K:
        return GetKingProtectedSquares(chess_board, piece, row, col);
    case k:
        return GetKingProtectedSquares(chess_board, piece, row, col);
    
    default:
        return NULL;
    }
}

// Get current player king's position in the board
void GetPlayerKingPosition(struct Board_s* chess_board, int* row, int* col)
{
    for (int i = 0; i < 8; i++)
    {
       for (int j = 0; j < 8; j++)
       {
            if(chess_board->white_turn && (chess_board->squares[i*8+j] == K))
            {
                *row = i;
                *col = j;
                return;
            }
            else if(!chess_board->white_turn && (chess_board->squares[i*8+j] == k))
            {
                *row = i;
                *col = j;
                return;
            }
       }
       
    }
    
}

__attribute__((always_inline)) inline bool isRook(struct Board_s* restrict chess_board, int row, int col)
{
    if((chess_board->squares[row*8+col] == r) || (chess_board->squares[row*8+col] == R))
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isQueen(struct Board_s* restrict chess_board, int row, int col)
{
    if((chess_board->squares[row*8+col] == q) || (chess_board->squares[row*8+col] == Q))
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isBishop(struct Board_s* chess_board, int row, int col)
{
    if((chess_board->squares[row*8+col] == b) || (chess_board->squares[row*8+col] == B))
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isKnight(struct Board_s* restrict chess_board, int row, int col)
{
    if((chess_board->squares[row*8+col] == n) || (chess_board->squares[row*8+col] == N))
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isBlackPawn(struct Board_s* restrict chess_board, int row, int col)
{
    if(chess_board->squares[row*8+col] == p)
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isWhitePawn(struct Board_s* restrict chess_board, int row, int col)
{
    if(chess_board->squares[row*8+col] == P)
        return true;
    return false;
}

__attribute__((always_inline)) inline bool isKing(struct Board_s* restrict chess_board, int row, int col)
{
    if((chess_board->squares[row*8+col] == K) || (chess_board->squares[row*8+col] == k))
        return true;
    return false;
}

// TODO : Making a faster move generator !!!

__attribute__((always_inline)) inline bool isMoveIllegal(struct Board_s* chess_board, struct Move_s* move)
{
    bool is_illegal = false;
    // Cant capture the king
    if(isKing(chess_board, move->row, move->col))
    {
        // Remember that the piece is checking the opponent king
        struct Piece_Position_s* pieces_position;
        if(chess_board->white_turn)
            pieces_position = chess_board->white_pieces_position;
        else
            pieces_position = chess_board->black_pieces_position;

        while(pieces_position)
        {
            if((pieces_position->row == move->previous_col) && (pieces_position->col == move->previous_col))
            {
                pieces_position->checking = true;
            }

            pieces_position = pieces_position->next_position;
        }
        return true;
    }
    // Virtually make the move
    //chess_board->holded_piece = chess_board->squares[move->previous_row*8+move->previous_col];
    enum Piece_e captured = MakeMove(move, chess_board, move->previous_row, move->previous_col);
    if(isKingChecked(chess_board))
        is_illegal = true;
    UndoMove(captured, move, chess_board, move->previous_row, move->previous_col);
    return is_illegal;
}

// Check if the king is in the enemy attacking spot
bool isKingChecked(struct Board_s* chess_board)
{
    // Retrieve the king's position
    int king_row, king_col;
    if(chess_board->white_turn){
        king_row = chess_board->white_king_position->row;
        king_col = chess_board->white_king_position->col;
    }
    else{
        king_row = chess_board->black_king_position->row;
        king_col = chess_board->black_king_position->col;
    }
    /*while(pieces_position)
    {
        if((chess_board->squares[pieces_position->row*8+pieces_position->col] == K)
        || (chess_board->squares[pieces_position->row*8+pieces_position->col] == k)
        )
        {
            king_row = pieces_position->row;
            king_col = pieces_position->col;
            break;
        }
        pieces_position = pieces_position->next_position;
        
    }*/

    // Check king up row
    if(isKing(chess_board, king_row-1, king_col))
        return true;
    for (int i = king_row-1; i >= 0; i--)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, king_col))
            break;
        // If an enemy pieces specialy queen or rook then king is checked
        if(isEnemy(chess_board, i, king_col) && (isRook(chess_board, i, king_col) || isQueen(chess_board, i, king_col)))
            return true;
        else if(isEnemy(chess_board, i, king_col) && !(isRook(chess_board, i, king_col) || isQueen(chess_board, i, king_col)))
            break;
    }
    
    // Check king down row
    if(isKing(chess_board, king_row+1, king_col))
        return true;
    for (int i = king_row+1; i <= 7; i++)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, king_col))
            break;
        // If an enemy pieces specialy queen or rook then king is checked
        if(isEnemy(chess_board, i, king_col) && (isRook(chess_board, i, king_col) || isQueen(chess_board, i, king_col)))
            return true;
        else if(isEnemy(chess_board, i, king_col) && !(isRook(chess_board, i, king_col) || isQueen(chess_board, i, king_col)))
            break;
    }

    // Check king right col
    if(isKing(chess_board, king_row, king_col+1))
        return true;
      for (int i = king_col+1; i <= 7; i++)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, king_row, i))
            break;
        // If an enemy pieces specialy queen or rook then king is checked
        if(isEnemy(chess_board, king_row, i) && (isRook(chess_board, king_row, i) || isQueen(chess_board, king_row, i)))  
            return true;
        else if(isEnemy(chess_board, king_row, i) && !(isRook(chess_board, king_row, i) || isQueen(chess_board, king_row, i)))  
            break;
    }

    // Check king left col
    if(isKing(chess_board, king_row, king_col-1))
        return true;
    for (int i = king_col-1; i >= 0; i--)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, king_row, i))
            break;
        // If an enemy pieces specialy queen or rook then king is checked
        if(isEnemy(chess_board, king_row, i) && (isRook(chess_board, king_row, i) || isQueen(chess_board, king_row, i)))   
            return true;
        else if(isEnemy(chess_board, king_row, i) && !(isRook(chess_board, king_row, i) || isQueen(chess_board, king_row, i)))   
            break;
        
    }

    // Check up right diagonal
    if(isKing(chess_board, king_row-1, king_col+1))
        return true;
    if(chess_board->white_turn && !isMoveOutside(king_row-1, king_col+1) && isBlackPawn(chess_board, king_row-1, king_col+1))
        return true;
    for (int i = king_row-1, j = king_col+1; (i >= 0) && (j <= 7); i--, j++)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, j))
            break;
        if(isEnemy(chess_board, i, j) && (isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))
            return true;
        else if(isEnemy(chess_board, i, j) && !(isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))  
            break;
    }

    // Check down right diagonal
    if(isKing(chess_board, king_row+1, king_col+1))
        return true;
    if(!chess_board->white_turn && !isMoveOutside(king_row+1, king_col+1) && isWhitePawn(chess_board, king_row+1, king_col+1))
        return true;
    for (int i = king_row+1, j = king_col+1; (i <= 7) && (j <= 7); i++, j++)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, j))
            break;
        if(isEnemy(chess_board, i, j) && (isBishop(chess_board, i, j) || (isQueen(chess_board, i, j)))) 
            return true;
        else if(isEnemy(chess_board, i, j) && !(isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))  
            break;
    }

    // Check down left diagonal
    if(isKing(chess_board, king_row+1, king_col-1))
        return true;
    if(!chess_board->white_turn && !isMoveOutside(king_row+1, king_col-1) && isWhitePawn(chess_board, king_row+1, king_col-1))
        return true;
    for (int i = king_row+1, j = king_col-1; (i <= 7) && (j >= 0); i++, j--)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, j))
            break;
        if(isEnemy(chess_board, i, j) && (isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))
            return true;
        else if(isEnemy(chess_board, i, j) && !(isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))  
            break;
    }

    // Check up left diagonal
    if(isKing(chess_board, king_row-1, king_col-1))
        return true;
    if(chess_board->white_turn && !isMoveOutside(king_row-1, king_col-1) && isBlackPawn(chess_board, king_row-1, king_col-1))
        return true;
    for (int i = king_row-1, j = king_col-1; (i >= 0) && (j >= 0); i--, j--)
    {
        // If friend piece protecting break
        if(isFriend(chess_board, i, j))
            break;
        if(isEnemy(chess_board, i, j) && (isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))
            return true;
        else if(isEnemy(chess_board, i, j) && !(isBishop(chess_board, i, j) || (isQueen(chess_board, i, j))))  
            break;
    }
    
    // Check for knight check
    if(!isMoveOutside(king_row - 2, king_col + 1) && isEnemy(chess_board, king_row-2, king_col+1) && isKnight(chess_board, king_row-2, king_col+1))
        return true;
    else if(!isMoveOutside(king_row - 2, king_col - 1) && isEnemy(chess_board, king_row-2, king_col-1) && isKnight(chess_board, king_row-2, king_col-1))
        return true;
    else if(!isMoveOutside(king_row + 2, king_col + 1) && isEnemy(chess_board, king_row+2, king_col+1) && isKnight(chess_board, king_row+2, king_col+1))
        return true;
    else if(!isMoveOutside(king_row + 2, king_col - 1) && isEnemy(chess_board, king_row+2, king_col-1) && isKnight(chess_board, king_row+2, king_col-1))
        return true;

    return false;
}

// Return a list of all the squares protected by the enemy
struct Move_s* GetEnemyMoves(struct Board_s* chess_board)
{
    struct Move_s* moves_list = NULL;
    struct Piece_Position_s* pieces_positions;
    if(!chess_board->white_turn)
        pieces_positions = chess_board->white_pieces_position;
    else
        pieces_positions = chess_board->black_pieces_position;
    
    // Check all the board squares for ennemy pieces
    while(pieces_positions)
    {
        int row = pieces_positions->row;
        int col = pieces_positions->col;

        AddMoves(&moves_list, GetPieceMoves(chess_board, chess_board->squares[row*8+col], row, col, false));
    
        pieces_positions = pieces_positions->next_position;
    }

    return moves_list;
}

bool isMoveLegal(struct Board_s* chess_board, struct Move_s* moves_list, struct Move_s* move)
{

    // Cannot caapture a king
    if((chess_board->squares[move->row*8+move->col] == k) || (chess_board->squares[move->row*8+move->col] == K))
        return false;
    // Check if castling move
    int row = move->row;
    int col = move->col;
    int previous_row = move->previous_row;
    int previous_col = move->previous_col;
    if((previous_row == 7) && (previous_col == 4) && (row == 7) && (col == 6) && isCasltingLegal(chess_board, WHITE_KING_CASTLE) )
    {
        *move = (struct Move_s){.row = WHITE_KING_CASTLE, .col = WHITE_KING_CASTLE, .previous_row = WHITE_KING_CASTLE, .previous_col = WHITE_KING_CASTLE};
        return true;
    }
    else if ((previous_row == 7) && (previous_col == 4) && (row == 7) && (col == 1) && isCasltingLegal(chess_board, WHITE_QUEEN_CASTLE) )
    {
        return true;
    }

    if((row == previous_row) && (col == previous_col))
        return true;
    // If not in the list of moves
    struct Move_s* cursor = moves_list;
    while(cursor)
    {
        // If the move is in the list
        if((cursor->row == row) && (cursor->col == col))
        {
            // Virtually make the move
            struct Move_s move = {row, col, cursor->previous_row, cursor->previous_col};
            enum Piece_e captured = MakeMove(&move, chess_board, cursor->previous_row, cursor->previous_col);
            // If putting the king in danger
            //struct Move_s* enemy_moves = GetEnemyMoves(chess_board);
            if(isKingChecked(chess_board))
            {   
                UndoMove(captured, &move, chess_board, cursor->previous_row, cursor->previous_col);
                //FreeMoves(enemy_moves);

                #ifdef DEBUG
                    fprintf(stdout, "[INFO] King is checked !\n");
                #endif // DEBUG
                return false;
            }
            
            UndoMove(captured, &move, chess_board, cursor->previous_row, cursor->previous_col);
            //FreeMoves(enemy_moves);

            return true;
        }    
        cursor = cursor->next_move;
    }

    return false;
  
}

// Get all the enemy moves and check if one of the player move saves the king
bool isKingCheckedMated(struct Board_s* chess_board, struct Move_s* enememy_moves)
{
    // Check all player pieces
    /*for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(isFriend(chess_board, i, j))
            {
                // Get all the piece moves
                struct Move_s* piece_moves = GetPieceMoves(chess_board, chess_board->squares[i*8 + j], i, j, true);
                // If a move is legal
                if(piece_moves && (chess_board->holded_piece = chess_board->squares[i*8 + j]) && isMoveLegal(chess_board, piece_moves, piece_moves->row, piece_moves->col, i ,j))
                {
                    FreeMoves(piece_moves);
                    chess_board->squares[i*8 + j] = chess_board->holded_piece;
                    return false;
                } 

                FreeMoves(piece_moves);
            }
        }
        
    }*/
    return false;
}

// Move the holded piece to new square, return the captured piece
enum Piece_e MakeMove(struct Move_s* move, struct Board_s* chess_board, int previous_row, int previous_col)
{
    bool promoted = false;
    if(isKing(chess_board, move->row, move->col))
        return E;
    if( (move->row != move->previous_row) || (move->col != move->previous_col))
    {
        enum Piece_e holded_piece = chess_board->squares[move->previous_row*8 + move->previous_col];
        if(holded_piece == E)
            holded_piece = chess_board->holded_piece;
        enum Piece_e captured_piece = chess_board->squares[move->row*8 + move->col];
        chess_board->squares[previous_row * 8 + previous_col] = E;
        chess_board->squares[move->row*8 + move->col] = holded_piece;
        chess_board->holded_piece = E;

        // Pawn promotions
        /*if((holded_piece == p) && (move->row == 7))
        {
            chess_board->squares[move->row*8 + move->col] = q;
            promoted = true;
        }
        else if((holded_piece == P) && (move->row == 0))
        {
            chess_board->squares[move->row*8 + move->col] = q;
            promoted = true;
        }*/

        // Updating pieces positions
        if(chess_board->squares[move->row*8+move->col] == k)
            UpdatePiecePosition(&chess_board->black_king_position, move, promoted);

        else if(chess_board->squares[move->row*8+move->col] == K)
            UpdatePiecePosition(&chess_board->white_king_position, move, promoted);
        else if(chess_board->squares[move->row*8+move->col] == p)
            UpdatePiecePosition(&chess_board->black_pawns_positions, move, promoted);
        else if(chess_board->squares[move->row*8+move->col] == P)
            UpdatePiecePosition(&chess_board->white_pawns_positions, move, promoted);

        else if(chess_board->white_turn)
            UpdatePiecePosition(&chess_board->white_pieces_position, move, promoted);
        else
            UpdatePiecePosition(&chess_board->black_pieces_position, move, promoted);

        if(captured_piece != E)
        {
            if(captured_piece == p)
                DeletePiecePosition(&chess_board->black_pawns_positions, move->row, move->col);
            else if(captured_piece == P)
                DeletePiecePosition(&chess_board->white_pawns_positions, move->row, move->col);
            else if(chess_board->white_turn)
                DeletePiecePosition(&chess_board->black_pieces_position, move->row, move->col);
            else
                DeletePiecePosition(&chess_board->white_pieces_position, move->row, move->col);

        }

        // Updating the protected squares
        

        return captured_piece;
    }

    else{
        switch (move->row)
        {
            struct Move_s king_move;
            struct Move_s rook_move;
            case WHITE_KING_CASTLE:
                chess_board->squares[7*8 + 4] = E;
                chess_board->squares[7*8 + 7] = E;
                chess_board->squares[7*8 + 6] = K;
                chess_board->squares[7*8 + 5] = R;
                king_move = (struct Move_s){.row = 7, .col = 6, .previous_row = 7, .previous_col = 4};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 7, .col = 5, .previous_row = 7, .previous_col = 7};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag ^= (WHITE_KING_CASTLE | WHITE_QUEEN_CASTLE);
                break;
            case WHITE_QUEEN_CASTLE:
                chess_board->squares[7*8 + 4] = E;
                chess_board->squares[7*8 + 0] = E;
                chess_board->squares[7*8 + 1] = K;
                chess_board->squares[7*8 + 2] = R;
                king_move = (struct Move_s){.row = 7, .col = 1, .previous_row = 7, .previous_col = 4};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 7, .col = 2, .previous_row = 7, .previous_col = 0};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag ^= (WHITE_KING_CASTLE | WHITE_QUEEN_CASTLE);
                break;
             case BLACK_KING_CASTLE:
                chess_board->squares[0*8 + 4] = E;
                chess_board->squares[0*8 + 7] = E;
                chess_board->squares[0*8 + 6] = k;
                chess_board->squares[0*8 + 5] = r;
                king_move = (struct Move_s){.row = 0, .col = 6, .previous_row = 0, .previous_col = 4};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 0, .col = 5, .previous_row = 0, .previous_col = 7};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag ^= (BLACK_KING_CASTLE | BLACK_QUEEN_CASTLE);
                break;
            case BLACK_QUEEN_CASTLE:
                chess_board->squares[0*8 + 4] = E;
                chess_board->squares[0*8 + 0] = E;
                chess_board->squares[0*8 + 1] = k;
                chess_board->squares[0*8 + 2] = r;
                king_move = (struct Move_s){.row = 0, .col = 1, .previous_row = 0, .previous_col = 4};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 0, .col = 2, .previous_row = 0, .previous_col = 0};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag ^= (BLACK_KING_CASTLE | BLACK_QUEEN_CASTLE);
                break;
            default:
                break;
        }
    }

    return E;

}

void UndoMove(enum Piece_e captured_piece, struct Move_s* move, struct Board_s* chess_board, int previous_row, int previous_col)
{
    if( (move->row != move->previous_row) || (move->col != move->previous_col))
    {
        chess_board->squares[previous_row * 8 + previous_col] = chess_board->squares[move->row*8 + move->col];
        chess_board->squares[move->row*8 + move->col] = captured_piece;
        
        struct Move_s undo = {.row = move->previous_row, .col = move->previous_col, .previous_row = move->row, .previous_col = move->col};
        
        if(chess_board->squares[previous_row * 8 + previous_col] == k)
            UpdatePiecePosition(&chess_board->black_king_position, &undo, false);
        else if(chess_board->squares[previous_row * 8 + previous_col] == K)
            UpdatePiecePosition(&chess_board->white_king_position, &undo, false);
        else if(chess_board->squares[previous_row * 8 + previous_col] == p)
            UpdatePiecePosition(&chess_board->black_pawns_positions, &undo, false);
        else if(chess_board->squares[previous_row * 8 + previous_col] == P)
            UpdatePiecePosition(&chess_board->white_pawns_positions, &undo, false);
        else if(chess_board->white_turn)
            UpdatePiecePosition(&chess_board->white_pieces_position, &undo, false);
        else
            UpdatePiecePosition(&chess_board->black_pieces_position, &undo, false);

        if(captured_piece != E)
        {
            if(captured_piece == p)
                AddPiecePosition(&chess_board->black_pawns_positions, CreatePiecePosition(move->row, move->col));
            else if(captured_piece == P)
                AddPiecePosition(&chess_board->white_pawns_positions, CreatePiecePosition(move->row, move->col));
            else if(chess_board->white_turn)
                AddPiecePosition(&chess_board->black_pieces_position, CreatePiecePosition(move->row, move->col));
            else
                AddPiecePosition(&chess_board->white_pieces_position, CreatePiecePosition(move->row, move->col));

        }
    }

    else{
        switch (move->row)
        {
            struct Move_s king_move;
            struct Move_s rook_move;
            case WHITE_KING_CASTLE:
                chess_board->squares[7*8 + 4] = K;
                chess_board->squares[7*8 + 7] = R;
                chess_board->squares[7*8 + 6] = E;
                chess_board->squares[7*8 + 5] = E;
                king_move = (struct Move_s){.row = 7, .col = 4, .previous_row = 7, .previous_col = 6};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 7, .col = 7, .previous_row = 7, .previous_col = 5};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag |= WHITE_KING_CASTLE;
                break;
            case WHITE_QUEEN_CASTLE:
                chess_board->squares[7*8 + 4] = K;
                chess_board->squares[7*8 + 7] = R;
                chess_board->squares[7*8 + 1] = E;
                chess_board->squares[7*8 + 2] = E;
                king_move = (struct Move_s){.row = 7, .col = 4, .previous_row = 7, .previous_col = 1};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 7, .col = 0, .previous_row = 7, .previous_col = 2};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag |= WHITE_QUEEN_CASTLE;
                break;
             case BLACK_KING_CASTLE:
                chess_board->squares[0*8 + 4] = k;
                chess_board->squares[0*8 + 7] = r;
                chess_board->squares[0*8 + 6] = E;
                chess_board->squares[0*8 + 5] = E;
                king_move = (struct Move_s){.row = 0, .col = 4, .previous_row = 0, .previous_col = 6};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 0, .col = 7, .previous_row = 0, .previous_col = 5};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag |= BLACK_KING_CASTLE;
                break;
            case BLACK_QUEEN_CASTLE:
                chess_board->squares[0*8 + 4] = k;
                chess_board->squares[0*8 + 7] = r;
                chess_board->squares[0*8 + 1] = E;
                chess_board->squares[0*8 + 2] = E;
                king_move = (struct Move_s){.row = 0, .col = 1, .previous_row = 0, .previous_col = 4};
                UpdatePiecePosition(&chess_board->white_pieces_position, &king_move, false);
                rook_move = (struct Move_s){.row = 0, .col = 2, .previous_row = 0, .previous_col = 0};
                UpdatePiecePosition(&chess_board->white_pieces_position, &rook_move, false);
                chess_board->castle_flag |= BLACK_QUEEN_CASTLE;
                break;
            default:
                break;
        }
    }
}   

