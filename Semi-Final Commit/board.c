#include "board.h"

int init_board(board_t *board){
    printf("[INIT]\n\tInitializing board...\n");
    memset(board,0,sizeof(board_t));
    board->turn_count = 1;
    init_player(&board->players[0]);
    init_player(&board->players[1]);
    printf("\tBoard successfully initialized.\n[/INIT]\n");
    return 0;
}

