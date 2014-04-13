#include <stdio.h>
#include "card.h"

int main()
{
    card_t first_card;
    card_t second_card;

    first_card.attack_points = 2;
    first_card.health_points = 3;

    second_card.attack_points = 1;
    second_card.health_points = 2;

    printf("Attacker[%d|%d] vs Defender[%d|%d]:\n\n",first_card.attack_points,first_card.health_points,second_card.attack_points,second_card.health_points);
    if(attack(&first_card,&second_card) == 0){
        printf("Cards destroyed !\n");
    }
    else if(attack(&first_card,&second_card) == 1){
        printf("Attacker wins !\n");
    }
    else if(attack(&first_card,&second_card) == 2){
        printf("Defender wins !\n");
    }
    else if(attack(&first_card,&second_card) == 3){
        printf("Both cards survived !\n");
    }
    else{
        printf("Error !\n");
    }
    return 0;
}
