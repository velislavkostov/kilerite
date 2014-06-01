#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "deck.h"
#include "manapool.h"

typedef struct{
    card_t cards[DECK_SIZE];
    int size;
}hand_t;

typedef struct {
    deck_t deck_of_cards;
    manapool pool_of_mana;
    //Hand has the size of a deck, because it is possible to hold all cards from your deck
    hand_t hand;
    int health_points;
    graph_object hp_texture;
} player_t;

int change_hp(player_t *player,int hp);
card_t* play_card_from_hand(player_t *player,int card_index);

#endif // PLAYER_H_INCLUDED
