#include <stdio.h>
#include "card.h"
#include "manapool.h"

#define CARD_INITIALIZER {0,0,0,0}

int main()
{
    manapool pool;
    card_t card = CARD_INITIALIZER;

    pool.mana_crystals = 5;

    card.mana_cost = 3;
    printf("Your current mana is: %d\n",pool.mana_crystals);
    can_put_card(card,pool) ? printf("You CAN place card requiring %d mana\n",card.mana_cost): printf("You CAN't place card requiring %d mana\n",card.mana_cost);

    return 0;
}
