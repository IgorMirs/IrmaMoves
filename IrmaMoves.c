/*Igor Mirsalikhov ID_7421*/
#define __STDC_WANT_LIB_EXT1__ 1
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "IrmaMoves.h"

#define row_ 8
#define col_ 8

//prototypes
void checkPoint(char* start);
void checkSpeed(char* str);
bool isLand(Location loc, char** board);
void updateSpeedH(Move* mv, char** board);
void updateSpeedV(Move* mv, char** board);

/*Creates initial configuration of a map board*/

char** createMapBoard(void)
{
    char** board = (char**)malloc(row_ * sizeof(char*) + 1);
    if (!board)
        return NULL;
    for (int i = 0; i < row_; i++) {
        board[i] = (char*)malloc(col_ * sizeof(char) + 1);
        if (!board[i])
            return NULL;
    }

    strncpy(board[0], "FF      ", col_);
    strncpy(board[1], " F      ", col_);
    strncpy(board[2], " FF     ", col_);
    strncpy(board[3], "  F     ", col_);
    strncpy(board[4], "  K     ", col_);
    strncpy(board[5], "C  B    ", col_);
    strncpy(board[6], " CC D   ", col_);
    strncpy(board[7], "  C  DD ", col_);
    
	return board;
}

/*Free all dynamically allocated memory associated with board*/

char** destroyMapBoard(char** board)
{
    for (int i = 0; i < 8; i++) {
        char* cur = board[i];
        free(cur);
    }
    free(board);
	return NULL;
}

/*Prints the map board*/

void printMapBoard(char** board)
{
    printf("========\n");
    for (int i = 0; i < row_; i++) {
        for (int j = 0; j < col_; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("========\n");
}

/*Print out the map board with starting and finishing positions of Irma.
Updates Irma wind and guts speeds after movement.*/

char** predictIrmaChange(char* str, Move* irmaMove)
{
    parseNotationString(str, irmaMove);
    char** board = createMapBoard();
    //original value of the board
    char initialBoard = board[irmaMove->from_loc.row][(int)(irmaMove->from_loc.col - 'a')];
    board[irmaMove->from_loc.row][(int)(irmaMove->from_loc.col - 'a')] = 'I';
    printMapBoard(board);

    //move horizontal
    while (irmaMove->current_loc.col != irmaMove->to_loc.col) {
        if (irmaMove->current_loc.col < irmaMove->to_loc.col) {
            irmaMove->current_loc.col++;
            updateSpeedH(irmaMove, board);
        }
        else {
            irmaMove->current_loc.col--;
            updateSpeedH(irmaMove, board);
        }
    }

    //move vertical
    while (irmaMove->current_loc.row != irmaMove->to_loc.row) {
        if (irmaMove->current_loc.row < irmaMove->to_loc.row) {
            irmaMove->current_loc.row++;
            updateSpeedV(irmaMove, board);
        }
        else {
            irmaMove->current_loc.row--;
            updateSpeedV(irmaMove, board);
        }
    }

    board[irmaMove->from_loc.row][(int)(irmaMove->from_loc.col - 'a')] = initialBoard;
    board[irmaMove->current_loc.row][(int)(irmaMove->current_loc.col - 'a')] = 'I';
    printMapBoard(board);
	return board;
}

/*Checks whatever location is land of water*/

bool isLand(Location loc, char** board) {
    int r = loc.row, c = (int)(loc.col - 'a');
    if (board[r][c] != ' ')
        return true;
    else
        return false;
}

/*Updates speed after horizontal movement*/

void updateSpeedH(Move* mv, char** board) {
    if (isLand(mv->current_loc, board)) {
        mv->irma.wg = mv->irma.wg - 10;
        mv->irma.ws = mv->irma.ws - 15;
    }
    else {
        mv->irma.wg = mv->irma.wg + 5;
        mv->irma.ws = mv->irma.ws + 10;
    }

    if (mv->irma.wg < 0)
        mv->irma.wg = 0;
    if (mv->irma.ws < 0)
        mv->irma.ws = 0;
}

/*Update speed after vertical movement*/

void updateSpeedV(Move* mv, char** board) {
    if (isLand(mv->current_loc, board)) {
        mv->irma.wg = mv->irma.wg - 1;
        mv->irma.ws = mv->irma.ws - 2;
    }
    else {
        mv->irma.wg = mv->irma.wg + 3;
        mv->irma.ws = mv->irma.ws + 6;
    }
}
/*The function parse str and extract information about Irma moves encoded there, 
and populate all corresponding fields in the struct pointed to by irmaMove.*/

void parseNotationString(char* str, Move* Irma) {
    const int length = strlen(str);
    if (length > 13 || length < 9) {
        printf("ERROR! String is not valid");
        exit(0);
    }
    //string for spliting the original string
    char str_temp[50];
    strcpy(str_temp, str);
    //string with separators
    char seps[] = " ,\t\n";
    char* token = NULL;
    char* next_token1 = NULL;
    //strings for Irma parameters
    char start[3];
    char windSpeed[4];
    char gutsSpeed[4];
    char dest[3];


    //Split the original string on different parameters and check each parameter
    token = strtok_s(str_temp, seps, &next_token1);
    checkPoint(token);
    strcpy(start, token);
    token = strtok_s(NULL, seps, &next_token1);
    checkSpeed(token);
    strcpy(windSpeed, token);
    token = strtok_s(NULL, seps, &next_token1);
    checkSpeed(token);
    strcpy(gutsSpeed, token);
    token = strtok_s(NULL, seps, &next_token1);
    checkPoint(token);
    strcpy(dest, token);

    Irma->irma.ws = atoi(windSpeed);
    Irma->irma.wg = atoi(gutsSpeed);
    //starting location
    Irma->from_loc.col = *((char*)start);
    Irma->from_loc.row = atoi(start + 1);
    //destination location
    Irma->to_loc.col = *((char*)dest);
    Irma->to_loc.row = atoi(dest + 1);
    //current location
    Irma->current_loc.col = *((char*)start);
    Irma->current_loc.row = atoi(start + 1);
}

/*Checks the correctnes of start and end positions
If the point is longer than two symbols or the coordinates are out of the board - prints error
*/

void checkPoint(char* str) {

    if (!str || strlen(str) != 2 || *((char*)str) > 'h' || *((char*)str) < 'a' ||
        *((char*)(str + 1)) < '0' || *((char*)(str + 1)) > '8') {
        printf("ERROR!!! Incorrect start or end point!");
        exit(0);
    }
}

/*Checks the correctnes of speed
If the point is longer than two symbols or the coordinates are out of the board - prints error
*/

void checkSpeed(char* str) {
    if (!str) {
        printf("ERROR!!! Incorrect speed value!");
        exit(0);
    }
    int speed = atoi(str);
    if (speed <= 0 || speed > 999){
        printf("ERROR!!! Incorrect speed value!");
        exit(0);
    }
}

/*Returns difficulty rating of the assignment from 1.0 to 5.0*/

double difficultyRating(void)
{
	return 2.5;
}

/*Returns total hours spent on this assignment*/

double hoursSpent(void)
{
	return 5.0;
}


