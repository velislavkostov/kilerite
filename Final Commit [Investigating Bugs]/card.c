#include "card.h"

int attack(int* attacker_hp,int* defender_hp,card_t *attacker,card_t *defender){
    //Perform attack calculations
    if(attacker->layout.cache_index == -1){
        *attacker_hp -= defender->attack_points;
    }
    else if(defender->layout.cache_index == -1){
        *defender_hp -= attacker->attack_points;
    }
    else{
        defender->health_points -= attacker->attack_points;
        attacker->health_points -= defender->attack_points;
    }


    /*Return code based on attack results*/
    if((attacker->health_points <= 0) & (defender->health_points <= 0)){
        //Both cards were destroyed
        return 0;
    }
    else if((attacker->health_points > 0) & (defender->health_points <= 0)){
        //Attacker is the winner
        return 1;
    }
    else if((attacker->health_points <= 0) & (defender->health_points > 0)){
        //Defender is the winner
        return 2;
    }
    else if((attacker->health_points > 0) & (defender->health_points > 0)){
        //Both cards survived the attack
        return 3;
    }
    return -1;
}
