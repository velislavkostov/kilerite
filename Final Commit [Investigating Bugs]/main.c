#include "gui.h"

/*TODO: Structure gameplay system
        FIX overlapping cards bug
        PERFORM Mana check on card place try
        Make code more readable and clean,
        CODE AI and PERFORM OVERALL CODE OPTIMIZATION if there's enough time
*/

char* cursor_path = "Resources/cursor_cyber_precise_2.bmp\0";
/* FOR SDL */ //int main(int argc, char *argv[]);
int main(int argc, char *argv[])
{
    bool_t running = true;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    //SDL_Color clear_color = {25,0,25,255}; //Ubuntu like color ;d KEEP THAT M8
    SDL_Color clear_color = {0,0,0,255};
    SDL_Event event;

    //Get the current display mode
    SDL_DisplayMode current;
    query_display_mode(&current);
    printf("Display mode: W[%d] x H[%d]\n",current.w,current.h);

    SDL_CreateWindowAndRenderer(current.w,current.h,SDL_WINDOW_BORDERLESS,&window,&renderer);

    board_t board;
    srand(time(NULL));
    init_board(renderer,&board);

    render_data_t render_data;
    render_data.board = &board;
    render_data.event = &event;
    render_data.clear_color = &clear_color;
    render_data.renderer = renderer;
    render_data.window = window;
    render_data.running = &running;
    render_data.board->display_mode = current;
    board.player_id = flip_coin();
    printf("[Flip coin]\n\t Player %d is on turn\n[/Flip Coin]", board.player_id);

    board.cursor.rectangle.w = 25;
    board.cursor.rectangle.h = 25;

    board.players[board.player_id].pool_of_mana.mana_crystals = 1;
    turn_begin(&board.players[board.player_id]);
    design_health(renderer,&board);
    position_board_elements(renderer,&board);

    SDL_TimerID main_timer = SDL_AddTimer(1000 / 120,main_timer_callback,&render_data);
    SDL_TimerID animator_timer = SDL_AddTimer(100,animator_timer_callback,&render_data);
    SDL_TimerID turn_timer = SDL_AddTimer(1000 * 25,turn_callback,&render_data);
    /*turn timer will be called at approximately 25s interval,
    the callback will return dragged but not placed card back to hand and will change player_id value
    rendering part is flexible enough to handle the change without data races of any kind
    */

    SDL_ShowCursor(0);
    /*Main Loop*/
    while(running){
        //Check for events independently from the draw part
        if(SDL_WaitEvent(&event)){

        }
        else{
            *render_data.running = false;
        }
    }

    //CLEANING
    free(board.cache);
    SDL_RemoveTimer(main_timer);
    SDL_RemoveTimer(animator_timer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
