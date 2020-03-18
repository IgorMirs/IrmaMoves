#ifndef __IRMAMOVES_H
#define __IRMAMOVES_H


typedef struct Location
{
	// the square's column ('a' through 'h')
	char col;

	// the square's row (1 through 8)
	int row;
} Location;

typedef struct Irma
{
	// wind speed (MPH)
	int ws;

	// wind gusts (MPH)
	int wg;

}Irma;

typedef struct Move
{
	// an instance of Irma
	Irma irma;

	// location where this piece is moving from
	Location from_loc;

	// location where this piece is currently exist
	Location current_loc;

	// location where Irma is moving to
	Location to_loc;

} Move;


// Functional Prototypes

char** createMapBoard(void);

char** destroyMapBoard(char** board);

void printMapBoard(char** board);

char** predictIrmaChange(char* str, Move* irmaMove);

void parseNotationString(char* str, Move* Irma);

double difficultyRating(void);

double hoursSpent(void);


#endif
