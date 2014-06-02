#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "player.h"
typedef struct{
    SDL_Texture* texture;
    SDL_Rect rectangle;
    int offset_x;
    int offset_y;
    int old_x;
    int old_y;
}cursor_t;

typedef struct{
    SDL_Surface* background;
    SDL_Surface* cursor;
    SDL_Surface* cards[DECK_SIZE];
    SDL_Texture* blank_card;
    SDL_Texture* mana_crystal;
    SDL_Texture* health_crystal;
    SDL_Surface* digits[10];
}surface_cache_t;

typedef struct{
    player_t players[2];
    card_t cards_on_board[10];
    SDL_Texture* background_texture;
    cursor_t cursor;
    SDL_DisplayMode display_mode;
    int player_id;
    surface_cache_t* cache;
    card_t card_pool[7];
}board_t;

int init_board(SDL_Renderer* renderer,board_t *board);

#endif // BOARD_H_INCLUDED
