#include "BoardGUI.h"
#include "AI.h"
//#define DEBUG

extern bool ai_turn;
extern struct AI_Node_s* ai_node;
gboolean DrawSquare(GtkDrawingArea* board_drawing_area, cairo_t* board_renderer, struct RenderingBoardData_s* rendering_data, 
                int i, int j, enum Piece_e piece, struct cairo_color_t color,
                double surface_square_size
               );

gboolean DrawChessBoard(GtkDrawingArea* board_drawing_area, cairo_t* board_renderer, gpointer data)
{
    // Get drawing area size, to adapt the drawing area squares dimensions 
    GtkAllocation* allocation = g_new(GtkAllocation, 1);
    gtk_widget_get_allocation(GTK_WIDGET(board_drawing_area), allocation);
    double width = allocation->width, height = allocation->height/8;
    // Get square size in sprite surface
    struct RenderingBoardData_s* rendering_data = data;
    int surface_width = cairo_image_surface_get_width(rendering_data->sprite_surface);
    int surface_height = cairo_image_surface_get_width(rendering_data->sprite_surface);
    double surface_square_size = surface_width/6;

    // Draw all board squares
    double scale = height / surface_square_size;
    enum Piece_e piece = B;
    struct cairo_color_t white = {1, 1, 1};
    //166, 68, 7
    struct cairo_color_t black = {166.f/255.f, 68.f/255.f, 7.f/255.f};
    struct cairo_color_t color;
    cairo_scale(board_renderer, scale, scale);

    for (int i = 0, k = 0; i < 8; i++ ) {
        for (int j = 0; j < 8; j++, k++) {
            if(!(k%2))
            {
                color = white;
            }
            else 
            {
                color = black;
            }
            DrawSquare(board_drawing_area, board_renderer, rendering_data, i, j, rendering_data->chess_board->squares[i * 8 + j], color, surface_square_size);
        }
        k++;
    }
    
    rendering_data->draw = true;

    #ifdef DEBUG
        fprintf(stdout, "[INFO] Board was drawn at %d %d\n", allocation->x, allocation->y);
    #endif
    g_free(allocation);
    return FALSE;
}

gboolean DrawSquare(GtkDrawingArea* board_drawing_area, cairo_t* board_renderer, struct RenderingBoardData_s* rendering_data, 
                int i, int j, enum Piece_e piece, struct cairo_color_t color,
                double surface_square_size
                 )
{
    double surface_x, surface_y;
    cairo_surface_t* sprite_surface = rendering_data->sprite_surface;
    cairo_set_source_rgb(board_renderer, color.r, color.g, color.r);
    switch (piece)
    {
    case K:
        surface_x = 0;
        surface_y = 0;
        break;
    case Q:
        surface_x = surface_square_size;
        surface_y = 0;
        break;
    case B:
        surface_x = surface_square_size*2;
        surface_y = 0;
        break;
    case N:
        surface_x = surface_square_size*3;
        surface_y = 0;
        break;
    case R:
        surface_x = surface_square_size*4;
        surface_y = 0;
        break;
    case P:
        surface_x = surface_square_size*5;
        surface_y = 0;
        break;
    case Pm:
        surface_x = surface_square_size*5;
        surface_y = 0;
        break;
    case k:
        surface_x = 0;
        surface_y = surface_square_size;
        break;
    case q:
        surface_x = surface_square_size;
        surface_y = surface_square_size;
        break;
    case b:
        surface_x = surface_square_size*2;
        surface_y = surface_square_size;
        break;
    case n:
        surface_x = surface_square_size*3;
        surface_y = surface_square_size;
        break;
    case r:
        surface_x = surface_square_size*4;
        surface_y = surface_square_size;
        break;
    case p:
        surface_x = surface_square_size*5;
        surface_y = surface_square_size;
        break;
    case pm:
        surface_x = surface_square_size*5;
        surface_y = surface_square_size;
        break;           
        
    }

    // Render the background color of the square
    cairo_rectangle(board_renderer, surface_square_size*j, surface_square_size*i, surface_square_size, surface_square_size);
    cairo_fill(board_renderer);

    // If hovered square
    if((i == rendering_data->player_row) && (j == rendering_data->player_col) && !rendering_data->holding)
    {
        cairo_set_source_rgba(board_renderer, 1, 0, 0, 0.5);
        cairo_rectangle(board_renderer, surface_square_size*rendering_data->player_col, surface_square_size*rendering_data->player_row, surface_square_size, surface_square_size);
        cairo_fill(board_renderer);
    }

    if(piece != E )
    {
        // Render the piece sprite into the board
        cairo_translate(board_renderer, -1 * surface_x,  -1 * surface_y);
        cairo_translate(board_renderer, j*surface_square_size, i * surface_square_size);
        cairo_set_source_surface(board_renderer, sprite_surface, 0, 0);
        // Crop the sprite surface for the piece located at surface_x/surface_y
        cairo_rectangle(board_renderer, surface_x, surface_y, surface_square_size, surface_square_size);
        cairo_fill(board_renderer);
        cairo_translate(board_renderer, -1*j*surface_square_size, -1 * i * surface_square_size);
        cairo_translate(board_renderer, surface_x, surface_y);
    }

    // If the player is holding a piece
    if(rendering_data->holding)
    {
        switch (rendering_data->holded_piece)
        {
        case K:
            surface_x = 0;
            surface_y = 0;
            break;
        case Q:
            surface_x = surface_square_size;
            surface_y = 0;
            break;
        case B:
            surface_x = surface_square_size*2;
            surface_y = 0;
            break;
        case N:
            surface_x = surface_square_size*3;
            surface_y = 0;
            break;
        case R:
            surface_x = surface_square_size*4;
            surface_y = 0;
            break;
        case P:
            surface_x = surface_square_size*5;
            surface_y = 0;
            break;
        case Pm:
            surface_x = surface_square_size*5;
            surface_y = 0;
            break;   
        case k:
            surface_x = 0;
            surface_y = surface_square_size;
            break;
        case q:
            surface_x = surface_square_size;
            surface_y = surface_square_size;
            break;
        case b:
            surface_x = surface_square_size*2;
            surface_y = surface_square_size;
            break;
        case n:
            surface_x = surface_square_size*3;
            surface_y = surface_square_size;
            break;
        case r:
            surface_x = surface_square_size*4;
            surface_y = surface_square_size;
            break;
        case p:
            surface_x = surface_square_size*5;
            surface_y = surface_square_size;
            break;
        case pm:
            surface_x = surface_square_size*5;
            surface_y = surface_square_size;
            break;               
        } 
 
        cairo_translate(board_renderer, -1 * surface_x,  -1 * surface_y);
        cairo_translate(board_renderer, ((double)rendering_data->player_x/(500.f/8.f)) * surface_square_size - surface_square_size/2,  ((double)rendering_data->player_y/(500.f/8.f)) * surface_square_size -  surface_square_size);
        cairo_set_source_surface(board_renderer, sprite_surface, 0, 0);
        // Crop the sprite surface for the piece located at surface_x/surface_y
        cairo_rectangle(board_renderer, surface_x, surface_y, surface_square_size, surface_square_size);
        cairo_fill(board_renderer);
        cairo_translate(board_renderer, -1*((double)rendering_data->player_x/(500.f/8.f)) * surface_square_size + surface_square_size/2, -1 *  ((double)rendering_data->player_y/(500.f/8.f)) * surface_square_size + surface_square_size);
        cairo_translate(board_renderer, surface_x, surface_y);
    }

    return FALSE;
}

// Get Player Cursor Coordinates update it in the structure
gboolean GetPlayerCursorCoordinates(GtkWidget* window, GdkEvent* event, gpointer user_data)
{
    gint row, col;
    struct RenderingBoardData_s* rendering_data = (struct RenderingBoardData_s*)user_data;
    GtkAllocation* allocation = NULL;

        allocation = g_new(GtkAllocation, 1);
        // Get mouse position in window space 
        gtk_widget_get_allocation(window, allocation);
        double renderer_square_size = (double)allocation->width/8.f; 
        gtk_widget_get_pointer (window, &col, &row);
        // Get mouse position in drawing area space
        gtk_widget_get_allocation(GTK_WIDGET(rendering_data->board_drawing_area), allocation);
        rendering_data->player_y = row; rendering_data->player_x = col; 
        row = row/renderer_square_size; col = col/renderer_square_size; 
        rendering_data->player_row = row; rendering_data->player_col = col;
        // Call draw signal
        gtk_widget_queue_draw(GTK_WIDGET(rendering_data->board_drawing_area));

        #ifdef DEBUG
            fprintf(stdout, "[INFO] Motion Event cursor position : %d %d\n", rendering_data->player_x, rendering_data->player_y);
            fprintf(stdout, "[INFO] Motion Event Board position : %d %d\n", row, col);
        #endif // DEBUG
        if(allocation)
            g_free(allocation);

        return FALSE;
}