#include "card.h"

int attack(card_t *attacker,card_t *defender)
{
    //Perform attack calculations
    attacker->health_points -= defender->attack_points;
    defender->health_points -= attacker->attack_points;

    /*Return code based on attack results*/
    if(attacker->health_points <= 0 & defender->health_points <= 0)
    {
        //Both cards were destroyed
        return 0;
    }
    else if(attacker->health_points > 0 & defender->health_points <= 0)
    {
        //Attacker is the winner
        return 1;
    }
    else if(attacker->health_points <= 0 & defender->health_points > 0)
    {
        //Defender is the winner
        return 2;
    }
    else if(attacker->health_points > 0 & defender->health_points > 0)
    {
        //Both cards survived the attack
        return 3;
    }
    else
    {
        return -1;
    }
}
