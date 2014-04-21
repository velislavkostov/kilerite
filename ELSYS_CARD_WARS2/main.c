#include <stdio.h>
#include "deck.h"

const char* CARD_NAMES[] = {"Dragon Kreator","Sub Zero","Shadow Warrior"};

void print_deck(deck_t *deck){
    card_t card;
    while(deck->top > 0){
        printf("%d|%s|\n",deck->top,deck->cards[deck->top - 1]);
        draw_card(deck,&card);
    }
}

int main()
{
    deck_t deck;
    deck.top = 0;
    card_t card;
    int i;
    for(i = 0;i < 3;i++){
        card.name = CARD_NAMES[i];
        push_card(card,&deck);
    }
    print_deck(&deck);

    return 0;
}
