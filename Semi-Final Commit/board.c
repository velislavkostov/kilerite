#include "board.h"

int init_board(board_t *board){
    printf("[INIT]\n\tInitializing board...\n");
    memset(board,0,sizeof(board_t));
    int i;
    for(i = 0; i < DECK_SIZE; i++){
        card_t card;
        memset(&card,0,sizeof(card_t));
        calculate_vital_ratios(&card);
        push_card(card,&board->players[0].deck_of_cards);
        push_card(card,&board->players[1].deck_of_cards);

        board->players[0].hand.cards[i].layout.dragged = false;
        board->players[1].hand.cards[i].layout.dragged = false;
    }
    init_player(&board->players[0]);
    init_player(&board->players[1]);
    printf("\tBoard successfully initialized.\n[/INIT]\n");
    return 0;
}

