#include "player.h"

int change_hp(player_t *player,int hp){
    player->health_points = hp;
    return(player->health_points <= 0 ? 0 : 1);
}

int place_card_in_hand(card_t *card, hand_t *hand){
    if(card != NULL && hand != NULL){
        hand->cards[hand->size++] = *card;
        return 0;
    }
    return -1;
}
