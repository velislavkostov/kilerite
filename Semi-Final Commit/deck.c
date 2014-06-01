#include "deck.h"

//Push card into deck
int push_card(card_t card_to_push,deck_t *player_deck){
    if (player_deck->size < DECK_SIZE){
            player_deck->cards[player_deck->size++] = card_to_push;
        return 0;
    }
    return -1;
}

//Draws card from player deck
int draw_card(deck_t *player_deck,card_t *card){
    if (player_deck->size > 0){
        card = &(player_deck->cards[--player_deck->size]);
        return 0;
    }
    return -1;
}
//Look the topmost card of the deck
int look_card(deck_t *player_deck,card_t *card){
    if(!player_deck->size == 0){
        card = &player_deck->cards[player_deck->size - 1];
    }
    return 0;
}
