//chava haviv - 211737440
#include "mineSweeper.h"
#include "colorPrint.h"
#include "getch.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int rows, cols, level;
    puts("pleas enter a row number [1-20]:");
    scanf("%d", &rows);
    puts("pleas enter a col number [1-20]:");
    scanf("%d", &cols);
    puts("pleas enter a level [1-Easy, 2-Medium, 3-Hard]:");
    scanf("%d", &level);

    GameBoard g;
    if (!initBoard(&g, rows, cols, level))
        exit(1);//if malloc faild.
    int cursorCoords[2] = {0, 0};
    getch();
    char get = ' ';
    while (!g.isMineClicked && g.hiddenTiles != 0) {
        //while we are not wining or losing.
        printBoard(&g, cursorCoords);
        get = getch();//gets input from the user.
        if (get == FLAG_TILE) {
            flagTile(&g, cursorCoords[0], cursorCoords[1]);
        } else if (get == CLICK_TILE) {
            clickTile(&g, cursorCoords[0], cursorCoords[1]);
        } else if (get == QUIT) {
            puts("End of game");
            break;
        } else if (get == LEFT && cursorCoords[1] != 0) {
            cursorCoords[1]--;
        } else if (get == RIGHT && cursorCoords[1] != g.cols - 1) {
            cursorCoords[1]++;
        } else if (get == UP && cursorCoords[0] != 0) {
            cursorCoords[0]--;
        } else if (get == DOWN && cursorCoords[0] != g.rows - 1) {
            cursorCoords[0]++;
        }
        //Moves the cursor according to the user's movements.

    }
    if (get != QUIT) {
        cursorCoords[0] = -1;
        cursorCoords[1] = -1;
        if (g.hiddenTiles == 0) {
            printBoard(&g, cursorCoords);
            puts("you won!");
        } else {
            printBoard(&g, cursorCoords);
            puts("game over!");
        }
        //Removes the marker from the board so that we see the entire board 
        //at the end of the game.
    }
    //if we quit we also do "free".
    for (int i = 0; i < rows; i++) {
        free((g.board)[i]);
    }
    free(g.board);

    return 0;
}
