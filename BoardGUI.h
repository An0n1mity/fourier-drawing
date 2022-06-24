#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <gtk/gtk.h>
#include <inttypes.h>
#include "BoardLogic.h"

struct RenderingBoardData_s{
    GtkWindow* window;
    cairo_surface_t* sprite_surface;
    struct Board_s* chess_board;
    GtkDrawingArea* board_drawing_area;   
    // Player cursor in square coordinates     
    int player_row;
    int player_col;
    // Is the player holding a piece
    bool holding;
    // The current piece being hold
    enum Piece_e holded_piece;
    // Where the grabbed piece was picked
    int picked_row;
    int picked_col;
    // Player mouse cursor in drawing are coordinates
    int player_x;
    int player_y;
    
    bool draw;
};

struct cairo_color_t {
    double r;
    double g;
    double b; 
};

// Draw the entire chess board 
gboolean DrawChessBoard(GtkDrawingArea* board_drawing_area, cairo_t* board_renderer, gpointer data);
// Get Player Cursor Coordinates
gboolean GetPlayerCursorCoordinates(GtkWidget* window, GdkEvent* event, gpointer user_data);
#endif