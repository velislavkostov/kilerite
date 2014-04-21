#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

typedef struct {
    char *name;
    int attack_points;
    int health_points;
    int mana_cost;
}card_t;

int attack(card_t *attacker,card_t *defender);

#endif // CARD_H_INCLUDED
