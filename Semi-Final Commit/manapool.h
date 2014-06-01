#ifndef MANAPOOL_H_INCLUDED
#define MANAPOOL_H_INCLUDED

#define MAX_MANA 10

typedef struct{
    int mana_crystals;
} manapool;

int can_put_card(card_t card,manapool pool);

#endif // MANAPOOL_H_INCLUDED
