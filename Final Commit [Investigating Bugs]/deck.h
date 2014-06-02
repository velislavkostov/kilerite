#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#include "card.h"

#define DECK_SIZE 30
typedef struct {
    int size;
    //int deck_owner; //Switch from type int to type Player later (after Player structure definition)
    card_t cards[DECK_SIZE];
}deck_t;

int push_card(card_t card_to_push,deck_t *player_deck);
int draw_card(deck_t *player_deck,card_t *card);
int look_card(deck_t *player_deck,card_t *card);

#endif // DECK_H_INCLUDED
