#include "board.h"

int init_board(SDL_Renderer* renderer, board_t *board){
    if(renderer != NULL && board != NULL){
        printf("[INIT]\n\tInitializing board...\n");
        memset(board,0,sizeof(board_t));
        init_player(&board->players[0]);
        init_player(&board->players[1]);
        board->cache = create_preloaded_cache(renderer);
        int i;
        for(i = 0; i < 7; i++){
            calculate_vital_ratios(&board->card_pool[i]);
            design_card(renderer,&board->card_pool[i],board->cache,i);
        }
        for(i = 0; i < DECK_SIZE; i++){
            push_card(board->card_pool[rand() % 7],&board->players[0].deck_of_cards);
            push_card(board->card_pool[rand() % 7],&board->players[1].deck_of_cards);
        }
        printf("\t[LOAD]\n\t\tLoading resources...\n");
        SDL_Surface* load_surface = NULL;
        if(board->cache){
            load_surface = board->cache->background;
            if(load_surface){
                board->background_texture = SDL_CreateTextureFromSurface(renderer,load_surface);
                load_surface = NULL;
            }

            load_surface = board->cache->cursor;
            if(load_surface){
                SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,0,0,0));
                board->cursor.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
                SDL_SetTextureColorMod(board->cursor.texture,0,0,0);
                load_surface = NULL;
            }
            int i;
            //Load initialized blank cards on board
            for(i = 0; i < 10; i++){
                memset(&board->cards_on_board[i],0,sizeof(card_t));
                //Mark the blank cards with -1
                board->cards_on_board[i].layout.cache_index = -1;
                board->cards_on_board[i].layout.texture = SDL_CreateTextureFromSurface(renderer,board->cache->blank_card);
            }
            printf("\t\tResources successfully loaded.\n\t[/LOAD]\n");
        }
        printf("\tBoard successfully initialized.\n[/INIT]\n");
        return 0;
    }
    printf("[ERROR] Board Initialization Failed\n");
    return -1;
}
