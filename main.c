#include "BoardGUI.h"
#include "BoardLogic.h"
#include "AI.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>

#define DEBUG
bool ai_turn = false;
struct AI_Node_s* ai_node = NULL;

gint Test(gpointer user_data)
{
    struct RenderingBoardData_s* rendering_data = (struct RenderingBoardData_s*)user_data;
    struct Board_s* chess_board = rendering_data->chess_board;
    if(rendering_data->holding)
    {
        GetPlayerCursorCoordinates(GTK_WIDGET(rendering_data->window), NULL, user_data);
        gtk_widget_queue_draw(GTK_WIDGET(rendering_data->board_drawing_area));
    }
            gtk_widget_queue_draw(GTK_WIDGET(rendering_data->board_drawing_area));

    if(!chess_board->white_turn && rendering_data->draw)
    {
            int nb_nodes = 0;
             struct Move_s** killer_moves = CreateKillerMovesArray(7);
                clock_t start = clock();
                MinMax(chess_board, killer_moves, 4, INT_MIN, INT_MAX, &nb_nodes);
                clock_t difference = clock() - start;
                int msec = difference * 1000 / CLOCKS_PER_SEC;
                printf("Nodes : %d in %ds and %dms\n", nb_nodes, msec/1000, msec%1000);
                printf("%d %d\n", chess_board->white_king_position->row, chess_board->white_king_position->col);
                for (int i = 0; i < 7; i++)
                {
                    FreeMoves(killer_moves[i*2]);
                    FreeMoves(killer_moves[i*2+1]);
                    FreeMoves(killer_moves[i]);
                }
                free(killer_moves);
                
                chess_board->white_turn = true;
                rendering_data->draw = false;

    }
    return G_SOURCE_CONTINUE;

}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event, gpointer user_data){
    struct RenderingBoardData_s* rendering_data = (struct RenderingBoardData_s*)user_data;
    struct Board_s* chess_board = rendering_data->chess_board;
    int player_row = rendering_data->player_row; 
    int player_col = rendering_data->player_col; 
    // Check if the player clicked on one of his pieces
    if(isPlayerPiece(rendering_data->chess_board, rendering_data->player_row, rendering_data->player_col))
    {
        // Remove the piece from the board and store it in grabed variable
        rendering_data->holded_piece = chess_board->squares[player_row * 8 + player_col];        
        chess_board->squares[player_row * 8 + player_col] = E;
        // The player is holding the piece
        rendering_data->holding = true;
        chess_board->holded_piece = rendering_data->holded_piece;
        // Stores where the piece was picked up 
        rendering_data->picked_row = player_row;
        rendering_data->picked_col = player_col;

        gtk_widget_queue_draw(GTK_WIDGET(rendering_data->board_drawing_area));

        #ifdef DEBUG
            fprintf(stdout, "[INFO] Click Event player holding a piece\n");
        #endif // DEBUG
    }
    
    #ifdef DEBUG
        fprintf(stdout, "[INFO] Click Event Board position : %d %d\n", rendering_data->player_row, rendering_data->player_col);
    #endif // DEBUG
    return TRUE;
}

GtkDialog* dialog;

static gboolean released(GtkWidget *window, GdkEventButton *event, gpointer user_data){
    struct RenderingBoardData_s* rendering_data = (struct RenderingBoardData_s*)user_data;
    // Check if the player has moved the piece 
    int previous_player_row = rendering_data->picked_row; 
    int previous_player_col = rendering_data->picked_col; 
    struct Board_s* chess_board = rendering_data->chess_board;
    if(rendering_data->holding)
    {
        if(rendering_data->holded_piece == E){
            fprintf(stderr, "[ERROR] Player holding a empty piece\n");
            return TRUE;
        }
        // The player has released the grabed piece
        rendering_data->holding = false;
        // Check if he moved the piece away
        GetPlayerCursorCoordinates(window, NULL, user_data);
        if((previous_player_col != rendering_data->player_col) || (previous_player_row != rendering_data->player_row))
        {
            // Check if the move is legal
            #ifdef DEBUG
                fprintf(stdout, "[INFO] Getting moves for the piece\n");
            #endif
            enum Piece_e holded = chess_board->holded_piece;
            struct Move_s* moves_list = GetPieceMoves(chess_board,  rendering_data->holded_piece, previous_player_row, previous_player_col, true);
            struct Move_s move = {.row = rendering_data->player_row, .col = rendering_data->player_col, .previous_row = previous_player_row, .previous_col = previous_player_col};
            chess_board->holded_piece = holded;
            if(isMoveLegal(chess_board, moves_list, &move))
            {
                // Move the piece
                #ifdef DEBUG
                    fprintf(stdout, "[INFO] Move is legal\n");
                #endif
                // Special case for the pawn 
                //if(rendering_data->holded_piece == p || rendering_data->holded_piece == P)
                    //rendering_data->holded_piece++;
                MakeMove(&move, chess_board, previous_player_row, previous_player_col);
                rendering_data->draw = false;
                /*chess_board->squares[ rendering_data->player_row * 8 +  rendering_data->player_col] = rendering_data->holded_piece;
                chess_board->squares[ previous_player_row * 8 +  previous_player_col] = E;
                rendering_data->holded_piece = E;*/

                //chess_board->white_turn = !chess_board->white_turn;
                //ai_turn = true;
                // Check if the move check mated opponent king
                struct Move_s* enemy_moves = GetEnemyMoves(chess_board);
                if(isKingCheckedMated(chess_board, enemy_moves)){
                        fprintf(stdout, "[INFO] Check Mate\n");
                        gtk_widget_show_all (GTK_WIDGET(dialog));
                }
                FreeMoves(enemy_moves);

                /* AI TESTING !*/
                // Create Node from actual state
                

                ai_node = NULL;
                chess_board->white_turn = false;
                //uint64_t* random_array = RandomArray();
                //uint64_t hash = ZobristHash(chess_board, random_array);
                //printf("%lld\n", hash);
                //free(random_array);   
                //FreeAINode(ai_node);

            }
            // If the move is illegal put down the piece where it was
            else{
                chess_board->squares[previous_player_row * 8 + previous_player_col] = rendering_data->holded_piece;
                #ifdef DEBUG
                    fprintf(stdout, "[INFO] Move is illegal\n");
                #endif
            }
            #ifdef DEBUG
            fprintf(stdout, "[INFO] Player has moved from %d %d to %d %d\n", previous_player_row, previous_player_col, rendering_data->player_row, rendering_data->player_col);
            #endif // DEBUG
            // Freeing the moves list
            FreeMoves(moves_list);
        }
        // Put the piece back where it was
        else{
            chess_board->squares[previous_player_row * 8 + previous_player_col] = rendering_data->holded_piece;
        }
    }
    #ifdef DEBUG
        fprintf(stdout, "[INFO] Release Event Board position : %d %d\n", rendering_data->player_row, rendering_data->player_col);
    #endif // DEBUG
    return FALSE;
}

int main(int argc, char* argv[]) {
    // Initialize gtk 
    gtk_init(&argc, &argv);
    // Gtk error handling
    GError* error = NULL;
    // Create and initialize gtk builder
    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../gui.glade", &error);
    // Report error 
    if(error)
    {   
        fprintf(stderr, "Builder error : %s", error->message);
        g_error_free(error);
        return 1;
    }
    
    // Get gtk element from glade file
    GtkWindow* window = (GtkWindow*) gtk_builder_get_object (builder, "window");
    if(!window)
    {
        fprintf(stderr, "[ERROR] Builder error : window is NULL");
        return 1;
    }
    // Get gtk ialog for win state
    dialog = (GtkDialog*)gtk_builder_get_object(builder, "dialog");

    // Drawing area where to render the chess board
    GtkDrawingArea* board_drawing_area = (GtkDrawingArea*) gtk_builder_get_object(builder, "board_drawing_area");
    if(!board_drawing_area)
    {
        fprintf(stderr, "[ERROR] Builder error : board_drawing_area is NULL");
        return 1;
    }
    gtk_widget_add_events(GTK_WIDGET(board_drawing_area),  GDK_POINTER_MOTION_MASK);
    // Chess pieces sprite 
    cairo_surface_t* sprite = cairo_image_surface_create_from_png("../sprite.png");
    if(!sprite)
    {
        fprintf(stderr, "[ERROR] Cairo error : sprite is NULL");
        return 1;
    }
    struct Board_s* chess_board = CreateBoardFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
    //chess_board->white_turn = false;
    struct RenderingBoardData_s rendering_data = {window, sprite, chess_board, board_drawing_area, 0, 0, false, false};
    gtk_widget_set_size_request (GTK_WIDGET(board_drawing_area), 500, 500);
    g_signal_connect (board_drawing_area, "draw", G_CALLBACK (DrawChessBoard), &rendering_data);
    g_signal_connect(window, "button-press-event", G_CALLBACK(clicked), &rendering_data);
    g_signal_connect (board_drawing_area, "motion-notify-event", G_CALLBACK (GetPlayerCursorCoordinates), &rendering_data);
    g_signal_connect (window, "button-release-event",G_CALLBACK (released), &rendering_data);
    g_timeout_add (10, Test, &rendering_data);
    gtk_widget_show_all (GTK_WIDGET(window));

    gtk_main();

    return 0;
}
