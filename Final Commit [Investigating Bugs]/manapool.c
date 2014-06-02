#include "card.h"
#include "manapool.h"


int can_put_card(card_t card,manapool pool)
{
    return (pool.mana_crystals >= card.mana_cost ? 1 : 0);
}
