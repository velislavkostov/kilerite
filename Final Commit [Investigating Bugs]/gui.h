#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "board.h"
#include <stdio.h>
#include <string.h> //string.h to use memset,memcpy and so on !
#include <stdlib.h> //malloc,free
#include <math.h>

#define MAX_PLACED_CARDS 10

typedef struct{
    SDL_Event* event;
    board_t* board;
    SDL_Renderer* renderer;
    SDL_Color* clear_color;
    SDL_Window* window;
    bool_t* running;
    int* player_id;
}render_data_t;

void compose_scene(void* param);
int render_scene(void* param);
int query_display_mode(SDL_DisplayMode *display_mode);
Uint32 main_timer_callback(Uint32 interval,void* param);
int handle_player_input(void* param);
int design_card(SDL_Renderer* renderer,card_t *card,surface_cache_t* cache,int card_index);
surface_cache_t* create_preloaded_cache(SDL_Renderer* renderer);
Uint32 animator_timer_callback(Uint32 interval, void* param);
Uint32 turn_callback(Uint32 interval,void* param);
SDL_Surface* two_digit_number(surface_cache_t* cache,int number);
int arrange_board(SDL_Renderer* renderer, board_t* board);

#endif // GUI_H_INCLUDED
