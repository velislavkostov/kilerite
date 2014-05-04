#include "player.h"
#define INDEX_OUT_OF_RANGE "card_index_out_of_hand_range\0"
#define INVALID_INDEX "invalid_card_index
int change_hp(player_t *player,int hp){
    player->health_points = hp;
    return(player->health_points > 0 ? 0 : 1);
}

card_t card_from_hand(hand_t *hand,int card_index){
    return (hand->cards[card_index]);
}

void turn_begin(player_t *player){
    draw_card(&player->deck_of_cards,&player->hand.cards[player->hand.size++]);
}
