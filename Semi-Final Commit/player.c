#include "player.h"

int init_player(player_t* player){
    player->hand.size = 0;
    player->deck_of_cards.size = 0;
    player->health_points = 30;
}

int change_hp(player_t *player,int hp){
    player->health_points = hp;
    return(player->health_points <= 0 ? 0 : 1);
}

card_t* play_card_from_hand(player_t *player,int card_index){
    if(card_index > -1 && card_index < player->hand.size){
        player->pool_of_mana.mana_crystals -= player->hand.cards[card_index].mana_cost;
        return (&player->hand.cards[card_index]);
    }
    return NULL;
}

void turn_begin(player_t* player,int turn_count){
    if(player->deck_of_cards.size > 0){
        draw_card(&player->deck_of_cards,&player->hand.cards[player->hand.size++]);
    }
    if(turn_count < 10){
        player->pool_of_mana.mana_crystals = turn_count;
    }
    else{
        player->pool_of_mana.mana_crystals = 10;
    }

}
