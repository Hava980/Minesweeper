//chava haviv - 211737440
#include "mineSweeper.h"
#include "colorPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


bool isValid(int, int, GameBoard *g);

void neighbors(int i, int j, GameBoard *g, bool is_clicked);


bool initBoard(GameBoard *g, int rows, int cols, int level) {
    if (cols < 1) {
        cols = 4;
    }
    if (rows < 1) {
        rows = 4;
    }
    if (cols > MAX_BOARD_DIM) {
        cols = MAX_BOARD_DIM;
    }
    if (rows > MAX_BOARD_DIM) {
        rows = MAX_BOARD_DIM;
    }
    //checks the inpute and initialaize it whith a difult values. 

    g->cols = cols;
    g->rows = rows;
    g->isMineClicked = FALSE;
    //initialaize g values.

    g->board = (Tile **) malloc(g->rows * sizeof(Tile *));
    if (g->board == NULL) {
        return FALSE;
    }
    for (int i = 0; i < g->rows; i++) {
        (g->board)[i] = (Tile *) malloc(g->cols * sizeof(Tile));
        if ((g->board)[i] == NULL) {
            return FALSE;
        }
    }
    //allocate space to the board.

    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            g->board[i][j].numOfMines = 0;
            g->board[i][j].isVisible = FALSE;
            g->board[i][j].isMine = FALSE;
            g->board[i][j].isFlagged = FALSE;
        }
    }
    //initialaize Tiles values.

    populateMines(g, level);// puts mines
    g->hiddenTiles = (g->cols * g->rows) - (g->totalMines);
    markNumbers(g);//marks the Tiles whit the number of the mines around them.
    return TRUE;
}


void populateMines(GameBoard *g, int level) {

    if (level == HARD) {
        g->totalMines = (HARD_LEVEL_PERCENT * (g->cols * g->rows)) / 100;
    } else if (level == MEDIUM) {
        g->totalMines = (MEDIUM_LEVEL_PERCENT * (g->cols * g->rows)) / 100;
    } else {
        g->totalMines = (EASY_LEVEL_PERCENT * (g->cols * g->rows)) / 100;
    }
    //initialaizes total mines according to the level.
    //the difult level is easy.
    for (int k = 0; k < g->totalMines; k++) {
        int i = rand() % g->rows;
        int j = rand() % g->cols;
        //creates random location.
        if (g->board[i][j].numOfMines == -1)
            k--;
            //if there is a mine in the random location then the loop runs 
            //one more time so that we don't miss a mine.
        else {
            g->board[i][j].numOfMines = -1;
            g->board[i][j].isMine = TRUE;
            //initialaizes board[i][j] to be mine.
        }
    }
}

void markNumbers(GameBoard *g) {
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            if (g->board[i][j].isMine == TRUE)
                neighbors(i, j, g, FALSE);
        }
    }
//for each mine we go through all its neighbors and update the number of their mines.
}


void clickTile(GameBoard *g, int row, int col) {

    if (g->board[row][col].isVisible || g->board[row][col].isFlagged)
        return;//if the tile is visible or the tile as flagged.

    if (g->board[row][col].isMine) {
        g->board[row][col].isVisible = TRUE;
        g->isMineClicked = TRUE;//סוף משחק
        return;
        //if we click on a mine.
    } else {
        g->board[row][col].isVisible = TRUE;
        (g->hiddenTiles)--;
        //else we revele the tile and update the hiddenTiles.
    }
    if (g->board[row][col].numOfMines == 0) {
        neighbors(row, col, g, TRUE);
//if in addition the numOfMines=0 then we open all of is nighbors that are not a mine.
    }

}

void flagTile(GameBoard *g, int row, int col) {

    if (g->board[row][col].isVisible) {
        return;//if the tile isVisible then we do nothing.
    }
    g->board[row][col].isFlagged = !(g->board[row][col].isFlagged);
    //else we pute a flagge or we remove the flagge.
}

void printBoard(GameBoard *g, int cursorCoords[2]) {
    clearScreen();
    for (int i = 0; i < g->rows; i++) {
        for (int j = 0; j < g->cols; j++) {
            if (i == cursorCoords[0] && j == cursorCoords[1]) {
                colorPrint(FG_Blue, BG_Blue, ATT_Def, "  ");
                //colors the cursor.
            } else if (g->board[i][j].isVisible == FALSE && g->board[i][j].isFlagged == FALSE) {
                colorPrint(FG_White, BG_White, ATT_Def, "  ");
                //colors the hiddenTiles.
            } else if (g->board[i][j].isVisible == FALSE) {
                colorPrint(FG_Red, BG_White, ATT_Def, " F");
                //colors the flagge.
            } else if (g->board[i][j].isMine) {
                colorPrint(FG_Black, BG_Red, ATT_Def, " M");
                //colors the mine.
            } else if(g->board[i][j].numOfMines==0){
                colorPrint(FG_Black, BG_Magenta, ATT_Def, "  ");
            } else {
                char temp[3];
                sprintf(temp, " %d", g->board[i][j].numOfMines);
                colorPrint(FG_Black, BG_Magenta, ATT_Def, temp);
                //colors a regular tile.
            }

        }
        printf("\n");
    }
}


bool isValid(int i, int j, GameBoard *g) {
    if (-1 < i && i < g->rows && -1 < j && j < g->cols) {
        if (g->board[i][j].numOfMines != -1)
            return TRUE;
    }
    return FALSE;
    //checks if the location are valid and also are not a mine.
}

void neighbors(int i, int j, GameBoard *g, bool is_clicked) {
//The function goes through all the neighbors of clickTile when is_clicked=TRUE
//if is_clicked=FALSE the function goes through all the neighbors of markNumbers

    if (isValid(i, j + 1, g)) {
        if (is_clicked) {
            clickTile(g, i, j + 1);
        } else {
            g->board[i][j + 1].numOfMines += 1;
        }
    }
    if (isValid(i + 1, j + 1, g)) {
        if (is_clicked) {
            clickTile(g, i + 1, j + 1);
        } else {
            g->board[i + 1][j + 1].numOfMines += 1;
        }
    }
    if (isValid(i - 1, j + 1, g)) {
        if (is_clicked) {
            clickTile(g, i - 1, j + 1);
        } else {
            g->board[i - 1][j + 1].numOfMines += 1;
        }
    }
    if (isValid(i + 1, j, g)) {
        if (is_clicked) {
            clickTile(g, i + 1, j);
        } else {
            g->board[i + 1][j].numOfMines += 1;
        }
    }
    if (isValid(i - 1, j, g)) {
        if (is_clicked) {
            clickTile(g, i - 1, j);
        } else {
            g->board[i - 1][j].numOfMines += 1;
        }
    }
    if (isValid(i - 1, j - 1, g)) {
        if (is_clicked) {
            clickTile(g, i - 1, j - 1);
        } else {
            g->board[i - 1][j - 1].numOfMines += 1;
        }

    }
    if (isValid(i + 1, j - 1, g)) {
        if (is_clicked) {
            clickTile(g, i + 1, j - 1);
        } else {
            g->board[i + 1][j - 1].numOfMines += 1;
        }

    }
    if (isValid(i, j - 1, g)) {
        if (is_clicked) {
            clickTile(g, i, j - 1);
        } else {
            g->board[i][j - 1].numOfMines += 1;
        }

    }

}



