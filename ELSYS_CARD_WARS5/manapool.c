#include "card.h"
#include "manapool.h"


int can_put_card(card_t card,manapool pool)
{
    return (pool.mana_crystals == card.mana_cost ? 1 : 0);
}

int change_mana(manapool *pool,int ammount_of_crystals)
{   if (ammount_of_crystals >= 1 && ammount_of_crystals <= 10)
    {
        pool->mana_crystals = ammount_of_crystals;
        return(0);
    }
    else return -1;
}
