#include "player.h"
#define INDEX_OUT_OF_RANGE "card_index_out_of_hand_range\0"
#define INVALID_INDEX "invalid_card_index
int change_hp(player_t *player,int hp){
    player->health_points = hp;
    return(player->health_points > 0 ? 0 : 1);
}

card_t* play_card_from_hand(player_t *player,int card_index){
    card_t card;
    if(card_index > 0 && card_index < sizeof(player->hand)/sizeof(card_t)){
        &card = &player->hand[card_index];
        if(&card == 0x000){
            card.name = INDEX_OUT_OF_RANGE;
            return &card;
        }
        else{
            card.name = INVALID_INDEX;
            return &card;
        }
    }
}

int place_card_in_hand(player_t *player, card_t *card){
    &player->hand[player->hand.size++] = card;
    return 0;
}
