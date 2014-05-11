#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
#include "SDL.h"
#include "my_base_types.h"

typedef struct{
    SDL_Texture* texture;
    SDL_Rect rectangle;
    bool_t focused;
    bool_t dragged;
}graph_object;

typedef struct {
    char *name;
    int attack_points;
    int health_points;
    int mana_cost;
    graph_object layout;
}card_t;

int attack(card_t *attacker,card_t *defender);

#endif // CARD_H_INCLUDED
