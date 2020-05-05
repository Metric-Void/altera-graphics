#include<stdio.h>
#include "logic.h"

int main(int argc, char** argv) {
    Game maingame;
    init_game(&maingame);
    printf("Game initialization Complete\n");

    print_board(&maingame);

    char getKB = getchar();
    while(getKB != 'q' && maingame.state == PLAYING) {
        if(getKB != '\n')tickgame(&maingame, getKB);
        print_board(&maingame);
        getKB = getchar();
    }
    if(maingame.state == DIED) printf("You lost.\n");
    printf("Exiting...");
    return 0;
}
