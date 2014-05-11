#include "gui.h"
#include <string.h> //string.h to use memset,memcpy and so on !
#include <math.h>
#define INITIAL_CARD_NAME "empty\0"
int counter = 0;
int r = 0;
int r2 = 0;
int g = 0;
int b = 0;

void compose_scene(SDL_Renderer* renderer,board_graph* board){
    if(!(renderer == NULL)){
        SDL_RenderCopy(renderer,board->background_texture,NULL,NULL);
        //SDL_RenderCopy(renderer,board->first_player.hand.cards[0].layout.texture,NULL,&board->first_player.hand.cards[0].layout.rectangle);

        int i;
        for(i = 0; i < 2; i++){
            SDL_RenderCopy(renderer,board->first_player.hand.cards[i].layout.texture,NULL,&board->first_player.hand.cards[i].layout.rectangle);
        }
        /*
        for(i = 0; i < sizeof(board->cards_on_board)/sizeof(card_t); i++){
            SDL_RenderCopy(renderer,&board->cards_on_board[i].layout.texture,NULL,&board->cards_on_board[i].layout.rectangle);
        }
        //for(i = 0; i < sizeof(board->first_player.hand)/sizeof(hand_t); i++){
            //SDL_RenderCopy(renderer,&board->first_player.hand.cards[0].layout.texture,NULL,&board->first_player.hand.cards[0].layout.rectangle);
        //}
        for(i = 0; i < sizeof(board->second_player.hand)/sizeof(hand_t) ; i++){
            SDL_RenderCopy(renderer,&board->second_player.hand.cards[i].layout.texture,NULL,&board->second_player.hand.cards[i].layout.rectangle);
        }
        */
        SDL_RenderCopy(renderer,board->cursor.texture,NULL,&board->cursor.rectangle);
    }
}

int render_scene(SDL_Window *window,SDL_Renderer *renderer,SDL_Color *clear_color,board_graph* board){
    //First we check if our window and renderer still exist
    if(!(window == NULL && renderer == NULL) && !(clear_color == NULL))
    {
        //For FPS count
        //Uint32 render_time = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer,clear_color->r,clear_color->g,clear_color->b,clear_color->a);
        SDL_RenderClear(renderer);
        compose_scene(renderer,board);
        SDL_RenderPresent(renderer);
        //FPS Printing
        //printf("Render time: %u \n",(SDL_GetTicks() - render_time));
        return 0;
    }
    else
    {
        return -1;
    }
}

Uint32 my_callback(Uint32 interval,void* param){
    Uint32 current_time = SDL_GetTicks();
    timer_params t_p = *((timer_params*)param);
    if((t_p.window != NULL )&& (t_p.renderer != NULL)){

        if(t_p.event->type == SDL_MOUSEBUTTONDOWN | t_p.event->type == SDL_MOUSEMOTION){
            if(t_p.event->button.button == SDL_BUTTON_LEFT){
                //printf("%d %d\n",t_p.board->cursor.rectangle.x,t_p.board->cursor.rectangle.y);
                SDL_SetTextureColorMod(t_p.board->cursor.texture,128,0,0);
                SDL_SetTextureAlphaMod(t_p.board->cursor.texture,255);
            }
            else if(t_p.event->button.button == SDL_BUTTON_RIGHT){
                SDL_SetTextureColorMod(t_p.board->cursor.texture,0,0,128);
                SDL_SetTextureAlphaMod(t_p.board->cursor.texture,255);
            }
        }
        else if(t_p.event->type == SDL_MOUSEBUTTONUP && (t_p.event->button.button == SDL_BUTTON_LEFT | t_p.event->button.button == SDL_BUTTON_RIGHT)){
            SDL_SetTextureColorMod(t_p.board->cursor.texture,0,0,0);
            SDL_SetTextureAlphaMod(t_p.board->cursor.texture,200);
        }

        if(r < 200){
            r++;
            g++;
            b++;
            SDL_SetTextureColorMod(t_p.board->cursor.texture,200 - r,200 - g, 200 - b);
        }
        SDL_SetTextureColorMod(t_p.board->background_texture,r,g,b);
        render_scene(t_p.window,t_p.renderer,t_p.clear_color,t_p.board);
        printf("Main timer callback returned for %d time",++counter);
        return interval;
    }
    return 0;

}

int init_board(board_graph *board){
    printf("[INIT]\n\t(Status report): Began at [%u] ticks\n",SDL_GetTicks());
    bool_t error_flag = false;

    memset(&board->first_player,0,sizeof(player_t));
    memset(&board->first_player,0,sizeof(player_t));
    printf("\t(Status report): Both players INITIALIZED\n");

    memset(&board->cards_on_board,0,sizeof(card_graph));
    memset(&board->sidebar,0,sizeof(graph_object));
    board->background_texture = NULL;
    printf("\t(Status report): Board INITIALIZED\n");

    printf("\t(Status report): Ended at [%u] ticks\n[/INIT]\n",SDL_GetTicks());

    if(!(error_flag == true)){
        return 0;
    }
    else{
        return -1;
    }
}

//This method gets called at game start ONLY !
int build_board(board_graph *board,SDL_Renderer *renderer){
    if((board != NULL) && (renderer != NULL)){
        init_board(board);
        SDL_Surface* load_surface = NULL;

        load_surface = surface_from_path(renderer,"Resources/board.bmp");
        //SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,0,0,0));
        //Map background elements to background surface before we convert it to texture
        SDL_SetSurfaceAlphaMod(load_surface,255);
        board->background_texture = SDL_CreateTextureFromSurface(renderer,load_surface);
        SDL_FreeSurface(load_surface);
        load_surface = NULL;

        load_surface = surface_from_path(renderer,"Resources/cards/cthulu_clear.bmp");
        SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,25,25,25));
        board->cards_on_board[0].layout.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
        SDL_FreeSurface(load_surface);
        load_surface = NULL;

    return 0;
    }
}

SDL_Surface* surface_from_path(SDL_Renderer *renderer, char* filepath){
    SDL_Surface *load_surface = NULL;
    load_surface = SDL_LoadBMP(filepath);
    if(load_surface != NULL){
        return load_surface;
    }
    SDL_FreeSurface(load_surface);
    return NULL;
}

void random_solid_color(SDL_Color *color){
    srand(time(NULL));
    color->r = rand() % 255;
    color->g = rand() % 255;
    color->b = rand() % 255;
    color->a = rand() % 255 / 2;
}
