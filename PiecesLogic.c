#include "PiecesLogic.h"

void AddMove(struct Move_s** moves_list, struct Move_s* move_to_add)
{
    if(!moves_list || !move_to_add)
    {
        return;
    }

    if(!(*moves_list))
    {
        *moves_list = move_to_add;
        return;
    }

    move_to_add->next_move = (*moves_list);
    *moves_list = move_to_add;
}

struct Move_s* CreateMove(int row, int col, int previous_row, int previous_col)
{
    struct Move_s* move = (struct Move_s*)calloc(1, sizeof(struct Move_s));
    move->row = row;
    move->col = col;
    move->previous_row = previous_row;
    move->previous_col = previous_col;

    return move;
}


void AddMoves(struct Move_s** moves_list, struct Move_s* moves_to_add)
{
    if(!moves_list || !moves_to_add)
        {
            return;
        }

        if(!(*moves_list))
        {
            *moves_list = moves_to_add;
            return;
        }

        // Go to the end
        struct Move_s* cursor = *moves_list;
        while (cursor && cursor->next_move)
        {
            cursor = cursor->next_move;
        }
        cursor->next_move = moves_to_add;
}

void FreeMoves(struct Move_s* moves_list)
{
    if(!moves_list)
    {
        return;
    }

    struct Move_s* tmp;
    while(moves_list)
    {
        tmp = moves_list->next_move;
        free(moves_list);
        moves_list = tmp;
    }

    moves_list = NULL;
}

struct Move_s* GetPawnMoves(struct Board_s* restrict chess_board, enum Piece_e pawn, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    int front;
    // If white pawn and not displaced
    if(pawn == P)
    {
        if((pawn == P) && (row == 6))
            front = row-2;
        else 
            front = row - 1;
        // Check each row in front of the pawn
        for (int i = row-1; i >= front; i--)
        {
            // If no piece in front move is possible
            move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
            if((chess_board->squares[i*8 + col] == E) && !isMoveIllegal(chess_board, &move))
            {
                AddMove(&moves_list, CreateMove(i, col, row, col));
            }
            else
                break;

        }

        // Check for enemy in diagonal
        move = (struct Move_s){.row = row-1, .col = col-1, .previous_row = row, .previous_col = col};
        if(!isMoveOutside(row-1, col-1) && isEnemy(chess_board, row-1, col-1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row-1, col-1, row, col));
        }
        move = (struct Move_s){.row = row-1, .col = col+1, .previous_row = row, .previous_col = col};
        if (!isMoveOutside(row-1, col+1) && isEnemy(chess_board, row-1, col+1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row-1, col+1, row, col));
        }

        return moves_list;
        
    }

    else if(pawn == p)
    {
        if((pawn == p) && (row == 1))
            front = row+2;
        else
            front = row + 1;
        // Check each row in front of the pawn
        for (int i = row+1; i <= front; i++)
        {
            // If no piece in front move is possible
            move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
            if((chess_board->squares[i*8 + col] == E) && !isMoveIllegal(chess_board, &move))
            {
                AddMove(&moves_list, CreateMove(i, col, row, col));
            }
             else
                break;

        }

        // Check for enemy in diagonal
        move = (struct Move_s){.row = row+1, .col = col-1, .previous_row = row, .previous_col = col};
        if(!isMoveOutside(row+1, col-1) && isEnemy(chess_board, row+1, col-1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row+1, col-1, row, col));
        }
        move = (struct Move_s){.row = row+1, .col = col+1, .previous_row = row, .previous_col = col};
        if (!isMoveOutside(row+1, col+1) && isEnemy(chess_board, row+1, col+1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row+1, col+1, row, col));
        }
        
        return moves_list;

    }

    return moves_list;

}

struct Move_s* GetPawnCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e pawn, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    int front;
    // If white pawn and not displaced
    if(pawn == P || pawn == Pm)
    {
        if((pawn == P) && (row == 6))
            front = row-2;
        else 
            front = row - 1;

        // Check for enemy in diagonal
        move = (struct Move_s){.row = row-1, .col = col-1, .previous_row = row, .previous_col = col};
        if(!isMoveOutside(row-1, col-1) && isEnemy(chess_board, row-1, col-1)&& !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row-1, col-1, row, col));
        }
        move = (struct Move_s){.row = row-1, .col = col+1, .previous_row = row, .previous_col = col};
        if (!isMoveOutside(row-1, col+1) && isEnemy(chess_board, row-1, col+1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row-1, col+1, row, col));
        }

        return moves_list;
        
    }

    else if(pawn == p || pawn == pm)
    {
        if((pawn == p)  && (row == 1))
            front = row+2;
        else 
            front = row + 1;

        // Check for enemy in diagonal
        move = (struct Move_s){.row = row+1, .col = col-1, .previous_row = row, .previous_col = col};
        if(!isMoveOutside(row+1, col-1) && isEnemy(chess_board, row+1, col-1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row+1, col-1, row, col));
        }
        move = (struct Move_s){.row = row+1, .col = col+1, .previous_row = row, .previous_col = col};
        if (!isMoveOutside(row+1, col+1) && isEnemy(chess_board, row+1, col+1) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row+1, col+1, row, col));
        }
        
        return moves_list;

    }

}

void GetPawnProtectedSquares(struct Board_s* cheess_board, enum Piece_e pawn, int row, int col)
{
    if(pawn == P)
    {
        if(!isMoveOutside(row-1, col-1))
            cheess_board->protected_squares[(row-1)*8 + col-1] |= WHITE_PROTECTED;
        if(!isMoveOutside(row-1, col+1))
            cheess_board->protected_squares[(row-1)*8 + col+1] |= WHITE_PROTECTED;
    }
    else if(pawn == p)
    {
        if(!isMoveOutside(row+1, col-1))
            cheess_board->protected_squares[(row+1)*8 + col-1] |= BLACK_PROTECTED;
        if(!isMoveOutside(row+1, col+1))
            cheess_board->protected_squares[(row+1)*8 + col+1] |= BLACK_PROTECTED;
    }
}

struct Move_s* GetRookMoves(struct Board_s* restrict chess_board, enum Piece_e rook, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    // Check up
    for (int i = row-1; i >= 0; i--)
    {
        if(isFriend(chess_board, i, col))
            break;
        move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(i, col, row, col));
        if(isEnemy(chess_board, i, col))
            break;
    }

    // Check down
    for (int i = row+1; i < 8; i++)
    {
        if(isFriend(chess_board, i, col))
            break;
        move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(i, col, row, col));
        if(isEnemy(chess_board, i, col))
            break;
    }

    // Check left
    for (int i = col-1; i >= 0; i--)
    {
        if(isFriend(chess_board, row, i))
            break;
        move = (struct Move_s){.row = row, .col = i, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(row, i, row, col));
        if(isEnemy(chess_board, row, i))
            break;
    }

     // Check right
    for (int i = col+1; i < 8; i++)
    {
        if(isFriend(chess_board, row, i))
            break;
        move = (struct Move_s){.row = row, .col = i, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(row, i, row, col));
        if(isEnemy(chess_board, row, i))
            break;
    }
    
    return moves_list;
}

struct Move_s* GetRookCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e rook, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    // Check up
    for (int i = row-1; i >= 0; i--)
    {
        move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
        if(isFriend(chess_board, i, col))
            break;
        if(isEnemy(chess_board, i, col) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(i, col, row, col));
            break;
        }
    }

    // Check down
    for (int i = row+1; i < 8; i++)
    {
        move = (struct Move_s){.row = i, .col = col, .previous_row = row, .previous_col = col};
        if(isFriend(chess_board, i, col))
            break;
        if(isEnemy(chess_board, i, col) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(i, col, row, col));
            break;
        }
    }

    // Check left
    for (int i = col-1; i >= 0; i--)
    {
        move = (struct Move_s){.row = row, .col = i, .previous_row = row, .previous_col = col};
        if(isFriend(chess_board, row, i))
            break;
        if(isEnemy(chess_board, row, i) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row, i, row, col));
            break;
        }
    }

     // Check right
    for (int i = col+1; i < 8; i++)
    {
        move = (struct Move_s){.row = row, .col = i, .previous_row = row, .previous_col = col};
        if(isFriend(chess_board, row, i))
            break;
        if(isEnemy(chess_board, row, i) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(row, i, row, col));
            break;
        }
    }
    
    return moves_list;
}

void GetRookProtectedSquares(struct Board_s* chess_board, enum Piece_e rook, int row, int col)
{
    // Check up
    for (int i = row-1; i >= 0; i--)
    {
        if(rook == R)
        {
            chess_board->protected_squares[i*8+col] |= WHITE_PROTECTED;
        }
        else if(rook == r)
        {
            chess_board->protected_squares[i*8+col] |= BLACK_PROTECTED;
        }
        if(isEnemy(chess_board, i, col) || isFriend(chess_board, i, col))
            break;
    }

    // Check down
    for (int i = row+1; i < 8; i++)
    {
        if(rook == R)
        {
            chess_board->protected_squares[i*8+col] |= WHITE_PROTECTED;
        }
        else if(rook == r)
        {
            chess_board->protected_squares[i*8+col] |= BLACK_PROTECTED;
        }
        if(isEnemy(chess_board, i, col) || isFriend(chess_board, i, col))
                break;
    }

    // Check right
    for (int i = col+1; i < 8; i++)
    {
        if(rook == R)
        {
            chess_board->protected_squares[row*8+i] |= WHITE_PROTECTED;
        }
        else if(rook == r)
        {
            chess_board->protected_squares[row*8+i] |= BLACK_PROTECTED;
        }
        if(isEnemy(chess_board, row, i) || isFriend(chess_board, row, i))
                break;
    }

    // Check left
    for (int i = col-1; i >= 0; i--)
    {
        if(rook == R)
        {
            chess_board->protected_squares[row*8+i] |= WHITE_PROTECTED;
        }
        else if(rook == r)
        {
            chess_board->protected_squares[row*8+i] |= BLACK_PROTECTED;
        }
        if(isEnemy(chess_board, row, i) || isFriend(chess_board, row, i))
                break;
    }

    
}

struct Move_s* GetBishopMoves(struct Board_s* restrict chess_board, enum Piece_e bishop, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;

    int i = row - 1, j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(i, j, row, col));
        if(isEnemy(chess_board, i, j))
            break;
        i--;
        j++;
    }

    i = row + 1; j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(i, j, row, col));
        if(isEnemy(chess_board, i, j))
            break;
        i++;
        j++;
    }

    i = row + 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))        
            AddMove(&moves_list, CreateMove(i, j, row, col));
        if(isEnemy(chess_board, i, j))
            break;
        i++;
        j--;
    }

    i = row - 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(!isMoveIllegal(chess_board, &move))
            AddMove(&moves_list, CreateMove(i, j, row, col));
        if(isEnemy(chess_board, i, j))
            break;
        i--;
        j--;
    }
    

    return moves_list;
}

struct Move_s* GetBishopCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e bishop, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    int i = row - 1, j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(i, j, row, col));
            break;
        }
        i--;
        j++;
    }

    i = row + 1; j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(i, j, row, col));
            break;
        }
        i++;
        j++;
    }

    i = row + 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move))
        {
            AddMove(&moves_list, CreateMove(i, j, row, col));
            break;
        }
        i++;
        j--;
    }

    i = row - 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(isFriend(chess_board, i, j))
            break;
        move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
        if(isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move) )
        {
            AddMove(&moves_list, CreateMove(i, j, row, col));
            break;
        }
        i--;
        j--;
    }
    

    return moves_list;
}

void GetBishopProtectedSquares(struct Board_s* chess_board, enum Piece_e bishop, int row, int col)
{
    // Check up right diagonal
    int i = row - 1, j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(bishop == B)
        {
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        }
        else if(bishop == b)
        {
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
        }
        if(isFriend(chess_board, i, j) || isEnemy(chess_board, i, j))
            break;
        i--;
        j++;
    }

    i = row + 1; j = col + 1;
    while(!isMoveOutside(i, j))
    {
        if(bishop == B)
        {
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        }
        else if(bishop == b)
        {
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
        }
        if(isFriend(chess_board, i, j) || isEnemy(chess_board, i, j))
            break;
        i--;
        j++;
    }

    i = row + 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(bishop == B)
        {
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        }
        else if(bishop == b)
        {
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
        }
        if(isFriend(chess_board, i, j) || isEnemy(chess_board, i, j))
            break;
        i--;
        j++;
    }

    i = row - 1; j = col - 1;
    while(!isMoveOutside(i, j))
    {
        if(bishop == B)
        {
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        }
        else if(bishop == b)
        {
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
        }
        if(isFriend(chess_board, i, j) || isEnemy(chess_board, i, j))
            break;
        i--;
        j++;
    }

}

struct Move_s* GetKnightMoves(struct Board_s* restrict chess_board, enum Piece_e knight, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;

    int i = row - 2, j = col + 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 2; j = col - 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 2; j = col + 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 2; j = col - 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col + 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1; j = col - 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1; j = col + 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1; j = col - 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    
    return moves_list;
}

struct Move_s* GetKnightCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e knight, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;

    int i = row - 2, j = col + 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 2; j = col - 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 2; j = col + 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 2; j = col - 1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col + 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};  
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1; j = col - 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1; j = col + 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1; j = col - 2;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    
    return moves_list;
}

void GetKnightProtectedSquares(struct Board_s* chess_board, enum Piece_e knight, int row, int col)
{
    int i = row - 2, j = col + 1;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    }    
    
    i = row - 2; j = col - 1;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 2; j = col + 1;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 2; j = col - 1;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row - 1, j = col + 2;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row - 1; j = col - 2;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 1; j = col + 2;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 
    
    i = row + 1; j = col - 2;
    if(!isMoveOutside(i, j))
    {
        if(knight == N)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(knight == n)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 
    
}

struct Move_s* GetQueenMoves(struct Board_s* restrict chess_board, enum Piece_e queen, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s* bishop_moves = GetBishopMoves(chess_board, queen, row, col);
    struct Move_s* rook_moves = GetRookMoves(chess_board, queen, row, col);
    AddMoves(&moves_list, bishop_moves);
    AddMoves(&moves_list, rook_moves);

    return moves_list;
}

struct Move_s* GetQueenCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e queen, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s* bishop_moves = GetBishopCaptureMoves(chess_board, queen, row, col);
    struct Move_s* rook_moves = GetRookCaptureMoves(chess_board, queen, row, col);
    AddMoves(&moves_list, bishop_moves);
    AddMoves(&moves_list, rook_moves);

    return moves_list;
}

void GetQueenProtectedSquares(struct Board_s* chess_board, enum Piece_e queen, int row, int col)
{
    if(queen == Q)
    {
        GetRookProtectedSquares(chess_board, R, row, col);
        GetBishopProtectedSquares(chess_board, B, row, col);    
    }
    else if(queen == q)
    {
        GetRookProtectedSquares(chess_board, r, row, col);
        GetBishopProtectedSquares(chess_board, b, row, col);  
    }
}

struct Move_s* GetKingMoves(struct Board_s* restrict chess_board, enum Piece_e king, int row, int col, bool castling)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;
    // Classical king's moves
    int i = row - 1, j = col;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) || isEmpty(chess_board, i, j)) && !isMoveIllegal(chess_board, &move))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    if(castling)
    {
        //AddMoves(&moves_list, GetCastleMoves(chess_board, king, row, col));
    }
    return moves_list;
}

struct Move_s* GetKingCaptureMoves(struct Board_s* restrict chess_board, enum Piece_e queen, int row, int col)
{
    struct Move_s* moves_list = NULL;
    struct Move_s move;

    int i = row - 1, j = col;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col+1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row + 1, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));
    i = row - 1, j = col-1;
    move = (struct Move_s){.row = i, .col = j, .previous_row = row, .previous_col = col};
    if(!isMoveOutside(i, j) && (isEnemy(chess_board, i, j) && !isMoveIllegal(chess_board, &move)))
        AddMove(&moves_list, CreateMove(i, j, row, col));


    return moves_list;
}

void GetKingProtectedSquares(struct Board_s* chess_board, enum Piece_e king, int row, int col)
{
    int i = row - 1, j = col;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row - 1, j = col+1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row, j = col+1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 1, j = col+1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 1, j = col;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row + 1, j = col-1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row, j = col-1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

    i = row - 1, j = col-1;
    if(!isMoveOutside(i, j))
    {
        if(king == K)
            chess_board->protected_squares[i*8+j] |= WHITE_PROTECTED;
        else if(king == k)
            chess_board->protected_squares[i*8+j] |= BLACK_PROTECTED;
    } 

}

struct Move_s* GetCastleMoves(struct Board_s* restrict chess_board, enum Piece_e king, int row, int col)
{
    struct Move_s* castle_moves = NULL;
    // Special king's moves/castling
    if(king == K)
    {
        // Check for king castle
        if(isCasltingLegal(chess_board, chess_board->castle_flag & WHITE_KING_CASTLE))
        {
            AddMove(&castle_moves, CreateMove(WHITE_KING_CASTLE, WHITE_KING_CASTLE, WHITE_KING_CASTLE, WHITE_KING_CASTLE));
        }
        if(isCasltingLegal(chess_board, chess_board->castle_flag & WHITE_QUEEN_CASTLE))
        {
            AddMove(&castle_moves, CreateMove(WHITE_QUEEN_CASTLE, WHITE_QUEEN_CASTLE, WHITE_QUEEN_CASTLE, WHITE_QUEEN_CASTLE));
        }
    }

    else{
         // Check for king castle
        if(isCasltingLegal(chess_board, chess_board->castle_flag & BLACK_KING_CASTLE))
        {
            AddMove(&castle_moves, CreateMove(BLACK_KING_CASTLE, BLACK_KING_CASTLE, BLACK_KING_CASTLE, BLACK_KING_CASTLE));
        }
        if(isCasltingLegal(chess_board, chess_board->castle_flag & BLACK_QUEEN_CASTLE))
        {
            AddMove(&castle_moves, CreateMove(BLACK_QUEEN_CASTLE, BLACK_QUEEN_CASTLE, BLACK_QUEEN_CASTLE, BLACK_QUEEN_CASTLE));
        }
    }

    return NULL;
}
