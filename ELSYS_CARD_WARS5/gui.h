#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "player.h"

#define MAX_PLACED_CARDS 10
#define MAX_TEXTURE_CACHE_SIZE 100

#define WIDTH 800
#define HEIGHT 600

typedef struct{
    card_t card;
    graph_object layout;
    bool_t focused;
    bool_t dragged;
}card_graph;

typedef struct{
    player_t player;
    graph_object avatar;
}player_graph;

typedef struct{
    player_t first_player;
    player_t second_player;
    card_t cards_on_board[MAX_PLACED_CARDS];
    SDL_Texture* background_texture;
    graph_object sidebar;
    graph_object cursor;
} board_graph;

typedef struct{
    SDL_Event* event;
    board_graph* board;
    SDL_Renderer* renderer;
    SDL_Color* clear_color;
    SDL_Window* window;
}timer_params;

typedef struct{
    SDL_Texture* textures[100];
}textures_cache;

void compose_scene(SDL_Renderer* renderer,board_graph* board);
int render_scene(SDL_Window *window,SDL_Renderer *renderer,SDL_Color *clear_color,board_graph* board);
Uint32 my_callback(Uint32 interval,void* param);
SDL_Surface* surface_from_path(SDL_Renderer *renderer, char* filepath);

#endif // GUI_H_INCLUDED
