#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define MAX_NUMBER_CARDS 50
#define MAX_LENGTH_NAME 20
#define MIN_PLAYERS 2
#define INITIAL_AMOUNT 4
#define NUMBER 'N'
#define TAKI 'T'
#define CHANGE_DIRECTION '<'
#define STOP 'S'
#define PLUS '+'
#define COLOR 'C'
#define YELLOW 'Y'
#define BLUE 'B'
#define RED 'R'
#define GREEN 'G'
#define NO_COLOR '0'
#define IS_NOT_TAKI false
#define IS_TAKI true
#define GAME_IS_ON -1
#define NUMBER_OF_TYPES 14
#define INT_TAKI 10
#define INT_COLOR 13
#define INT_PLUS 12
#define INT_CHANGE 11
#define INT_STOP 0
#define DIR_CLOCKWISE 1
#define DIR_COUNTER_CLOCKWISE -1
typedef struct card //CARD contains the details of a single card(number,type,color)
{
	int number;
	char type;
	char color;
}CARD;
typedef struct player//PLAYER contains the data of a single player
{
	char firsrName[MAX_LENGTH_NAME];
	CARD * cards_in_hand;
	int logSize;
	int phisicalSize;
}PLAYER;
typedef struct statisticLine //STATS contains the frequency of spesific card in a game
{
	int type;
	int amount;
}STATS;
void openMessage();
PLAYER* getPlayersInfo(int * size);
void getInitialCards(PLAYER * players, int * size);
void setCard(CARD * card, bool special);
void printCard(CARD card);
void printPlayerCards(PLAYER* players, int player);
void addCard(PLAYER* players, int player, STATS* temp);
int getChoice(CARD* gameCard, PLAYER* players, int player, bool* taki);
bool isValid(CARD gameCard, CARD playerCard, bool* taki);
void makeMove(PLAYER* players, int* player, CARD* gameCard, int choice, int* direction, int* currentPlayers);
void cardToEnd(PLAYER* players, int player, int choice);
int isWin(PLAYER* players, int size);
void playGame(PLAYER* players, CARD* gameCard, int* player, int* direction, int* currentPlayers, bool* taki, STATS* temp);
void getColor(CARD* gameCard);
CARD* arrReaclloc(CARD* cards, int size, int newSize);
void getTaki(PLAYER* players, int* player, CARD* gameCard, int* direction, int* currentPlayers, bool* taki, STATS* temp);
void merge(STATS* table1, int size1, STATS* table2, int size2, STATS* temp);
void copy(STATS* table, STATS* temp, int size);
void mergeSort(STATS* table, int size);
void printStatistics(STATS* table);
void initializeStats(STATS* table);
void main()
{
	STATS table[NUMBER_OF_TYPES];
	int currentPlayers, curPlayer, dir,i;
	int turn = true;
	int statusGame = GAME_IS_ON;
	bool Taki=IS_NOT_TAKI;
	srand(time(NULL));
	PLAYER* players;
	CARD cardPlay;
	CARD* pCardPlay = &cardPlay;
	setCard(pCardPlay, false);
	initializeStats(table);
	openMessage();
	players=getPlayersInfo (&currentPlayers);
	getInitialCards(players, &currentPlayers);
	curPlayer = rand() % currentPlayers;//gets random player
	dir = rand() % 2;
	switch (dir) //gets random direction
	{
	case 0:
		dir = DIR_COUNTER_CLOCKWISE;
		break;
	case 1:
		dir = DIR_CLOCKWISE;
		break;
	}
	while (statusGame == GAME_IS_ON)
	{
		playGame(players, pCardPlay, &curPlayer, &dir, &currentPlayers, &Taki,table);

		statusGame = isWin(players, currentPlayers);
	}
	printf("\nThe winner is... %s! Congratulations!", players[statusGame].firsrName);
	for (i = 0; i < currentPlayers; i++)
	{
		if (players[i].cards_in_hand != NULL)
		{
			free(players[i].cards_in_hand);
		}
	}
	free(players);
	mergeSort(table, NUMBER_OF_TYPES);
	printStatistics(table);
}
void openMessage() //prints open message
{
	printf("************  Welcome to TAKI game !!! ***********\n");
}
PLAYER* getPlayersInfo(int * size)//This function gets array of players and integer pointer.
								 //The function collects all names of the players
{
	int i;
	PLAYER* players;
	printf("Please enter the number of players:\n");
	scanf("%d",size);
	while (*size<MIN_PLAYERS)
	{
		printf("Invaild number! please enter an integer number larger than %d:\n", MIN_PLAYERS-1);
		scanf("%d",size);
	}
	players = (PLAYER*)malloc((*size) * sizeof(PLAYER));
	if (players == NULL)
	{
		printf("Allocation memory faild!");
		exit(1);
	}
	for (i = 0; i < *size; i++)
	{
		printf("Please enter the first name of player #%d:\n", i+1);
		scanf("%s", players[i].firsrName);
	}
	return players;
}
void getInitialCards(PLAYER* players, int* size)/*This function gets array of players and integer pointer.
												The function setting initial Cards for all players*/
{
	int i,j;
	for (i = 0; i < *size; i++)
	{
		players[i].cards_in_hand = (CARD*)malloc(INITIAL_AMOUNT * sizeof(CARD));
		if (players[i].cards_in_hand == NULL)
		{
			printf("Allocation memory faild!");
			exit(1);
		}
		for (j = 0; j < INITIAL_AMOUNT; j++)
		{
			
			setCard(&players[i].cards_in_hand[j],true);
		}
		players[i].logSize = INITIAL_AMOUNT;
		players[i].phisicalSize = INITIAL_AMOUNT;
	}
}
void setCard(CARD * card, bool special)/*This function gets CARD pointer and boolean variable
									   and definies the card by random choice with number,type,color*/
{
	int type,color;
	if (special)
	{
		type = rand() % 14 +1;
		switch (type)
		{
		case 10:
			card->type = TAKI;
			break;
		case 11:
			card->type = COLOR;
			break;
		case 12:
			card->type = PLUS;
			break;
		case 13:
			card->type = CHANGE_DIRECTION;
			break;
		case 14:
			card->type = STOP;
			break;
		default:
			card->type = NUMBER;
			card->number = type;
		}
		if(card->type==COLOR)
			card->color = NO_COLOR;
		if (card->type != COLOR)
		{
			
			color = rand() % 4 + 1;
			switch (color)
			{
			case 1:
				card->color = YELLOW;
				break;
			case 2:
				card->color = BLUE;
				break;
			case 3:
				card->color = GREEN;
				break;
			case 4:
				card->color = RED;
				break;
			default:;
			}
		}
	}
	else
	{
		card->number= rand() % 9 + 1;
		card->type = NUMBER;
		color = rand() % 4 + 1;
		switch (color)
		{
		case 1:
			card->color = YELLOW;
			break;
		case 2:
			card->color = BLUE;
			break;
		case 3:
			card->color = GREEN;
			break;
		case 4:
			card->color = RED;
			break;
		default:;
		}
	}
	
}
void printCard(CARD card)/*This function get CARD variable and prints it by a spesific form*/
{
	printf("*********\n");
	printf("*       *\n");
	switch (card.type)
	{
	case NUMBER:
		printf("*   %d   *\n", card.number);
		printf("*   %c   *\n", card.color);
		break;
	case COLOR:
		printf("* COLOR *\n");
		if(card.color!=NO_COLOR)
			printf("*   %c   *\n", card.color);
		else
			printf("*       *\n");
		break;
	case STOP:
		printf("*  STOP *\n");
		printf("*   %c   *\n", card.color);
		break;
	case TAKI:
		printf("*  TAKI *\n");
		printf("*   %c   *\n", card.color);
		break;
	case PLUS:
		printf("*   +   *\n");
		printf("*   %c   *\n", card.color);
		break;
	case CHANGE_DIRECTION:
		printf("*  <->  *\n");
		printf("*   %c   *\n", card.color);
		break;
	default:;
	}
	printf("*       *\n");
	printf("*********\n");
}
void printPlayerCards(PLAYER* players,int player) /*This function gets array of players ,current player as integer.
												The function setting initial Cards for all players*/
{
	int i;
	printf("\n%s's turn:\n", players[player].firsrName);
	for (i = 0; i < players[player].logSize; i++)
	{
		printf("\nCard #%d:\n",i+1);
		printCard(players[player].cards_in_hand[i]);
	}
}
void addCard(PLAYER* players, int player, STATS* table)/*This function gets array of players ,current player as integer and STATS table
													   the functon add a card to the current player and updates the STATS table*/
{
	int length = players[player].logSize;
	int num;
	if (players[player].phisicalSize == length)
	{
		players[player].phisicalSize *=2;
		players[player].cards_in_hand = arrReaclloc(players[player].cards_in_hand, length, players[player].phisicalSize);
	}
	setCard(&(players[player].cards_in_hand[length]), true);
	if (players[player].cards_in_hand[length].type == NUMBER)
	{
		num = players[player].cards_in_hand[length].number;
		table[num].amount++;
	}
	else
	{
		switch (players[player].cards_in_hand[length].type)
		{
		case COLOR:
			table[INT_COLOR].amount++;
			break;
		case TAKI:
			table[INT_TAKI].amount++;
			break;
		case STOP:
			table[INT_STOP].amount++;
			break;
		case PLUS:
			table[INT_PLUS].amount++;
			break;
		case CHANGE_DIRECTION:
			table[INT_CHANGE].amount++;
			break;
		default:
			table[players[player].cards_in_hand[length].number].amount++;
			break;
		}
	}

	players[player].logSize++;
}
int getChoice(CARD* gameCard, PLAYER* players, int player,bool * taki)/*This function gets array of players ,current player as integer,gameCard as CARD and boolean var
																	  return as integer the correct choice to make a move*/
{
	int choice;
	bool validMove;
	if((*taki)==IS_NOT_TAKI)
		printf("Please enter 0 if you want to take a card from the deck or 1 - %d \nif you want to put one of your cards in the middle : \n",players[player].logSize);
	else
		printf("Please enter 0 if you want to finish your turn \nor 1 - %d if you want to put one of your cards in the middle : \n", players[player].logSize);
	scanf("%d", &choice);
	while (choice < 0 || choice> players[player].logSize)
	{
		printf("Invalid choice! Try again.\n");
		if ((*taki) == IS_NOT_TAKI)
			printf("Please enter 0 if you want to take a card from the deck or 1 - %d \nif you want to put one of your cards in the middle : \n", players[player].logSize);
		else
			printf("Please enter 0 if you want to finish your turn \nor 1 - %d if you want to put one of your cards in the middle : \n", players[player].logSize);
		scanf("%d", &choice);
	}
	if (choice == 0)
		return choice;
	else
	{
		validMove = isValid(*gameCard, players[player].cards_in_hand[choice-1],taki);
		if (validMove)
			return choice;
		else
		{
			printf("Invalid card! Try again.\n");
			return getChoice(gameCard, players, player,taki);
		}
	}
}
bool isValid(CARD gameCard, CARD playerCard, bool* taki)/*This function gets two CARD variables and boolean var
														the function returns true/false if it is valid for player to play with playerCard on gameCard*/
{
	
	if (*(taki) == IS_NOT_TAKI)
		if (playerCard.type == COLOR)
			return true;
	if ((playerCard.type == gameCard.type) && gameCard.type == NUMBER)
		if(*(taki)==IS_NOT_TAKI)
			if (playerCard.number==gameCard.number)
				return true;
	if (*(taki) == IS_NOT_TAKI)
		if ((playerCard.type == gameCard.type) && gameCard.type != NUMBER)
			return true;
	if (playerCard.color == gameCard.color)
		return true;
	return false;
}
void checkCard(CARD* gameCard, PLAYER* players, int * player,int *direction,int * currentPlayers, bool* taki, STATS* table)
/*gets gamecard as CARD ,array of players,integer pointers to currentplayer,current direction,number of players,pointer boolean var and STATS table
The function examines the card that was put to the center and operates accordingly*/
{
	switch ((*gameCard).type)
	{
	case NUMBER:
		break;
	case PLUS:
		if (players[*player].logSize == 0)
			addCard(players, *player,table);
		else
		{
			if ((*direction) == DIR_CLOCKWISE)
				(*player)--;
			else
				(*player)++;
			playGame(players, gameCard, player, direction, currentPlayers,taki,table);
		}
		break;
	case STOP:
		if (*currentPlayers == MIN_PLAYERS)
			if (players[*player].logSize == 0)
				addCard(players, *player,table);
		*player = *player + (*direction);
		break;
	case CHANGE_DIRECTION:
		if (*currentPlayers == MIN_PLAYERS)
			*player = *player + (*direction);
		else
		{
			if (*direction == DIR_CLOCKWISE)
				*direction = DIR_COUNTER_CLOCKWISE;
			else
				*direction = DIR_CLOCKWISE;
		}
		break;
	case COLOR:
		getColor(gameCard);
		break;
	case TAKI:
		if (players[*player].logSize>0)
			getTaki(players, player, gameCard,direction,currentPlayers, taki,table);
	}
}
void makeMove(PLAYER* players, int * player, CARD* gameCard,int choice, int* direction, int* currentPlayers)
/*The function gets players array,choice as integer, CARD pointer to gamecard and integer pointers to current player,direction and number of players
the function places the card that was chosen to the center.*/
{
	(*gameCard).type = players[*player].cards_in_hand[choice - 1].type;
	if (players[*player].cards_in_hand[choice - 1].type == NUMBER)
		(*gameCard).number=players[*player].cards_in_hand[choice - 1].number;
	if (players[*player].cards_in_hand[choice - 1].type != COLOR)
		(*gameCard).color = players[*player].cards_in_hand[choice - 1].color;
	players[*player].cards_in_hand[choice - 1].color = NO_COLOR;
	if (choice != players[*player].logSize)
		cardToEnd(players,*player,choice-1);
	players[*player].logSize--;
}
void cardToEnd(PLAYER* players, int player, int choice)
/*The function gets players array, number of player and choice as integers
The funcion moves the chosen card of the current player to the end of his array of cards*/
{
	int i = 0;
	for (i = choice; i < players[player].logSize - 1; i++)
	{
		players[player].cards_in_hand[i].color = NO_COLOR;
		players[player].cards_in_hand[i].type = players[player].cards_in_hand[i + 1].type;
		if (players[player].cards_in_hand[i + 1].type == NUMBER)
			players[player].cards_in_hand[i].number = players[player].cards_in_hand[i + 1].number;
		if (players[player].cards_in_hand[i + 1].type != COLOR)
			players[player].cards_in_hand[i].color = players[player].cards_in_hand[i + 1].color;
	}
}
int isWin(PLAYER* players, int size)
/*The function gets array of players and the number of players as integer.
The function return the index of players, unless the function returns that there is no winner yet as integer.*/
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (players[i].logSize == 0)
			return i;
	}
	return GAME_IS_ON;
}
void playGame(PLAYER* players, CARD* gameCard, int * player, int* direction, int* currentPlayers,bool * taki, STATS* table)
/*The function gets players array,Card pointer to gamecard hoice , integer pointers to current player ,direction , number of players ,boolean pointer and array of STATS
This function is one of the main functions in the game which is responsible of the course of the game*/
{
	int choice;
	if (*player + *direction >= *currentPlayers)
		*player = *player+*direction-*currentPlayers;
	else if ((*player + *direction) < 0)
		*player = *currentPlayers+(*player+*direction);
	else
		*player = *player + *direction;
	printf("\nUpper card:\n");
	printCard(*gameCard);
	printPlayerCards(players, *player);
	choice = getChoice(gameCard, players, *player,taki);
	if (choice == 0)
		addCard(players, *player,table);
	else
	{
		makeMove(players, player, gameCard, choice, direction, currentPlayers);
		checkCard(gameCard, players, player, direction, currentPlayers,taki,table);
	}

}
void getColor(CARD* gameCard)/*This function gets CARD pointer to the gameCard
							 the function sets the color of the gamecard in case of COLOR card*/
{
	int num;
	printf("Please enter your color choice:\n");
	printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
	scanf("%d", &num);
	while (num < 1 || num>4)
	{
		printf("Invalid choice! ");
		printf("Please enter your color choice:\n");
		printf("1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
		scanf("%d", &num);
	}
	switch (num)
	{
	case 1:
		gameCard->color = YELLOW;
		break;
	case 2:
		gameCard->color = RED;
		break;
	case 3:
		gameCard->color = BLUE;
		break;
	case 4:
		gameCard->color = GREEN;
		break;
	default:;
	}


}
CARD* arrReaclloc(CARD* cards, int size, int newSize)/*The funcion gets array of cards and it's size as integer and also a new size
													 The function return a pointer to a new address which can fit the new size an dalso the function copies all data from the old array*/
{
	CARD * newCards;
	int i;
	if (newSize > 0)
	{
		newCards = (CARD*)malloc(newSize * sizeof(CARD));
		if (newCards == NULL)
		{
			printf("Allocation memory faild!");
			exit(1);
		}
		for (i = 0; i < size; i++)
		{
			newCards[i].color = cards[i].color;
			newCards[i].type = cards[i].type;
			newCards[i].number = cards[i].number;
		}
	}
	else
		newCards= NULL;
	free(cards);
	return newCards;
}
void getTaki(PLAYER* players, int* player, CARD* gameCard,int * direction ,int* currentPlayers, bool* taki, STATS* table)
/*The function gets players array,Card pointer to gamecard hoice , integer pointers to current player ,direction , number of players ,boolean pointer and array of STATS
The function is responsible to play with the Taki type of Card in case it presented  */
{
	(*taki) = IS_TAKI;
	int choice;
	bool checkLastCard=false;
	printf("\nUpper card:\n");
	printCard(*gameCard);
	printPlayerCards(players, *player);
	choice = getChoice(gameCard, players, *player, taki);
	if (choice == 0)
	{
		(*taki) = IS_NOT_TAKI;
	}
	else if ((players[*player].logSize - 1 == 0))
		(*taki) = IS_NOT_TAKI;
	while (*(taki)==IS_TAKI&&choice!=0)
	{
		checkLastCard = true;
		makeMove(players, player, gameCard, choice, direction, currentPlayers);
		printf("\nUpper card:\n");
		printCard(*gameCard);
		printPlayerCards(players, *player);
		choice = getChoice(gameCard, players, *player, taki);
		if (choice == 0)
		{
			(*taki) = IS_NOT_TAKI;
			break;
		}
		else if (players[*player].logSize - 1 == 0)
			(*taki) = IS_NOT_TAKI;

	}
	if(choice!=0)
		makeMove(players, player, gameCard, choice, direction, currentPlayers);
	if(checkLastCard)
		checkCard(gameCard, players, player, direction, currentPlayers,taki,table);
}
void printStatistics(STATS* table)/*The function gets array of STATS and prints all his elmentes*/
{
	int i;
	printf("\n\n************ Game Statistics ************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (i = NUMBER_OF_TYPES-1; i > -1; i--)
	{
		switch (table[i].type)
		{
		case INT_TAKI:
			printf(" TAKI  |    %d      \n",table[i].amount);
			break;
		case INT_CHANGE:
			printf("  <->  |    %d      \n", table[i].amount);
			break;
		case INT_PLUS:
			printf("   +   |    %d      \n", table[i].amount);
			break;
		case INT_COLOR:
			printf(" COLOR |    %d      \n", table[i].amount);
			break;
		case INT_STOP:
			printf(" STOP  |    %d      \n", table[i].amount);
			break;
		default:
			printf("   %d   |    %d      \n",table[i].type, table[i].amount);
			break;
		}
	}
}
void initializeStats(STATS* table)/*The function gets array of STATS and initialize all his elments*/
{
	int i = 0;
	for (i = 0; i < NUMBER_OF_TYPES; i++)
	{
		table[i].type = i;
		table[i].amount = 0;
	}
}
void mergeSort(STATS* table, int size) /*The function gets array of STATS and its size as integer .
									   The function sorts all elments in a merge sort type*/
{
	STATS* tmpArr;
	if (size <= 1)
		return;
	mergeSort(table,size/2);
	mergeSort(table+size/2,size-size/2);
	tmpArr = (STATS*)malloc(size * sizeof(STATS));
	if (tmpArr)
	{
		merge(table,size/2,table+size/2,size-size/2,tmpArr);
		copy(table,tmpArr,size);
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);
	}
}
void copy(STATS* table, STATS* temp, int size)
										/*The function gets two arrays of STATS in a same size and gets their integer size .
									  The function Copies all data from temp array to table array*/
{
	int i;
	for (i = 0; i < size; i++)
	{
		table[i].amount = temp[i].amount;
		table[i].type = temp[i].type;
	}
}
void merge(STATS* table1, int size1, STATS* table2, int size2, STATS* temp)
/*The function gets two sorted arrays of STATS and thier integer size and also gets temp array of STATS
the function merge the two sorted arrays into the temp array in a sorted way*/
{
	int read1=0, read2=0, write=0;
	while (read1 < size1 && read2 < size2)
	{
		if (table1[read1].amount <= table2[read2].amount)
		{
			temp[write].amount = table1[read1].amount;
			temp[write].type = table1[read1].type;
			read1++;
		}
		else
		{
			temp[write].amount = table2[read2].amount;
			temp[write].type = table2[read2].type;
			read2++;
		}
		write++;
	}
	while (read1 < size1)
	{
		temp[write].amount = table1[read1].amount;
		temp[write].type = table1[read1].type;
		read1++;
		write++;
	}
	while (read2 < size2)
	{
		temp[write].amount = table2[read2].amount;
		temp[write].type = table2[read2].type;
		read2++;
		write++;
	}
}