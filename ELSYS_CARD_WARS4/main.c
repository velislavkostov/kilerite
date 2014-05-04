#include <stdio.h>
#include "player.h"

int main(){
    player_t player;
    player.hand.size = 0;
    player.health_points = 5;

    card_t card;

    printf("change_hp returned %d\n",change_hp(&player,player.health_points - 5));

    if(player.health_points <= 0){
        printf("player health dropped to %d\n",player.health_points);
    }
    else{
        printf("player health remains positive %d\n",player.health_points);
    }

    card.name = "monster\0";
    player.hand.cards[0] = card;

    card.name = "default\0";
    card = card_from_hand(&player.hand,0);

    printf("[Player actions] played card from hand with name: [%s]\n",card.name);
    return 0;
}
