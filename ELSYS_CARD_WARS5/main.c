#include <stdio.h>
//#include "board.h"
#include "gui.h"

/*TODO LIST
    ! - Global variable: bool_t scene_changed so we can track down a change and re-render the scene depending on the type of change
    ! - When a player draws a card from deck to hand then every card should resize accordingly so the hand can fit in the screen
    PSEUDO: if (card_in_hand++) then for each card (card.w = BASE_card.w / number_of_cards_in_hand)
    ! - Arrange things according to SDL_WaitEvent,because we'll use it to spare CPU !!!
    {i.e. if(user_interaction) do_something;
    ! - Implement graphical path type so we can draw only the things that are in the graphical path every frame,
    if we don't want to see something on the screen - remove it from the graphical_path_t
    ! - Implement custom animation event,so we can bypass the SDL_WaitEvent with it as argument
    (example: if(moving) raise ANIMATION_EVENT)
    We can even implement a function that takes the desired SDL_Rect to be moved,SDL_Rect with only x and y values used as
    destination position,and interval of the animation and perform movement behind the curtains !
    ===============================================================================================

    1.To implement more optimized collision detection we'll divide the screen into separate "rectangles of interest"
    For example if mouse enters rectangle of interest of player one (a SDL_Rect that overlaps with the player's visual hand)
    then we check for collision between mouse and a card from that player's hand (:
    We do this to reduce the number of calls to the function that detects such collision (for example focus_on_click)

    2.To reduce the pixel depth of the cards so we get shorter render times !!! Remember to sync them with the display pixel format !
    3.Implement better collision detection between MOUSE & CARD
*/
char* card_names[] = {"Resources/cards/cthulu_clear.bmp","Resources/cards/dragon_kreator.bmp","Resources/cards/vicious_demon.bmp"};
char* cursor_path = "Resources/cursor_cyber_precise_2.bmp\0";
Uint32 mouse_accelaration = 0;
int myval = 0;
/* FOR SDL */ //int main(int argc, char *argv[]);
int main(int argc, char *argv[])
{
    bool_t running = true;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    //SDL_Color clear_color = {25,0,25,255}; //Ubuntu like color ;d KEEP THAT M8
    SDL_Color clear_color = {0,0,0,255};
    SDL_Event event;

    //Get the current display mode
    SDL_DisplayMode current;

    int i;
    for(i = 0;i < SDL_GetNumVideoDisplays();i++){
        int var = SDL_GetCurrentDisplayMode(i,&current);
        if(var == 0) printf("Display mode found at index %d\n",i);
    }

    /* !!!Scaling graphics!!!*/
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");
    //SDL_RenderSetLogicalSize(renderer,current.w,current.h);
    printf("W[%d] H[%d]\n",current.w,current.h);

    SDL_CreateWindowAndRenderer(current.w,current.h,SDL_WINDOW_BORDERLESS | SDL_WINDOW_MOUSE_FOCUS,&window,&renderer);



    board_graph board;
    build_board(&board,renderer);

    SDL_Surface* load_surface = NULL;

    load_surface = surface_from_path(renderer,"Resources/cards/cthulu_clear.bmp");
    SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,25,25,25));
    board.first_player.hand.cards[0].layout.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
    board.first_player.hand.cards[1].layout.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
    SDL_FreeSurface(load_surface);
    load_surface = NULL;
    board.first_player.hand.cards[1].layout.rectangle.x = current.w / 4 + 20;
    board.first_player.hand.cards[1].layout.rectangle.y = current.h - current.h / 4 - 10;
    board.first_player.hand.cards[1].layout.rectangle.w = current.w / 2 / 5;
    board.first_player.hand.cards[1].layout.rectangle.h = current.h / 4 - 40;

    SDL_SetTextureAlphaMod(board.first_player.hand.cards[1].layout.texture,200);
    SDL_SetTextureColorMod(board.first_player.hand.cards[1].layout.texture,200,200,200);
    SDL_SetTextureBlendMode(board.first_player.hand.cards[1].layout.texture,SDL_BLENDMODE_MOD);

    board.first_player.hand.cards[0].layout.rectangle = board.first_player.hand.cards[1].layout.rectangle;
    board.first_player.hand.cards[0].layout.rectangle.x = current.w / 4 + 200;
    board.first_player.hand.cards[0].layout.rectangle.w = current.w / 2 / 5;

    board.first_player.hand.cards[0].layout.focused = false;
    board.first_player.hand.cards[0].layout.dragged = false;

    //board.background_texture = board.first_player.hand.cards[0].layout.texture;
    load_surface = surface_from_path(renderer,cursor_path);
    SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,0,0,0));
    board.cursor.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
    SDL_FreeSurface(load_surface);
    load_surface = NULL;

    timer_params t_p;
    t_p.board = &board;
    t_p.event = &event;
    t_p.clear_color = &clear_color;
    t_p.renderer = renderer;
    t_p.window = window;

    SDL_TimerID timer = SDL_AddTimer(1000 / 60,my_callback,&t_p);

    int offset_x = 0;
    int offset_y = 0;

    SDL_ShowCursor(0);
    /*Main Loop*/
    while(!running){
        //Check for events independently from the draw part
        if(SDL_WaitEvent(&event)){
            //If the Escape key was pressed, we exit the main loop
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE | event.type == SDL_QUIT){
                running = false;
            }
        }

        SDL_GetMouseState(&board.cursor.rectangle.x,&board.cursor.rectangle.y);
        board.cursor.rectangle.h = current.h / 10 / 2; //2
        board.cursor.rectangle.w = current.w / 10 / 3;

        /*
        if(event.button.clicks == SDL_BUTTON_LEFT){
            SDL_SetTextureAlphaMod(board.cursor.texture,255 / 2);
        }
        else{
            SDL_SetTextureAlphaMod(board.cursor.texture,255);
        }
        */

        if(SDL_HasIntersection(&board.cursor.rectangle,&board.first_player.hand.cards[0].layout.rectangle)){
            board.first_player.hand.cards[0].layout.focused = true;
            if(event.button.clicks == SDL_BUTTON_LEFT){
                board.first_player.hand.cards[0].layout.dragged = !board.first_player.hand.cards[0].layout.dragged;
                if(board.first_player.hand.cards[0].layout.dragged == true){
                    offset_x = board.cursor.rectangle.x - board.first_player.hand.cards[0].layout.rectangle.x;
                    offset_y = board.cursor.rectangle.y - board.first_player.hand.cards[0].layout.rectangle.y;
                    SDL_SetTextureAlphaMod(board.first_player.hand.cards[0].layout.texture,255 / 2);
                }
                else{
                    SDL_SetTextureAlphaMod(board.first_player.hand.cards[0].layout.texture,225);
                }
                board.first_player.hand.cards[0].layout.focused = !board.first_player.hand.cards[0].layout.focused;
            }
        }
        else{
            board.first_player.hand.cards[0].layout.focused = false;
        }
        if(board.first_player.hand.cards[0].layout.focused == true){
            int i = 10;
            //for(i = 1 ; i <= 10 ; i++){
                //board.first_player.hand.cards[0].rectangle.y = current.h - board.first_player.hand.cards[0].rectangle.h;
                board.first_player.hand.cards[0].layout.rectangle.y = (current.h - board.first_player.hand.cards[0].layout.rectangle.h / 2) - (board.first_player.hand.cards[0].layout.rectangle.h / 2 / 10) * i;
                //SDL_Delay(25);
                SDL_SetTextureColorMod(board.first_player.hand.cards[0].layout.texture,255,255,255);
            //}
        }
        else{
            SDL_SetTextureColorMod(board.first_player.hand.cards[0].layout.texture,100,100,100);
            if(board.first_player.hand.cards[0].layout.rectangle.x < 0){
                board.first_player.hand.cards[0].layout.rectangle.x = 0;
            }
            else if(board.first_player.hand.cards[0].layout.rectangle.x > current.w - board.first_player.hand.cards[0].layout.rectangle.w){
                board.first_player.hand.cards[0].layout.rectangle.x = current.w - board.first_player.hand.cards[0].layout.rectangle.w;
            }
            board.first_player.hand.cards[0].layout.rectangle.y = current.h - (board.first_player.hand.cards[0].layout.rectangle.h / 2);

        }

        if(board.first_player.hand.cards[0].layout.dragged == true){
            board.first_player.hand.cards[0].layout.rectangle.x = board.cursor.rectangle.x - offset_x;
            board.first_player.hand.cards[0].layout.rectangle.y = board.cursor.rectangle.y - offset_y;
        }
        random_solid_color(&clear_color);
        //render_scene(window,renderer,&clear_color,&board);
        SDL_Delay(1000 / 60);
    }

    //CLEANING
    SDL_RemoveTimer(timer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
