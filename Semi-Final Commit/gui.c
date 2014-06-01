#include "gui.h"
#define DEBUG printf("Debug - :)\n");

char* BASE_CARD_PATHS[] = {"Resources/cards/darkmistress_clear.bmp","Resources/cards/death_bringer_clear.bmp","Resources/cards/robochamp_clear.bmp","Resources/cards/illuminata_clear.bmp","Resources/cards/witch_clear.bmp","Resources/cards/gouache_clear.bmp","Resources/cards/demonicthor_clear.bmp"};
char* BASE_NUMERIC_PATHS[] = {"Resources/numeric/0.bmp","Resources/numeric/1.bmp","Resources/numeric/2.bmp","Resources/numeric/3.bmp","Resources/numeric/4.bmp"
    ,"Resources/numeric/5.bmp","Resources/numeric/6.bmp","Resources/numeric/7.bmp","Resources/numeric/8.bmp","Resources/numeric/9.bmp"};

int a = 255;
bool_t decreasing = false;
int local_count = 0;
int timer = 0;

void compose_scene(void* param){
    if(param != NULL){
        render_data_t data =*((render_data_t*)param);
        if(data.window && data.renderer != NULL){
            SDL_RenderCopy(data.renderer,data.board->background_texture,NULL,NULL);
            int i;
            for(i = 0; i < MAX_PLACED_CARDS; i++){
                SDL_RenderCopy(data.renderer,data.board->cards_on_board[i].layout.texture,NULL,&data.board->cards_on_board[i].layout.rectangle);
            }
            //Drawing manacrystals
            SDL_Rect temp_rect = {0,0,0,0};
            temp_rect.w = 50;
            temp_rect.h = data.board->display_mode.h / 2 / 10;
            temp_rect.x = data.board->display_mode.w - temp_rect.w;
            temp_rect.y = data.board ->display_mode.h - temp_rect.h;
            SDL_SetTextureColorMod(data.board->cache->mana_crystal,0,0,255);
            SDL_SetTextureAlphaMod(data.board->cache->mana_crystal,255 / 2);
            for(i = 0; i < data.board->players[data.board->player_id].pool_of_mana.mana_crystals; i++, temp_rect.y -= temp_rect.h / 2 + 10){
                SDL_RenderCopy(data.renderer,data.board->cache->mana_crystal,NULL,&temp_rect);
            }
            //Drawing health crystal
            temp_rect.w = 50;
            temp_rect.h = 50;
            temp_rect.x = 25;
            temp_rect.y = data.board->display_mode.h - temp_rect.h;
            SDL_SetTextureColorMod(data.board->players[data.board->player_id].hp_texture.texture,255,0,0);
            SDL_SetTextureAlphaMod(data.board->players[data.board->player_id].hp_texture.texture,255 / 3);
            SDL_RenderCopy(data.renderer,data.board->players[data.board->player_id].hp_texture.texture,NULL,&temp_rect);
            //Drawing hand cards
            for(i = 0; i < data.board->players[data.board->player_id].hand.size; i++){
                SDL_RenderCopy(data.renderer,data.board->players[data.board->player_id].hand.cards[i].layout.texture,NULL,&data.board->players[data.board->player_id].hand.cards[i].layout.rectangle);
            }
        }
        //Cursor texture must be copied to the render last because it's on top of all visual "layers"
        SDL_RenderCopy(data.renderer,data.board->cursor.texture,NULL,&data.board->cursor.rectangle);
    }
}

int render_scene(void* param){
    if(param != NULL){
        render_data_t data = *((render_data_t*) param);
        if((data.window != NULL) & (data.renderer != NULL) & (data.board != NULL)){
            SDL_SetRenderDrawColor(data.renderer,data.clear_color->r,data.clear_color->g,data.clear_color->b,data.clear_color->a);
            SDL_RenderClear(data.renderer);
            compose_scene(&data);
            SDL_RenderPresent(data.renderer);
            return 0;
        }
    }
    return -1;
}

Uint32 main_timer_callback(Uint32 interval,void* param){
    if(param != NULL){
        render_data_t data = *((render_data_t*)param);
        if((data.window != NULL )&& (data.renderer != NULL) && *data.running == true){
            //Check if the game is still running before doing anything
            if(data.event->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                *data.running = false;
            }
            SDL_GetMouseState(&data.board->cursor.rectangle.x,&data.board->cursor.rectangle.y);
            //Handle input from current user or AI
            handle_player_input(param);
            if(data.event->type == SDL_MOUSEBUTTONDOWN | data.event->type == SDL_MOUSEMOTION){
                if(data.event->button.button == SDL_BUTTON_LEFT){
                    SDL_SetTextureColorMod(data.board->cursor.texture,128,0,0);
                    SDL_SetTextureAlphaMod(data.board->cursor.texture,255);
                }
                else if(data.event->button.button == SDL_BUTTON_RIGHT){
                    SDL_SetTextureColorMod(data.board->cursor.texture,0,0,128);
                    SDL_SetTextureAlphaMod(data.board->cursor.texture,255);
                }
            }
            else{
                SDL_SetTextureColorMod(data.board->cursor.texture,156,156,156);
                SDL_SetTextureAlphaMod(data.board->cursor.texture,128);
            }
            render_scene(param);
            return interval;
        }
    }
    return 0;
}

//This method gets called at game start ONLY !
int load_game_resources(SDL_Renderer* renderer, board_t* board){
    if(renderer){
        printf("[LOAD]\n\tLoading resources...\n");
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
            //Fill decks with cards
            int i;
            for(i = 0; i < DECK_SIZE / 2; i++){
                /*! DONT MESS WITH THIS CODE */
                card_t card;
                memset(&card,0,sizeof(card_t));
                card.layout.dragged = false;
                card.layout.focused = false;
                calculate_vital_ratios(&card);
                design_card(renderer,&card,board->cache,rand() % 6);
                board->players[0].hand.cards[i] = card;
                board->players[1].hand.cards[i] = card;
                board->players[0].hand.size = i + 1;
                board->players[1].hand.size = i + 1;
            }
            //Load initialized blank cards on board
            for(i = 0; i < MAX_PLACED_CARDS; i++){
                memset(&board->cards_on_board[i],0,sizeof(card_t));
                //Mark the blank cards with -1
                board->cards_on_board[i].layout.cache_index = -1;
                board->cards_on_board[i].layout.texture = SDL_CreateTextureFromSurface(renderer,board->cache->blank_card);
            }
            printf("\tResources successfully loaded.\n[/LOAD]\n");
        }
        return 0;
    }
    printf("\tFailed to load resources.\n[x/LOAD]\n");
    return -1;
}

int automated_gameplay_system(board_t *board){
    return 0;
}

int query_display_mode(SDL_DisplayMode *display_mode){
    SDL_DisplayMode current;
    int i;
    for(i = 0; i < SDL_GetNumVideoDisplays(); i++){
        int result = SDL_GetCurrentDisplayMode(i,&current);
        if(result == 0){
            *display_mode = current;
            return 0;
        }
    }
    return -1;
}

int flip_coin(){
    srand(time(NULL));
    return (rand() % 2);
}

int handle_player_input(void* param){
    if(param){
        render_data_t data = *((render_data_t*)param);
        int i;
        SDL_Rect mouse_rect = {data.board->cursor.rectangle.x,data.board->cursor.rectangle.y,1,1};
        for(i = 0; i < sizeof(data.board->players[data.board->player_id].hand.cards)/sizeof(card_t); i++){
            //If mouse intersects with card
            if(SDL_HasIntersection(&mouse_rect,&data.board->players[data.board->player_id].hand.cards[i].layout.rectangle)){
                data.board->players[data.board->player_id].hand.cards[i].layout.focused = true;
                //If there was a left mouse click on card, begin drag
                if(data.event->button.state == SDL_PRESSED){
                    data.board->players[data.board->player_id].hand.cards[i].layout.dragged = true;
                    //Set drag offset
                    data.board->cursor.offset_x = data.board->cursor.rectangle.x - data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x;
                    data.board->cursor.offset_y = data.board->cursor.rectangle.y - data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y;
                    //Get initial coordinates
                    data.board->cursor.old_x = data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x;
                    data.board->cursor.old_y = data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y;
                    data.board->players[data.board->player_id].hand.cards[i].layout.focused = false;
                }
                if(data.event->type == SDL_MOUSEBUTTONUP){
                    //Check if the card is being placed on the board
                    int b;
                    for(b = 5; b < 10; b++){
                        if(SDL_HasIntersection(&mouse_rect,&data.board->cards_on_board[b].layout.rectangle) && can_put_card(data.board->players[data.board->player_id].hand.cards[i],data.board->players[data.board->player_id].pool_of_mana)){
                            //Redesign the card to unlink the original texture from the target one (i.e. hand card texture, board card texture)
                            card_t* temp_card = play_card_from_hand(&data.board->players[data.board->player_id],i);
                            data.board->cards_on_board[b].attack_points = temp_card->attack_points;
                            data.board->cards_on_board[b].health_points = temp_card->health_points;
                            data.board->cards_on_board[b].mana_cost = temp_card->mana_cost;
                            data.board->cards_on_board[b].layout.cache_index = temp_card->layout.cache_index;
                            data.board->cards_on_board[b].layout.texture = temp_card->layout.texture;
                            memset(temp_card,0,sizeof(card_t));
                            temp_card->layout.texture = NULL;
                        }
                    }

                    data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x = data.board->cursor.old_x;
                    data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y = data.board->cursor.old_y;
                    data.board->players[data.board->player_id].hand.cards[i].layout.dragged = false;
                }
            }
            else{
                SDL_SetTextureBlendMode(data.board->players[data.board->player_id].hand.cards[i].layout.texture,SDL_BLENDMODE_NONE);
                data.board->players[data.board->player_id].hand.cards[i].layout.focused = false;
            }
            //If card has focus
            if(data.board->players[data.board->player_id].hand.cards[i].layout.focused == true){
                data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y = data.board->display_mode.h - data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.h;
                SDL_SetTextureColorMod(data.board->players[data.board->player_id].hand.cards[i].layout.texture,255,255,255);
            }
            else{
                //If card lost focus
                SDL_SetTextureColorMod(data.board->players[data.board->player_id].hand.cards[i].layout.texture,128,128,128);
                //If the card was released outside the left bound of the screen
                if(data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x < 0){
                    data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x = 0;
                }
                //If the card was released outside the right bound of the screen
                else if(data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x > data.board->display_mode.w - data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.w){
                    data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x = data.board->display_mode.w - data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.w;
                }

                data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y = data.board->display_mode.h - (data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.h / 2);
            }

            if(data.board->players[data.board->player_id].hand.cards[i].layout.dragged == true){
                SDL_SetTextureBlendMode(data.board->players[data.board->player_id].hand.cards[i].layout.texture,SDL_BLENDMODE_ADD);
                data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.x = data.board->cursor.rectangle.x - data.board->cursor.offset_x;
                data.board->players[data.board->player_id].hand.cards[i].layout.rectangle.y = data.board->cursor.rectangle.y - data.board->cursor.offset_y;
            }
        }
    }
    return 0;
}

//Will remap card element surfaces based on new card values (like mana, health etc.)
//Instead of card_path we'll use (int card_index) to load surfaces from the surface cache
int design_card(SDL_Renderer* renderer,card_t *card,surface_cache_t* cache,int card_index){
    SDL_Surface* load_surface = NULL;
    SDL_Surface* temp_surface = NULL;
    SDL_Rect rect = {0,0,0,0};

    //Load card if valid index
    if((card_index >= 0) && (card_index < (sizeof(cache->cards)/sizeof(SDL_Surface*)))){
        //We make a copy of the original cached surface, so the original remains unchanged !!!
        load_surface = SDL_ConvertSurface(cache->cards[card_index],cache->cards[card_index]->format,cache->cards[card_index]->flags);
    }
    else{
        return -1;
    }
    //Map attack digit surface
    if(load_surface){
        temp_surface = SDL_ConvertSurface(cache->digits[card->attack_points],cache->digits[card->attack_points]->format,cache->digits[card->attack_points]->flags);
        if(temp_surface){
            SDL_SetSurfaceBlendMode(temp_surface,SDL_BLENDMODE_ADD);
            rect.x = temp_surface->clip_rect.x + 20;
            rect.y = load_surface->h - (temp_surface->clip_rect.h + 15);
            rect.w = 25;
            rect.h = 25;
            SDL_BlitSurface(temp_surface,NULL,load_surface,&rect);
            SDL_FreeSurface(temp_surface);
            temp_surface = NULL;
            rect.x = 0;
            rect.y = 0;
            rect.w = 0;
            rect.h = 0;
        }
        //Map health digit surface
        temp_surface = SDL_ConvertSurface(cache->digits[card->health_points],cache->digits[card->health_points]->format,cache->digits[card->health_points]->flags);
        if(temp_surface){
            SDL_SetSurfaceBlendMode(temp_surface,SDL_BLENDMODE_ADD);
            rect.x = load_surface->clip_rect.w - temp_surface->w - 20;
            rect.y = load_surface->h - (temp_surface->clip_rect.h + 15);
            rect.w = 25;
            rect.h = 25;
            SDL_BlitSurface(temp_surface,NULL,load_surface,&rect);
            SDL_FreeSurface(temp_surface);
            temp_surface = NULL;
            rect.x = 0;
            rect.y = 0;
            rect.w = 0;
            rect.h = 0;
        }

        /*Map manacost digit surface*/
        temp_surface = SDL_ConvertSurface(cache->digits[card->mana_cost],cache->digits[card->mana_cost]->format,cache->digits[card->mana_cost]->flags);
        if(temp_surface){
            SDL_SetSurfaceBlendMode(temp_surface,SDL_BLENDMODE_ADD);
            rect.w = 25 / 2;
            rect.h = 30 / 2;
            rect.x = load_surface->clip_rect.w / 2 + (temp_surface->clip_rect.w / 2 - 17);
            rect.y = load_surface->clip_rect.h - (temp_surface->clip_rect.h / 2 + 15);
            SDL_BlitScaled(temp_surface,NULL,load_surface,&rect);
            SDL_FreeSurface(temp_surface);
            temp_surface = NULL;
        }
        //Replace current texture, with the new one
        /*Destroy previously stored texture to prevent memory leak*/
        if(card->layout.texture){
            SDL_DestroyTexture(card->layout.texture);
        }
        /*Create Texture*/
        card->layout.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
        card->layout.cache_index = card_index;
        temp_surface = NULL;
        SDL_FreeSurface(load_surface);
        return 0;
    }
    return -1;
}

surface_cache_t* create_preloaded_cache(SDL_Renderer* renderer){
    surface_cache_t* cache = (surface_cache_t*)malloc(sizeof(surface_cache_t));
    if(cache){
        SDL_Surface* load_surface = NULL;
        //Load background surface
        load_surface = SDL_LoadBMP("Resources/bgr_1.bmp");
        if(load_surface){
            cache->background = load_surface;
            load_surface = NULL;
        }
        //Load blank card surface (initial surface for cards_on_board)
        load_surface = SDL_LoadBMP("Resources/cards/blank.bmp");
        if(load_surface){
            cache->blank_card = load_surface;
            load_surface = NULL;
        }
        //Load all available card surfaces
        int i;
        for(i = 0; i < (sizeof(BASE_CARD_PATHS)/sizeof(char*)); i++){
            load_surface = SDL_LoadBMP(BASE_CARD_PATHS[i]);
            if(load_surface){
                cache->cards[i] = load_surface;
                load_surface = NULL;
            }
        }
        //Load digits surfaces
        for(i = 0; i < 10; i++){
            load_surface = SDL_LoadBMP(BASE_NUMERIC_PATHS[i]);
            if(load_surface){
                cache->digits[i] = load_surface;
                load_surface = NULL;
            }
        }
        //Load cursor surface
        load_surface = SDL_LoadBMP("Resources/cursor_cyber_precise_2.bmp");
        if(load_surface){
            SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,0,0,0));
            cache->cursor = load_surface;
            load_surface = NULL;
        }
        //Load manacrystal texture
        load_surface = SDL_LoadBMP("Resources/field/full_crystal_right.bmp");
        if(load_surface){
            SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,255,255,255));
            cache->mana_crystal = SDL_CreateTextureFromSurface(renderer,load_surface);
            SDL_FreeSurface(load_surface);
            load_surface = NULL;
        }
        //Load health crystal texture
        load_surface = SDL_LoadBMP("Resources/field/hp_crystal_full.bmp");
        if(load_surface){
            SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,255,255,255));
            cache->health_crystal = SDL_CreateTextureFromSurface(renderer,load_surface);
            SDL_FreeSurface(load_surface);
            load_surface = NULL;
        }
        return cache;
    }
    return NULL;
}

int light_index = 0;
Uint32 animator_timer_callback(Uint32 interval, void* param){
    if(param){
        render_data_t data = *((render_data_t*)param);
        if(decreasing == false){
            if(a < 255){
                a+=5;
            }
            else{
                decreasing = true;
            }
        }
        else{
            if(a > 200){
                a-=5;
            }
            else{
                decreasing = false;
            }
        }
        if(light_index < MAX_PLACED_CARDS - 1){
            light_index++;
        }
        else{
            light_index = 5;
        }
        //SDL_SetTextureColorMod(data.board->background_texture,a,255 - a,0);
        if(data.board->player_id == 0){
            SDL_SetTextureColorMod(data.board->cards_on_board[MAX_PLACED_CARDS - 1 - light_index].layout.texture,a,0,a);
            SDL_SetTextureColorMod(data.board->cards_on_board[light_index].layout.texture,0,a,a);
            SDL_SetTextureColorMod(data.board->background_texture,a / 2,0, a / 2);
        }
        else{
            SDL_SetTextureColorMod(data.board->cards_on_board[MAX_PLACED_CARDS - 1 - light_index].layout.texture,0,a,a);
            SDL_SetTextureColorMod(data.board->cards_on_board[light_index].layout.texture,a,0,a);
            SDL_SetTextureColorMod(data.board->background_texture,0,a / 2,a / 2);
        }
        return interval;
    }
    return 0;
}

int position_board_elements(SDL_Renderer* renderer, board_t* board){
    if((renderer && board)){
        int width = board->display_mode.w / 2 / board->players[board->player_id].hand.size;
        int x = (board->display_mode.w / 2) - (board->players[board->player_id].hand.size * width / 2);
        int i;
        for(i = 0; i < board->players[board->player_id].hand.size; i++){
            board->players[board->player_id].hand.cards[i].layout.rectangle.w = width;
            board->players[board->player_id].hand.cards[i].layout.rectangle.h = board->display_mode.h / 4;
            board->players[board->player_id].hand.cards[i].layout.rectangle.x = x + (width * i);
            board->players[board->player_id].hand.cards[i].layout.rectangle.y = board->display_mode.h - (board->display_mode.h / 10);
        }
        x = board->display_mode.w / 4;
        int y = board->display_mode.h / 4;
        width = board->display_mode.w / 2 / 5;//5 cards in a row

        //Variable j will assure the proper distribution of the 10 cards
        int j;
        for(i = 0, j = 0; i < 10; i++, j++){
            if(j == 5){
                j = 0;
                y *= 2;
            }
            //Here we need to redesign all cards on the board to update their vital state
            if(board->cards_on_board[i].layout.cache_index != -1){
                design_card(renderer,&board->cards_on_board[i],board->cache,board->cards_on_board[i].layout.cache_index);
            }
            else{
                //If the texture was destroyed i.e. the card lost a fight
                board->cards_on_board[i].layout.texture = SDL_CreateTextureFromSurface(renderer,board->cache->blank_card);
            }

            board->cards_on_board[i].layout.rectangle.x = x + (width * j);
            board->cards_on_board[i].layout.rectangle.y = y;
            board->cards_on_board[i].layout.rectangle.w = width;
            board->cards_on_board[i].layout.rectangle.h = board->display_mode.h / 4;
            SDL_SetTextureBlendMode(board->cards_on_board[i].layout.texture,SDL_BLENDMODE_ADD);
        }
        return 0;
    }
    return -1;
}

int rotate_board(board_t *board){
    card_t temp_card;
    memset(&temp_card,0,sizeof(card_t));
    int i = 0;
    for(i = 0; i < 5; i++){
        temp_card = board->cards_on_board[i];
        board->cards_on_board[i] = board->cards_on_board[i + 5];
        board->cards_on_board[i + 5] = temp_card;
    }
return 0;
}

int end_turn(board_t *board){
    int i = 0;
    int result;
    for(i = 0; i < 5; i++){
        if(board->player_id == 0){
            result = attack(&board->players[1].health_points,&board->players[0].health_points,&board->cards_on_board[i + 5],&board->cards_on_board[i]);
        }
        else{
            result = attack(&board->players[0].health_points,&board->players[1].health_points,&board->cards_on_board[i + 5],&board->cards_on_board[i]);
        }
        switch(result){
            case 0:
                SDL_DestroyTexture(board->cards_on_board[i].layout.texture);
                memset(&board->cards_on_board[i],0,sizeof(card_t));
                board->cards_on_board[i].layout.cache_index = -1;
                SDL_DestroyTexture(board->cards_on_board[i + 5].layout.texture);
                board->cards_on_board[i + 5].layout.texture = NULL;
                memset(&board->cards_on_board[i + 5],0,sizeof(card_t));
                board->cards_on_board[i + 5].layout.cache_index = -1;
            break;

            case 1:
                SDL_DestroyTexture(board->cards_on_board[i].layout.texture);
                memset(&board->cards_on_board[i],0,sizeof(card_t));
                board->cards_on_board[i].layout.cache_index = -1;
            break;

            case 2:
                SDL_DestroyTexture(board->cards_on_board[i + 5].layout.texture);
                memset(&board->cards_on_board[i + 5],0,sizeof(card_t));
                board->cards_on_board[i + 5].layout.cache_index = -1;
            break;
        }
    }
    //Check for winning player
return 0;
}

Uint32 turn_callback(Uint32 interval,void* param){
    if(param){
        render_data_t data = *((render_data_t*)param);
        if(data.board){
            end_turn(data.board);
            design_health(data.renderer,data.board);
            if(data.board->player_id == 0){
                data.board->player_id = 1;
                local_count++;
            }
            else{
                data.board->player_id = 0;
                local_count++;
            }
            if(local_count == 2){
                data.board->turn_count++;
                local_count = 0;
            }
            rotate_board(data.board);
            position_board_elements(data.renderer,data.board);
            light_index = 0;
            turn_begin(&data.board->players[data.board->player_id],data.board->turn_count);

        }
        return interval;
    }
    return 0;
}

SDL_Surface* two_digit_number(surface_cache_t* cache,int number){
    SDL_Surface* load_surface = SDL_CreateRGBSurface(0,25 * 2,25,32,0,0,0,0);
    if(load_surface){
        SDL_Rect rect = {0,0,25,25};
        if(number / 10 > -1 && number / 10 < 10){
            SDL_BlitScaled(cache->digits[number / 10],NULL,load_surface,&rect);
        }

        rect.x = 25;
        number %=10;

        if(number > -1 && number < 10){
            SDL_BlitScaled(cache->digits[number],NULL,load_surface,&rect);
        }
        SDL_SetColorKey(load_surface,SDL_TRUE,SDL_MapRGB(load_surface->format,0,0,0));
        return load_surface;
    }
    return (SDL_Surface*)NULL;
}

void calculate_vital_ratios(card_t* card){
    do{
        card->attack_points = rand() % 3 + 1;
        card->health_points = rand() % 3 + 1;
        card->mana_cost = rand() % 4 + 1;
    }while((card->attack_points / card->health_points > 1) && (card->mana_cost / 2 > (card->attack_points + card->health_points)));
    return;
}

void design_health(SDL_Renderer* renderer, board_t* board){
    SDL_Surface* load_surface = two_digit_number(board->cache,board->players[board->player_id].health_points);
    if(load_surface){
        SDL_DestroyTexture(board->players[0].hp_texture.texture);
        SDL_DestroyTexture(board->players[1].hp_texture.texture);
        board->players[0].hp_texture.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
        board->players[1].hp_texture.texture = SDL_CreateTextureFromSurface(renderer,load_surface);
        SDL_FreeSurface(load_surface);
    }
}
