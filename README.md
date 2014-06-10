swag 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* name;
	int attack;
	int energy_cost;
}ability_t;

typedef struct{
	char* name;
	int hp;
	int energy;
	ability_t abilities[2];
}player_t;

void init_player(player_t* player){
	player->name = NULL;
	player->hp = 100;
	player->energy = 50;
}

void load_player(player_t* player,FILE* file){
	if(file){
		memset(player,0,sizeof(player_t));
		char* token = NULL;
		char buffer[100];
		fgets(buffer,100,file);
		
		//Load player name
		token = strtok(buffer,",");
		player->name = token + '\0';
		
		//Load hp
		token = strtok(NULL,",");
		player->hp = atoi(token);
		
		//Load energy
		token = strtok(NULL,",");
		player->energy = atoi(token);
		
		//Load first ability
		token = strtok(NULL,",");
		player->abilities[0].name = token;
		
		token = strtok(NULL,",");
		player->abilities[0].attack = atoi(token);
		
		token = strtok(NULL,",");
		player->abilities[0].energy_cost = atoi(token);
		
		//Load second ability
		token = strtok(NULL,",");
		player->abilities[1].name = token;
		
		token = strtok(NULL,",");
		player->abilities[1].attack = atoi(token);
		
		token = strtok(NULL,",");
		player->abilities[1].energy_cost = atoi(token);

		printf("Loaded player data \n");
	}
	else{
		printf("File missing or corrupt\n");
	}
}

void fight(player_t* attacker,player_t* defender){
	int r = 0;
	
	defender->hp -= attacker->abilities[r].attack;
	printf("%s used %s against %s\n",attacker->name,attacker->abilities[r].name,defender->name);
	attacker->energy =attacker->abilities[r].energy_cost;
}

int main(){
	FILE* file = NULL;
	file = fopen("ab.csv","r");
	player_t players[2];
	load_player(&players[0],file);
	load_player(&players[1],file);
	players[0].name = "random";
	printf("%s\n",players[0].name);
	while((players[0].hp > 0 && players[0].energy > 0) && (players[1].hp > 0 && players[1].energy > 0)){
		fight(&players[0],&players[1]);
	}
	
	fclose(file);
	
	return 0;
}

/*
Vili,100,50,Kick,5,2,Punch,7,4
Hristiqn,100,50,Spell,8,6,Poisonous dart,4,1
*/
