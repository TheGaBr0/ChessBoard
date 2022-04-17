#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> 

#define GCOLUMNS 49
#define GROWS 33

#define XPIECE 0
#define YPIECE 1
#define XDESTINATION 2
#define YDESTINATION 3

#define DATA_SIZE 1000

#define MAXGAMES 20
#define MAXLENPERGAME 35
#define MAXLENPERWRITTENMOVE 10

#define BishopSwitch switch(rcdChecked){ \
			case 0: xAdder=1; yAdder=1; break; /*giu destra*/\
			case 1: xAdder=-1; yAdder=1; break; /*su destra*/\
			case 2: xAdder=1; yAdder=-1; break; /*giu sinistra*/\
			case 3: xAdder=-1; yAdder=-1; break; /*su sinistra*/\
		}

#define RookSwitch switch(rcdChecked){ \
			case 0: xAdder=1; yAdder=0; break; /*giu*/\
			case 1: xAdder=-1; yAdder=0; break; /*su*/\
			case 2: xAdder=0; yAdder=-1; break; /*sinistra*/\
			case 3: xAdder=0; yAdder=1; break; /*destra*/\
		}

#define KnightSwitch switch(rcdChecked){ \
			case 0: xAdder=2; yAdder=1; break; /*su destra*/\
			case 1: xAdder=2; yAdder=-1; break; /*su sinistra*/\
			case 2: xAdder=-2; yAdder=1; break; /*giu destra*/\
			case 3: xAdder=-2; yAdder=-1; break; /*giu sinistra*/\
			case 4: xAdder=1; yAdder=2; break; /*destra giu*/\
			case 5: xAdder=-1; yAdder=2; break; /*destra su*/\
			case 6: xAdder=1; yAdder=-2; break; /*sinistra giu*/\
			case 7: xAdder=-1; yAdder=-2; break; /*sinistra su*/\
		}
#define KingQueenSwitch switch(rcdChecked){ \
			case 0: xAdder=1; yAdder=0; break; /*giu*/\
			case 1: xAdder=-1; yAdder=0; break; /*su*/\
			case 2: xAdder=0; yAdder=-1; break; /*sinistra*/\
			case 3: xAdder=0; yAdder=1; break; /*destra*/\
			case 4: xAdder=1; yAdder=1; break; /*giu destra*/\
			case 5: xAdder=-1; yAdder=1; break; /*su destra*/\
			case 6: xAdder=1; yAdder=-1; break; /*giu sinistra*/\
			case 7: xAdder=-1; yAdder=-1; break; /*su sinistra*/\
		}

#pragma GCC diagnostic ignored "-Wwrite-strings"

typedef enum{

	EMPTY = 0, //0
	KING, //1
	QUEEN, //2
	ROOK, //3
	BISHOP, //4
	KNIGHT, //5
	PAWN, //6

}PIECES;

typedef enum{

	BLACK = 0,
	WHITE = 1,

}COLOURS;

typedef struct{
	int row;
	int column;
	
	PIECES pezzo;
	COLOURS colore;
	
	int pieceColor;
	
}Square;

typedef struct{
	Square squares[8][8];
}Board;

//Prototypes related to the chess engine
void reset();
void red();
void yellow();
int getGraphicX(int x);
int getGraphicY(int y);
void updatePieces(char graphicBoard[GROWS][GCOLUMNS]);
void showGraphicBoard(char graphicBoard[GROWS][GCOLUMNS]);
void showDebugBoards(int x, int y);
void initializeGraphicBoard(char graphicBoard[GROWS][GCOLUMNS]);
void initializeBoard();
void inputInterface(bool whiteTurn, bool drawRequest[]);
void showInvalidMoveWarning(int index);
int convertCharCoords(char ch);
int convertIntCoords(int n);
void deleteStoredPossibleMoves();
void deleteStoredCheckMoves();
int getNumberOfMoves();
int getNumberOfCheckMoves();
void addPossibleMove(int x, int y);
void addPossibleCheckMove(int x, int y);
bool isUnderCheckAt(int x, int y, int pieceColor, bool recalculate);
void findKing(int*x, int*y, int pieceColor);
bool isKingUnderCheck(int pieceColor);
bool isKingUnderCheckAfterMove(int moveCoordsData[], PIECES pieceToBeMovedTemp, int playerColorTurn);
void pawnCalculator(int x, int y, int pieceColor);
void pieceCalculator(int x, int y, int pieceNeeded, int pieceColor, bool check);
void calculateCheckSquares(int pieceColor);
int pieceReverseFinder(int pieceColor, int pieceNeeded, int moveCoordsData[]);
int pieceFinder(int pattern, char currentPosition[], char nextMove[], int moveCoordsData[], PIECES pieceToBeMoved, int playerColorTurn);
int pieceMover(int moveCoordsData[], int playerColorTurn, PIECES pieceToBeMoved);
bool castlingChecker(bool castlingCheck[], char castlingType, int playerColorTurn);
int castlingMover(bool castlingCheck[], char castlingType, int playerColorTurn);
void castlingHandler(int yPiece, PIECES pieceToBeMoved, int pieceColor, bool castlingCheck[]);
void promotionHandler(int x, int y,  char move[], char pieceToPromove, int pieceColor);
bool acceptableChar(char value);
int isMoveValid(char move[]);
void getInput(char *inputBuffer, int bufferLength);
void getFirstPart(char *firstPart, char move[]);
void getSecondPart(char *secondPart, char move[]);
int analyzeInput(char move[], bool castlingCheck[], int playerColorTurn, PIECES *pieceToBeMoved, char *pieceToPromove, char *playerInteraction, int moveCoordsData[]);
bool noPieceCanPreventCheckmate(int pieceColor);
int checkEndOfGame(int pieceColor);
int playerInteractionHandler(bool drawRequest[], char playerInteraction, bool whiteTurn);
void chessEngine();
//*** Prototypes related to file writing ***
void fileNameGenerator(char fileName[]);
void writeMove(char fileName[], char move[]);

//Prototypes related to the game viewer
void printGameViewerInterface(char gameMoves[][MAXLENPERWRITTENMOVE], int moveNumber, bool whiteTurn, int endOfGame);
int loadGame(FILE *file, Board gameStates[], char gameMoves[][MAXLENPERWRITTENMOVE], int* endOfGame);
void gameViewer(char gamesList[][MAXLENPERGAME], int index);
int getNumberOfGames(char gamesList[][MAXLENPERGAME]);
void initializeGamesList(char gamesList[][MAXLENPERGAME]);
void cloneArray(char clone[][35], char original[][MAXLENPERGAME]);
void getFilesInGamesFolder(char gamesList[][MAXLENPERGAME]);
void printInterfaceGameViewer(int choice, char gamesList[][MAXLENPERGAME], char gamesListClone[][MAXLENPERGAME]);
void handleInputGameViewer();
void handleInput();

//Prototypes related to thje starting interface
void printInterface(int choice);
void handleInput();

Board board;

int possibleMoves[32][2];
int possibleCheckMoves[64][2];

void reset () {
  printf("\033[0m");
}

void red () {
  printf("\033[1;31m");
}

void yellow () {
  printf("\033[2;33m");
}

int getGraphicX(int x){
	int i, k = 0;
	for(i=2; i<GROWS; i+=4)
	{
		if(k==x)
			return i;
		k++;
	}
	
}

int getGraphicY(int y){
	int j, l = 0;
	for(j=3; j<GCOLUMNS; j+=6)
	{  
		if(l==y)
			return j;
		l++;
	}
}

void updatePieces(char graphicBoard[GROWS][GCOLUMNS]){
	
	int i, j;
	char pieceColor;
	
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			if(board.squares[i][j].pieceColor == WHITE)
				pieceColor = 'w';	
			else
				pieceColor = 'b';
				
			switch(board.squares[i][j].pezzo){
				case PAWN: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'P'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case BISHOP: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'B'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case KNIGHT: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'N'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case ROOK: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'R'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case QUEEN: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'Q'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case KING: graphicBoard[getGraphicX(i)][getGraphicY(j)] = 'K'; graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = pieceColor; 
					break;
				case EMPTY:
						if(board.squares[i][j].colore == BLACK){
							graphicBoard[getGraphicX(i)][getGraphicY(j)] = ' ';
							graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = ' ';
						}
						else{
								graphicBoard[getGraphicX(i)][getGraphicY(j)] = 178; //white rectangle
								graphicBoard[getGraphicX(i)][getGraphicY(j)+1] = 178; //white rectangle
						}
				}
			}
		}
	}
 
void showGraphicBoard(char graphicBoard[GROWS][GCOLUMNS]){
	int i, j;
	updatePieces(graphicBoard);
	
	for(i=0; i<GROWS; i++){
		for(j=0; j<GCOLUMNS; j++){
			printf("%c", graphicBoard[i][j]);
		}
		printf("\n");
	}
	
}

void showDebugBoards(int x, int y){
	bool occupato, pezzo;
	int i,j,k;
	
		for(i = 0; i < 8; i++){
			for(j = 0; j < 8; j++){
				occupato = false;
				pezzo = false;
				for(k = 0; k<getNumberOfMoves()+1; k++)
				{
					if(i == possibleMoves[k][0] && j == possibleMoves[k][1]){
							occupato = true;
					}
					if(x == i && y == j){
						pezzo = true;
					}
				}
				
				if(occupato){
					printf("[*]");
				}else{
					if(pezzo){
						printf("[x]");
					}else{
						printf("[ ]");
					}
						
				}
			}
			
			printf("\t");
			for(j = 0; j < 8; j++){
				occupato = false;
				pezzo = false;
				for(k = 0; k<getNumberOfCheckMoves()+1; k++)
				{
					if(i == possibleCheckMoves[k][0] && j == possibleCheckMoves[k][1]){
							occupato = true;
					}
					if(x == i && y == j){
							pezzo = true;
					}
				}
				
				if(occupato){
					printf("[*]");
				}else{
					if(pezzo){
						printf("[x]");
					}else{
						printf("[ ]");
					}
						
				}
			}
			printf("\n");
		}
}

void initializeGraphicBoard(char graphicBoard[GROWS][GCOLUMNS]){
	int i, j, k, l, countColumns=0, rowsCounter = 0, intValue;
	char charValue;
	bool white = true;
	
	//initialization 
	for(i=0; i<GROWS; i++)
	{
		for(j=0; j<GCOLUMNS; j++)
		{
		   graphicBoard[i][j] = ' ';
		}
	}
	
	//sides
	for(i=0; i<GCOLUMNS; i++)
		graphicBoard[0][i] = 196; //horiz
		
	for(i=0; i<GCOLUMNS; i++)
		graphicBoard[GROWS-1][i] = 196; //horiz
	
	for(i=0; i<GROWS; i++)
		graphicBoard[i][0] = 179; //vert
		
	for(i=0; i<GROWS; i++)
		graphicBoard[i][GCOLUMNS-1] = 179; //vert
	
	//corners
	graphicBoard[0][0] = 218;
	graphicBoard[GROWS-1][0] = 192;
	graphicBoard[0][GCOLUMNS-1] = 191;
	graphicBoard[GROWS-1][GCOLUMNS-1] = 217;	

	//intersections
	
	for(i=6; i<GCOLUMNS-1; i+=6) //up and down sides
	{
		graphicBoard[0][i] = 194;
		graphicBoard[GROWS-1][i] = 193;
		for(j=1; j<GROWS-1; j++)
			graphicBoard[j][i] = 179; 
	}
	
	
	
	for(i=4; i<GROWS-1; i+=4) //left and right sides
	{
		graphicBoard[i][0] = 195;
		graphicBoard[i][GCOLUMNS-1] = 180;
		for(j=1; j<GCOLUMNS-1; j++)
			graphicBoard[i][j] = 196;
	}
	
	for(i=4; i<GROWS-1; i+=4) //left and right sides
	{
		for(j=6; j<GCOLUMNS-1; j+=6)
			graphicBoard[i][j] = 197;
	}
	
	//paint white squares
	for(i=1; i<GROWS; i++)
	{		
		rowsCounter++;
			for(j=0; j<GCOLUMNS; j++)
			{  
				if(graphicBoard[i][j] == ' '){
					if(countColumns <=  4){
						graphicBoard[i][j] = 178;
					}
					
					countColumns++;
					
					if(countColumns > 9)
						countColumns=0;
				}
			}
			
		if(rowsCounter == 4){
			white = !white;
			rowsCounter = 0;
			countColumns = 0;
		}
		
		if(white == false){
			countColumns=5;
		}
	}
	
	//Initialize identifiers for rows and columns
	
	charValue = 'a';
	intValue = 1;
	
	for(i = 1; i<GCOLUMNS; i+=6){
		graphicBoard[31][i] = charValue;
		
		charValue += 1;
	}
	
	for(i = 29; i>0; i-=4){
		
		graphicBoard[i][47] = intValue + '0'; 
		
		intValue += 1;
	}
}

void initializeBoard(){
	int i, j, k, l, pawnIndex = 0;
	bool changeColor = true;
	
	//Identify colours in squares
	changeColor = false;
	k=0;
	for(i=2; i<GROWS; i+=4)
	{
		l=0;
		for(j=3; j<GCOLUMNS; j+=6)
		{
		   board.squares[k][l].row = i;
		   board.squares[k][l].column = j;
		   
		   if(changeColor){
			if(l%2 == 0){
			   		board.squares[k][l].colore = BLACK;
			   }else{
			   		board.squares[k][l].colore = WHITE;
			   }
		   }else{
		   	if(l%2 != 0){
			   		board.squares[k][l].colore = BLACK;
			   }else{
			   		board.squares[k][l].colore = WHITE;
			   }
		   }
		   
		   
		   l++;
		}
		k++;
		if(changeColor)
		   	changeColor = false;
		else
		   	changeColor = true;
		   	
		}
	
	for(i = 0; i<8; i++){
		for(j=0; j<8; j++){
			board.squares[i][j].pezzo = EMPTY;
		}
	}
	
	//PIECES initialization
	
	//PAWNS
	
	for(i = 0; i<8; i++){
		board.squares[1][pawnIndex].pezzo = PAWN;
		board.squares[1][pawnIndex].pieceColor = BLACK;
		
		board.squares[6][pawnIndex].pezzo = PAWN;
		board.squares[6][pawnIndex].pieceColor = WHITE;
		pawnIndex += 1;
	}

	//ROOKS
	board.squares[0][0].pezzo = ROOK;
	board.squares[0][0].pieceColor = BLACK;
	
	board.squares[0][7].pezzo = ROOK;
	board.squares[0][7].pieceColor = BLACK;
	
	board.squares[7][0].pezzo = ROOK;
	board.squares[7][0].pieceColor = WHITE;
	
	board.squares[7][7].pezzo = ROOK;
	board.squares[7][7].pieceColor = WHITE;
	
	//KNIGHTS
	board.squares[0][1].pezzo = KNIGHT;
	board.squares[0][1].pieceColor = BLACK;
	
	board.squares[0][6].pezzo = KNIGHT;
	board.squares[0][6].pieceColor = BLACK;
	
	board.squares[7][1].pezzo = KNIGHT;
	board.squares[7][1].pieceColor = WHITE;
	
	board.squares[7][6].pezzo = KNIGHT;
	board.squares[7][6].pieceColor = WHITE;
	
	//BISHOPS
	board.squares[0][2].pezzo = BISHOP;
	board.squares[0][2].pieceColor = BLACK;
	
	board.squares[0][5].pezzo = BISHOP;
	board.squares[0][5].pieceColor = BLACK;
	
	board.squares[7][2].pezzo = BISHOP;
	board.squares[7][2].pieceColor = WHITE;
	
	board.squares[7][5].pezzo = BISHOP;
	board.squares[7][5].pieceColor = WHITE;
	
	//QUEENS
	board.squares[0][3].pezzo = QUEEN;
	board.squares[0][3].pieceColor = BLACK;
	
	board.squares[7][3].pezzo = QUEEN;
	board.squares[7][3].pieceColor = WHITE;
	
	//KINGS
	board.squares[0][4].pezzo = KING;
	board.squares[0][4].pieceColor = BLACK;
	
	board.squares[7][4].pezzo = KING;
	board.squares[7][4].pieceColor = WHITE;
	
}

void inputInterface(bool whiteTurn, bool drawRequest[]){
	if(whiteTurn == WHITE){
		yellow();
		if(drawRequest[0]){
			printf("\nBlack player has offered a draw, digit [accept] or [deny]");
			printf("\nwhenever you want for accepting or denying.\n");
		}else{
			if(drawRequest[1])
				printf("\nDraw offer pending.");
		}
		reset();
		
		printf("\nIt's WHITE turn, digit your move --> ");
	}else{
		yellow();
		if(drawRequest[1]){
			printf("\nBLACK player has offered a draw, digit [accept] or [deny]");
			printf("\nwhenever you want for accepting or denying.\n");
		}else{
			if(drawRequest[0])
				printf("\nDraw offer pending.");
		}
		reset();
		printf("\nIt's BLACK turn, digit your move --> ");
	}
}

void showInvalidMoveWarning(int index){ 
	red();
	switch(index){
		case 0: printf("Invalid move!\n"); break;
		case 1: printf("Castling is not possible!\n"); break;
		case 2: printf("The wanted piece cannot reach that position! [pieceFinder] \n"); break;
		case 3: printf("The wanted piece cannot reach that position! [pieceMover] \n"); break;
		case 4: printf("The king would be under check after this move!\n"); break;
		case 5: printf("The king is under check!\n"); break;
		case 6: printf("Many pieces of the same type can reach that position, please specify which one you want!\n");
		case 7: printf("The wanted piece cannot reach that position!! []pieceReverseFinder \n"); break;
		default: break;
	}
	reset();
}

int convertCharCoords(char ch){
	switch(ch){
		case 'a': return 0;
		case 'b': return 1;
		case 'c': return 2;
		case 'd': return 3;
		case 'e': return 4;
		case 'f': return 5;
		case 'g': return 6;
		case 'h': return 7;
	}
}

int convertIntCoords(int n){
	switch(n){
		case 1: return 7;
		case 2: return 6;
		case 3: return 5;
		case 4: return 4;
		case 5: return 3;
		case 6: return 2;
		case 7: return 1;
		case 8: return 0;
	}
}

void deleteStoredPossibleMoves(){
	int i, j;
	for(i = 0; i<32; i++)
	{
		for(j=0; j<2; j++) 
			possibleMoves[i][j]= -1;
	}
}

void deleteStoredCheckMoves(){  
	int i, j;
	for(i = 0; i<64; i++)
	{
		for(j=0; j<2; j++) 
			possibleCheckMoves[i][j]= -1;
	}
}

int getNumberOfMoves(){
	int i, j, count = 0;
	for(i=0; i<32; i++){
		if(possibleMoves[i][0] != -1)
			count++;
	}
	return count;
}

int getNumberOfCheckMoves(){
	int i, j, count = 0;
	for(i=0; i<64; i++){
		if(possibleCheckMoves[i][0] != -1)
			count++;
	}
	return count;
}

void addPossibleMove(int x, int y){ 
	
	int count = getNumberOfMoves();
	
	possibleMoves[count][0] = x;
	possibleMoves[count][1] = y;
	 
}

void addPossibleCheckMove(int x, int y){

	int i, count = getNumberOfCheckMoves();
	bool found = false;
	
	for(i = 0; i<getNumberOfCheckMoves()+1; i++){ //Check if coords are already stored
		if(x == possibleCheckMoves[i][0]){
			if(y == possibleCheckMoves[i][1]){
				found = true;
			}
		}
	}
	
	if(!found){
		possibleCheckMoves[count][0] = x;
		possibleCheckMoves[count][1] = y;
	}
}

bool isUnderCheckAt(int x, int y, int pieceColor, bool recalculate){  

	int i;	
	
	if(recalculate){ //Recalculation of positions where the pieceColor king cannot go
		deleteStoredCheckMoves();
		calculateCheckSquares(!pieceColor); //The colour is inverted, so i can check if enemy pieces can check the pieceColor king 
	}	
	
	for(i = 0; i<getNumberOfCheckMoves()+1; i++){
		if(x == possibleCheckMoves[i][0]){
			if(y == possibleCheckMoves[i][1]){
				return true;
			}
		}
	}
	
	return false;
	 
}

void findKing(int*x, int*y, int pieceColor){ 
	
	bool found = false;
	int i = 0, j = 0;
	
	while(!found){
		j=0;
		while(!found && j<8){
			if(board.squares[i][j].pezzo == KING && board.squares[i][j].pieceColor == pieceColor){
				found = true;
				*x = i;
				*y = j;
			}
			j++;	
		}
		i++;
	}
}

bool isKingUnderCheck(int pieceColor){
	
	int x, y;
	
	findKing(&x, &y, pieceColor);
	
	return	isUnderCheckAt(x, y, pieceColor, true);
}

bool isKingUnderCheckAfterMove(int moveCoordsData[], PIECES pieceToBeMovedTemp, int playerColorTurn){ 
	
	Square tempBoard[8][8];
	bool underCheck = false;
		
	memcpy(tempBoard, board.squares, 8*8*sizeof(Square));
	
	pieceMover(moveCoordsData, playerColorTurn, pieceToBeMovedTemp);

	if(isKingUnderCheck(playerColorTurn))
		underCheck = true;
		
	memcpy(board.squares, tempBoard, 8*8*sizeof(Square));
	
	return underCheck;
}

void pawnCalculator(int x, int y, int pieceColor){
	
	deleteStoredPossibleMoves();

	if(pieceColor == WHITE){ //If white is playing
		if(board.squares[x-1][y].pezzo == EMPTY && x != 0){ //I check if the pawn can go forward 
			if(x == 6 && pieceColor == WHITE){ //I check if its starting position hasn't changed since initialization
				addPossibleMove(x-1, y);
				addPossibleMove(x-2, y);
			}else{
				addPossibleMove(x-1, y);
			}
		}
		
		if(y != 0){ //I check if the pawn is located adjacent to the left side
			if(x != 0){ //I check if the pawn is located adjacent to the upper side
				addPossibleCheckMove(x-1, y-1);
				if(board.squares[x-1][y-1].pezzo != EMPTY && board.squares[x-1][y-1].pieceColor == BLACK)  //I check if the pawn can eat another piece diagonally
					addPossibleMove(x-1, y-1);
			}
		}
		
		if(y != 7){ //I check if the pawn is located adjacent to the right side
			if(x != 0){ //I check if the pawn is located adjacent to the lower side
				addPossibleCheckMove(x-1, y+1);
				if(board.squares[x-1][y+1].pezzo != EMPTY && board.squares[x-1][y+1].pieceColor == BLACK)  //I check if the pawn can eat another piece diagonally
					addPossibleMove(x-1, y+1);
			}
		}
	}else{ //If black is playing, checks are the same but mirrored
		if(board.squares[x+1][y].pezzo == EMPTY && x != 7){ 
			if(x == 1 && pieceColor == BLACK){ 
				addPossibleMove(x+1, y);
				addPossibleMove(x+2, y);
			}else{
				addPossibleMove(x+1, y);
			}
		}
		
		if(y != 0){
			if(x != 7){
				addPossibleCheckMove(x+1, y-1);
				if(board.squares[x+1][y-1].pezzo != EMPTY && board.squares[x+1][y-1].pieceColor == WHITE)
					addPossibleMove(x+1, y-1);
			}
		}
		
		if(y != 7){
			if(x != 7){
				addPossibleCheckMove(x+1, y+1);
				if(board.squares[x+1][y+1].pezzo != EMPTY && board.squares[x+1][y+1].pieceColor == WHITE)
					addPossibleMove(x+1, y+1);
			}
		}
	}
}

void pieceCalculator(int x, int y, int pieceNeeded, int pieceColor, bool check){
	int rcdChecked = 0, maxrcdChecked = 4, startX, startY, xAdder, yAdder;
	bool border, piece, skip;
	
	deleteStoredPossibleMoves();
	
	if(check){ //if the king is being moved
		deleteStoredCheckMoves();
		calculateCheckSquares(!pieceColor);
	}

	while(rcdChecked != maxrcdChecked){
		
		border = false;
		piece = false;
		
		startX = x;
		startY = y;

		switch(pieceNeeded){
			case BISHOP: BishopSwitch; break; 
			case KNIGHT: maxrcdChecked = 8; KnightSwitch; break;
			case ROOK: RookSwitch; break;
			case QUEEN:
			case KING: KingQueenSwitch; maxrcdChecked = 8; break;
		}
		
		while(!border && !piece){
			
			startX+=xAdder;
			startY+=yAdder;
			
			if((startX > -1 && startX < 8) && (startY > -1 && startY < 8)){ // If i don't find borders
			
				if(board.squares[startX][startY].pezzo == EMPTY){ //If i don't find PIECES
					if(pieceNeeded == KING){ 
						if(!isUnderCheckAt(startX, startY, pieceColor, false))
							addPossibleMove(startX, startY);
					}else
						addPossibleMove(startX, startY);	
				}else{
					if(board.squares[startX][startY].pieceColor != pieceColor){//If i'm checking for check moves, the even this piece is added cause it's protected 
						if(pieceNeeded == KING){ 
							if(!isUnderCheckAt(startX, startY, pieceColor, false))
								addPossibleMove(startX, startY);
						}else
							addPossibleMove(startX, startY);
					}else{
						if(!check) //this check avoids that the pieceColor king is taken in consideration  
							addPossibleCheckMove(startX, startY); 
					}
				
				 	if(pieceNeeded != KNIGHT){ //this prevents the pieceColor king from going under check if moving backward on a row/diagonal/column which is entirely controlled by an enemy piece
						if(board.squares[startX][startY].pezzo != KING){
							piece = true;
						}else{
							if(board.squares[startX][startY].pieceColor == pieceColor)
								piece = true;
						}	
					}
				}
				
				if(pieceNeeded == KNIGHT || pieceNeeded == KING)//this is needed for stopping kings and knights from looping over unneeded squares
					break;	
					
			}else{
				border = true;
			}
		}
		
		rcdChecked++;
	}

}

void calculateCheckSquares(int pieceColor){
	int i, j, k, possibleMovesClone[32][2];
	
	memcpy(possibleMovesClone, possibleMoves, 32*2*sizeof(int)); //This is needed in order to preserve the stored moves, look at line 887
	
	for(i = 0; i < 8; i++){
			for(j = 0; j < 8; j++){
				if(board.squares[i][j].pezzo != EMPTY && board.squares[i][j].pieceColor == pieceColor){
					
					if(board.squares[i][j].pezzo == PAWN)
						pawnCalculator(i, j, board.squares[i][j].pieceColor);
					else{
						pieceCalculator(i, j, board.squares[i][j].pezzo, board.squares[i][j].pieceColor, false);
						for(k=0; k<getNumberOfMoves()+1; k++){
							if(possibleMoves[k][0] != -1){
								addPossibleCheckMove(possibleMoves[k][0], possibleMoves[k][1]);
							}
						}
					}
				}
			}
		}
		
	memcpy(possibleMoves, possibleMovesClone, 32*2*sizeof(int));
}

int pieceReverseFinder(int pieceColor, int pieceNeeded, int moveCoordsData[]){
	
	int i = 0, rcdChecked = 0, startX, startY, xAdder, yAdder, foundX, foundY, piecesFound = 0, maxrcdChecked = 4;
	bool skip, found;
	
	deleteStoredPossibleMoves();
	
	while(rcdChecked != maxrcdChecked){
		
		skip = false;
		found = false;
		
		startX = moveCoordsData[XDESTINATION];
		startY = moveCoordsData[YDESTINATION];
		
		switch(pieceNeeded){
			case BISHOP: BishopSwitch; break; 
			case KNIGHT: maxrcdChecked = 8; KnightSwitch; break;
			case ROOK: RookSwitch; break;
			case QUEEN:
			case KING: KingQueenSwitch; maxrcdChecked = 8; break;
		}
		
		while(!skip && !found){
			
			startX+=xAdder;
			startY+=yAdder;
			
			if((startX > -1 && startX < 8) && (startY > -1 && startY < 8)){ //If i don't find borders
				if(board.squares[startX][startY].pezzo == EMPTY){ //If i don't find PIECES
					
					if(pieceNeeded == KNIGHT || pieceNeeded == KING)
						skip = true;
					
				}else{
					if(board.squares[startX][startY].pezzo == pieceNeeded && board.squares[startX][startY].pieceColor == pieceColor){ //se mi trovo su un pezzo giusto
						foundX = startX;
						foundY = startY;
						found = true;
						piecesFound++;
					}else{
						skip = true;
					}
				}
			}else{
				skip = true;
			}
		}
		
		rcdChecked++;
	}
	
	switch(piecesFound){
		case 0: return 7;
		case 1: moveCoordsData[XPIECE] = foundX;
				moveCoordsData[YPIECE] = foundY;
				return -1;
			
		default: if(moveCoordsData[YPIECE] == -2) //if the move doesn't specify which piece, then i return an error
					return 6;
				 
				while(i<8){ //I look for pawns on moveCoordsData[YPIECE] and i select the first one i find 
					if(board.squares[i][moveCoordsData[YPIECE]].pezzo == pieceNeeded && board.squares[i][moveCoordsData[YPIECE]].pieceColor == pieceColor){ //se trovo il pezzo
							moveCoordsData[XPIECE] = i;
							return -1;
					}
					i++; 
				}	
	}
	
}

int pieceFinder(int pattern, char currentPosition[], char nextMove[], int moveCoordsData[], PIECES pieceToBeMoved, int playerColorTurn){
	
	int letterIndex, i=0, j;
	
	switch(pattern){
		case 3: moveCoordsData[YDESTINATION] = convertCharCoords(currentPosition[0]); //[e4]
				moveCoordsData[XDESTINATION] = convertIntCoords(currentPosition[1] - '0');
			break;
		case 4: moveCoordsData[YDESTINATION] = convertCharCoords(currentPosition[1]); //[Bb4]
				moveCoordsData[XDESTINATION] = convertIntCoords(currentPosition[2] - '0');
			break;
		case 5:
				moveCoordsData[YDESTINATION] = convertCharCoords(nextMove[0]); //[Bxd4] 
				moveCoordsData[XDESTINATION] = convertIntCoords(nextMove[1] - '0');
			break;
		case 6: //[e4xd5]
		case 7: moveCoordsData[XPIECE] = convertIntCoords(currentPosition[2] - '0'); //[Be4xd5]
				moveCoordsData[YPIECE] = convertCharCoords(currentPosition[1]);
				
				if(board.squares[moveCoordsData[XPIECE]][moveCoordsData[YPIECE]].pezzo == pieceToBeMoved && board.squares[moveCoordsData[XPIECE]][moveCoordsData[YPIECE]].pieceColor == playerColorTurn){
					moveCoordsData[YDESTINATION] = convertCharCoords(nextMove[0]);
					moveCoordsData[XDESTINATION] = convertIntCoords(nextMove[1] - '0');
					return -1;
					
				}
			break;
		case 8:	moveCoordsData[YDESTINATION] = convertCharCoords(nextMove[0]); //[Rac1]
				moveCoordsData[XDESTINATION] = convertIntCoords(nextMove[1] - '0');
				moveCoordsData[YPIECE] = convertCharCoords(currentPosition[1]); 
			break;
	}
		
	if(pattern <= 5 || pattern == 8){ //In case of patterns 1,2,3,8 i only have destination coords, i need to find the starting coords.
		if(pieceToBeMoved != PAWN)
			return pieceReverseFinder(playerColorTurn, pieceToBeMoved, moveCoordsData); 
		else{
			letterIndex = convertCharCoords(currentPosition[0]);
			
			while(i<8){ //I look for the pawn on the letterIndex column
				if(board.squares[i][letterIndex].pezzo == PAWN && board.squares[i][letterIndex].pieceColor == playerColorTurn){ 
					
					pawnCalculator(i, letterIndex, playerColorTurn); 
					
					for(j = 0; j<getNumberOfMoves(); j++){ 
						if(moveCoordsData[XDESTINATION] == possibleMoves[j][0]){
							if(moveCoordsData[YDESTINATION] == possibleMoves[j][1]){
								moveCoordsData[XPIECE] = i;
								moveCoordsData[YPIECE] = letterIndex;
								
								return -1; 
							}
						}
					}
					i++; 
				}
				else
					i++;
			}	
		}
	}
		
	return 2;
}

int pieceMover(int moveCoordsData[], int playerColorTurn, PIECES pieceToBeMoved){
	int i, j;
	bool check;	
	
	check = pieceToBeMoved == KING;

	if(pieceToBeMoved != PAWN)
		pieceCalculator(moveCoordsData[XPIECE], moveCoordsData[YPIECE], pieceToBeMoved, playerColorTurn, check);
	else
		pawnCalculator(moveCoordsData[XPIECE], moveCoordsData[YPIECE], playerColorTurn);
	
	for(i = 0; i<getNumberOfMoves(); i++){
		
		if(moveCoordsData[XDESTINATION] == possibleMoves[i][0]){
			if(moveCoordsData[YDESTINATION] == possibleMoves[i][1]){
				board.squares[possibleMoves[i][0]][possibleMoves[i][1]].pezzo = pieceToBeMoved;
				board.squares[possibleMoves[i][0]][possibleMoves[i][1]].pieceColor = playerColorTurn;
				board.squares[moveCoordsData[XPIECE]][moveCoordsData[YPIECE]].pezzo = EMPTY;
				
				return -1;
			}
		}
	}
	
	return 3;
}

bool castlingChecker(bool castlingCheck[], char castlingType, int playerColorTurn){
	
	if(playerColorTurn == WHITE){ 
		if(castlingCheck[0] == false){ //If the king hasn't moved
			if(castlingType == 's' && castlingCheck[4] == true) //if short castling is selected and right white tower hasn't moved
				return false;
			if(castlingType == 'l' && castlingCheck[2] == true) //if long castling is selected and left white tower hasn't moved
				return false;
		}else{
			return false;
		}	
	}else{
		if(castlingCheck[1]  == false){ 
			if(castlingType == 's' && castlingCheck[5] == true) //if short castling is selected and right black tower hasn't moved
				return false;
			if(castlingType == 'l' && castlingCheck[3] == true) //if long castling is selected and left black tower hasn't moved
				return false;
		}else{
			return false;
		}	
	}
	
	//I need to verify that between the king and the tower there are empty squares
	
	if(castlingType == 's'){ 
		if(playerColorTurn == WHITE){
			if(board.squares[7][5].pezzo != EMPTY || board.squares[7][6].pezzo != EMPTY)
				return false;
		}else{
			if(board.squares[0][5].pezzo != EMPTY || board.squares[0][6].pezzo != EMPTY)
				return false;
		}
		
	}else{ 
		if(playerColorTurn == WHITE){
			if(board.squares[7][1].pezzo != EMPTY || board.squares[7][2].pezzo != EMPTY || board.squares[7][3].pezzo != EMPTY)
				return false;
		}else{
			if(board.squares[0][1].pezzo != EMPTY || board.squares[0][2].pezzo != EMPTY || board.squares[0][3].pezzo != EMPTY)
				return false;
		}
	}
	
	//I need to verify that the king isn't currently under check
	
	if(playerColorTurn == WHITE){//se tocca al WHITE
		if(isUnderCheckAt(7, 4, playerColorTurn, true))
			return false;
	}else{
		if(isUnderCheckAt(0, 4, playerColorTurn, true))
			return false;
	}
		
	//I need to verify that the king won't be under check after castling
	if(castlingType == 's'){ 
		if(playerColorTurn == WHITE){
			if(isUnderCheckAt(7, 6, playerColorTurn, true))
				return false;
		}else{
			if(isUnderCheckAt(0, 6, playerColorTurn, true))
				return false;
		}
		
	}else{ 
		if(playerColorTurn == WHITE){
			if(isUnderCheckAt(7, 1, playerColorTurn, true))
				return false;
		}else{
			if(isUnderCheckAt(0, 1, playerColorTurn, true))
				return false;
		}
	}

	return true;
}

int castlingMover(bool castlingCheck[], char castlingType, int playerColorTurn){
	
	if(castlingChecker(castlingCheck, castlingType, playerColorTurn)){
		
		if(castlingType == 's'){
			if(playerColorTurn == WHITE){
				board.squares[7][4].pezzo = EMPTY;
				board.squares[7][7].pezzo = EMPTY;
				board.squares[7][6].pezzo = KING;
				board.squares[7][5].pezzo = ROOK;
		
				board.squares[7][6].pieceColor = playerColorTurn;
				board.squares[7][5].pieceColor = playerColorTurn;
			
			}else{
				board.squares[0][4].pezzo = EMPTY;
				board.squares[0][7].pezzo = EMPTY;
				board.squares[0][6].pezzo = KING;
				board.squares[0][5].pezzo = ROOK;
		
				board.squares[0][6].pieceColor = playerColorTurn;
				board.squares[0][5].pieceColor = playerColorTurn;
			}
		
		}else{ 
			if(playerColorTurn == WHITE){
				board.squares[7][4].pezzo = EMPTY;
				board.squares[7][0].pezzo = EMPTY;
				board.squares[7][1].pezzo = KING;
				board.squares[7][2].pezzo = ROOK;
		
				board.squares[7][1].pieceColor = playerColorTurn;
				board.squares[7][2].pieceColor = playerColorTurn;
			}else{
				board.squares[0][4].pezzo = EMPTY;
				board.squares[0][0].pezzo = EMPTY;
				board.squares[0][1].pezzo = KING;
				board.squares[0][2].pezzo = ROOK;
		
				board.squares[0][1].pieceColor = playerColorTurn;
				board.squares[0][2].pieceColor = playerColorTurn;
			}
				
		}
		
		return -1;
	}
	return 1;
}

void castlingHandler(int yPiece, PIECES pieceToBeMoved, int pieceColor, bool castlingCheck[]){
	
	if(pieceToBeMoved == KING){
		if(pieceColor == WHITE)
			castlingCheck[0] = true; //wKingHasMoved
		else
			castlingCheck[1] = true; //bKingHasMoved
	}else{
		if(pieceToBeMoved == ROOK){
			if(yPiece == 0){
				if(pieceColor == WHITE)
					castlingCheck[2] = true; //wLeftRook
				else
					castlingCheck[3] = true; //bLeftRook
			}else{
				if(pieceColor == WHITE)
					castlingCheck[4] = true; //wRightRook
				else
					castlingCheck[5] = true; //bRightRook
			}	
		}
	}
}

void promotionHandler(int x, int y,  char move[], char pieceToPromove, int pieceColor){  
	
	char choice[9];
	
	char bishop[7] = {'b', 'i', 's', 'h', 'o', 'p', '\0'};
	char queen[6] = {'q', 'u', 'e', 'e', 'n', '\0'};
	char rook[5] = {'r', 'o', 'o', 'k', '\0'};
	char knight[7] = {'k', 'n', 'i', 'g', 'h', 't', '\0'};
	
	int i;
	
	bool acceptable = false;
	PIECES wantedPiece;
	
	if((pieceColor == WHITE && x == 0) || (pieceColor == BLACK && x == 7)){
			
			if(pieceToPromove != 0){
				switch(pieceToPromove){
					case 'R': wantedPiece = ROOK;
							strncat(move, "=R", 2);
						break;
					case 'B': wantedPiece = BISHOP;
							strncat(move, "=B", 2);
						break;
					case 'N': wantedPiece = KNIGHT;
							strncat(move, "=N", 2);
						break;
					case 'Q': wantedPiece = QUEEN;
							strncat(move, "=Q", 2);
				}
			}else{
				printf("[Queen] [Knight] [Rook] [Bishop]");
				do{
					printf("\nWhich piece do you want to promote the pawn to?: ");
		  				
					getInput(choice, 9);
					
					for(i = 0; choice[i]; i++){
		  				choice[i] = tolower(choice[i]);
		  				
			  			if(strcmp(choice, bishop) == 0){
			  				wantedPiece = BISHOP;
			  				strncat(move, "=B", 2);
			  				acceptable = true;
						}
			  			
			  			if(strcmp(choice, queen) == 0){
			  				wantedPiece = QUEEN;
			  				strncat(move, "=Q", 2);
			  				acceptable = true;
						}
			  		
			  			if(strcmp(choice, rook) == 0){
			  				wantedPiece = ROOK;
			  				strncat(move, "=R", 2);
			  				acceptable = true;
						}
		
			  			if(strcmp(choice, knight) == 0){
			  				wantedPiece = KNIGHT;
			  				strncat(move, "=N", 2);
			  				acceptable = true;
						}
			  					
			  			printf("\r                              ");
					}
				}while(!acceptable);
			}
			board.squares[x][y].pezzo = wantedPiece;
			
			if(pieceColor == WHITE)
				board.squares[x][y].pieceColor = WHITE;
			else
				board.squares[x][y].pieceColor = BLACK;
	}		
}

bool acceptableChar(char value){
	char acceptableChars[13] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'K', 'Q', 'R', 'B', 'N'};
	int i = 0;
	while(i<strlen(acceptableChars)){
		
		if(acceptableChars[i] == value)
			return true;
		else
			i++;
	}
	
	return false;
}

int isMoveValid(char move[]){ 
	int i;
	char longCastling[6] = {'o', '-', 'o', '-', 'o', '\0'};
	char shortCastling[4] = {'o', '-', 'o', '\0'};
	char drawInteraction[5] = {'d', 'r', 'a', 'w', '\0'};
	char resign[7] = {'r', 'e', 's', 'i', 'g', 'n', '\0'};
	char accept[7] = {'a', 'c', 'c', 'e', 'p', 't', '\0'};
	char deny[5] = {'d', 'e', 'n', 'y', '\0'};
	char blackWon[10] = {'b', 'l', 'a', 'c', 'k', '_', 'w', 'o','n','\0'};
	char whiteWon[10] = {'w', 'h', 'i', 't', 'e', '_', 'w', 'o','n','\0'};
	char draw[6] = {'d', 'r', 'a', 'w', '_' ,'\0'};
	char choice[MAXLENPERWRITTENMOVE];
	
	for(i = 0; i<strlen(move); i++)
		choice[i] = tolower(move[i]);
		
	choice[i] = '\0';
	
	if(strcmp(choice, draw) == 0)
		return -8;
	
	if(strcmp(choice, whiteWon) == 0)
		return -7;
	
	if(strcmp(choice, blackWon) == 0)
		return -6;
	
	if(strcmp(choice, accept) == 0)
		return -4;
	
	if(strcmp(choice, deny) == 0)
		return -3;
	
	if(strcmp(choice, drawInteraction) == 0)
		return -2;
	
	if(strcmp(choice, resign) == 0)
		return -1;
	
	if(strcmp(choice, longCastling) == 0)
		return 1;
	
	if(strcmp(choice, shortCastling) == 0)
		return 2;
	
	switch(strlen(move)){
		case 2:	if(acceptableChar(move[0]) && ((int)move[1]- '0' < 9 && (int)move[1]- '0' > 0)) //[e4]
					return 3;
				break;
		case 3:	if(acceptableChar(move[0]) && acceptableChar(move[1]) && ((int)move[2]- '0' < 9 && (int)move[2]- '0' > 0)) //[Bd3]
					return 4;
				break;
		case 4:	if(acceptableChar(move[0]) && move[1] == 'x' && acceptableChar(move[2]) && ((int)move[3]- '0' < 9 && (int)move[3]- '0' > 0)) //[Bxd3]
					return 5;
				if(acceptableChar(move[0]) && acceptableChar(move[1]) && acceptableChar(move[2]) && ((int)move[3]- '0' < 9 && (int)move[3]- '0' > 0)) //[Rac1]
					return 8;
				break;
		case 5:	if(acceptableChar(move[0]) && ((int)move[1]- '0' < 9 && (int)move[1]- '0' > 0) && move[2] == 'x' && acceptableChar(move[3]) //[e4xd5]
					&&	((int)move[4]- '0' < 9 && (int)move[4]- '0' > 0))
					return 6;
				break;
		case 6:	if(acceptableChar(move[0]) && acceptableChar(move[1]) && ((int)move[2]- '0' < 9 && (int)move[2]- '0' > 0) && move[3] == 'x' //[Bd4xd3]
					&& acceptableChar(move[4])	&& ((int)move[5]- '0' < 9 && (int)move[5]- '0' > 0))
					return 7;
				break;
		default: return 0;
	}
															  															
}

void getInput(char *inputBuffer, int bufferLength){ //this function prevents bufferOverflow
	int ch;
    fgets(inputBuffer, bufferLength, stdin);

    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
    {
        while ((ch = getchar()) != '\n' && ch != EOF); 
    }
    else
    {
        inputBuffer[strlen(inputBuffer) -1] = '\0'; 
    }
}

void getFirstPart(char *firstPart, char move[]){  
	int i = 0;
	
	while(move[i] != 'x' && i<strlen(move)){
		firstPart[i] = move[i];
		i++;
	}
	
	firstPart[i] = '\0';
}

void getSecondPart(char *secondPart, char move[]){
	int i = strlen(move)-1; //-1 in order to not count the terminator 
	int j = 1;
	
	while(move[i] != 'x'){
		secondPart[j] = move[i];
					
		i--;
		j--;
	}
	secondPart[2] = '\0';
}

int analyzeInput(char move[], bool castlingCheck[], int playerColorTurn, PIECES *pieceToBeMoved, char *pieceToPromove, char *playerInteraction, int moveCoordsData[]){
	
	char firstPart[4] = {' '};
	char secondPart[3] = {' '}; 
	char *ch;
	PIECES pieceToBeMovedTemp;
	
	int returnValue, pattern, index;
	
	if(strstr(move, "=") != NULL){ //if the move contains '=' it means that probably the user wants to promote
		ch = strchr(move, '='); //i get the address of the '='
		index = (int)(ch - move); //i get the index of the character
		
		*pieceToPromove = move[index+1]; 
		move[index] = '\0'; //i delete the part after '='
	}
	
	pattern = isMoveValid(move);
	
	if(pattern != 0){ 
	
		if(pattern == -4){
			*playerInteraction = 'Y';
			return -100;
		}
	
		if(pattern == -3){
			*playerInteraction = 'N';
			return -100;
		}
		
		if(pattern == -2){
			*playerInteraction = 'D';
			return -100;
		}
	
		if(pattern == -1){
			*playerInteraction = 'R';
			return -100;
		}

		if(pattern == 1){
			*pieceToBeMoved = KING;
			return castlingMover(castlingCheck, 'l', playerColorTurn);
		}
		
		if(pattern == 2){
			*pieceToBeMoved = KING;
			return castlingMover(castlingCheck, 's', playerColorTurn);
		}
			
		if(pattern != 8){
			getFirstPart(firstPart, move); //I get the first part of the move (before the letter x)
		
			if(strlen(move) > 3)
				getSecondPart(secondPart, move); //I get the second part of the move (after the letter x) if it exists
		}else{
			firstPart[0] = move[0];
			firstPart[1] = move[1];
			firstPart[2] = '\0';
			
			secondPart[0] = move[2];
			secondPart[1] = move[3];
			secondPart[3] = '\0';
		}
		
		//I need to obtain which piece the user wants to move
		if(firstPart[0] != tolower(firstPart[0])){ 
			switch(firstPart[0]){
				case 'K': pieceToBeMovedTemp = KING;
					break;
				case 'Q': pieceToBeMovedTemp = QUEEN;
					break; 
				case 'R': pieceToBeMovedTemp = ROOK;
					break;
				case 'B': pieceToBeMovedTemp = BISHOP;
					break;
				case 'N': pieceToBeMovedTemp = KNIGHT;
					break;
			}
		}else{
			pieceToBeMovedTemp = PAWN;
		}
		
		returnValue = pieceFinder(pattern, firstPart, secondPart, moveCoordsData, pieceToBeMovedTemp, playerColorTurn); //I need to get the board coordinates from the move
		
		if(returnValue != -1)
			return returnValue;
		
		*pieceToBeMoved = pieceToBeMovedTemp;

		if(isKingUnderCheck(playerColorTurn)){//I check if the king is under check
			if(isKingUnderCheckAfterMove(moveCoordsData, pieceToBeMovedTemp, playerColorTurn) == false){ //if it is under check, i need to verify that the move can protect it
				returnValue = pieceMover(moveCoordsData, playerColorTurn, pieceToBeMovedTemp);
				
				return returnValue;
			}else{
				return 5;
			}
		}else{
			//I check if the king is under check after the move
			
			if(isKingUnderCheckAfterMove(moveCoordsData,  pieceToBeMovedTemp, playerColorTurn) == false){ 
				
				returnValue = pieceMover(moveCoordsData, playerColorTurn, pieceToBeMovedTemp);
				
				return returnValue;
			}else{
				return 4;
			}
		}
	}
	
	return 0;
}

bool noPieceCanPreventCheckmate(int pieceColor){
	
	int i, j, k, tempMoveCoordsData[4];
	for(i = 0; i < 8; i++){ //i need to iterate on every enemy piece in the board
			for(j = 0; j < 8; j++){
				if(board.squares[i][j].pezzo != EMPTY && board.squares[i][j].pezzo != KING && board.squares[i][j].pieceColor == pieceColor){ //i select the enemy piece
					
					if(board.squares[i][j].pezzo != PAWN) //I calculate the piece possible moves
						pieceCalculator(i, j, board.squares[i][j].pezzo, board.squares[i][j].pieceColor, false);
					else
						pawnCalculator(i, j, board.squares[i][j].pieceColor);
							
					for(k = 0; k < getNumberOfMoves(); k++){ //I verify if the king is under check on any of the enemy piece's possible moves
						
						tempMoveCoordsData[XPIECE] = i;
						tempMoveCoordsData[YPIECE] = j;
						tempMoveCoordsData[XDESTINATION] = possibleMoves[k][0];
						tempMoveCoordsData[YDESTINATION] = possibleMoves[k][1];
						
						if(!isKingUnderCheckAfterMove(tempMoveCoordsData, board.squares[i][j].pezzo, pieceColor)){
							return false;
						}
						
					}
				}
			}
		}
	
	return true;
	
}

int checkEndOfGame(int pieceColor){ 
	int x, y;
	int possibleMovesClone[32][2], possibleCheckMovesClone[64][2]; 
	
	memcpy(possibleMovesClone, possibleMoves, 32*2*sizeof(int)); // i need to preserve the original board
	memcpy(possibleCheckMovesClone, possibleCheckMoves, 64*2*sizeof(int));
	
	findKing(&x, &y, pieceColor);
	pieceCalculator(x, y, KING, pieceColor, true); 
	//stalemate
	if(!isUnderCheckAt(x, y, pieceColor, true) && possibleMoves[0][0] == -1){
		if(noPieceCanPreventCheckmate(pieceColor))
			return 1;
	}
	
	//checkmate
	if(isUnderCheckAt(x, y, pieceColor, true) && possibleMoves[0][0] == -1){
		if(noPieceCanPreventCheckmate(pieceColor))
			return 2;
	}
	
	memcpy(possibleMoves, possibleMovesClone, 32*2*sizeof(int));
	memcpy(possibleCheckMoves, possibleCheckMovesClone, 64*2*sizeof(int));
	
	return 0;
}

int playerInteractionHandler(bool drawRequest[], char playerInteraction, bool whiteTurn){
	if(playerInteraction != 0){
		switch(playerInteraction){
			case 'R': return 3;
				break;		
			case 'D': if(whiteTurn == BLACK)
						drawRequest[0] = true;
					  else
						drawRequest[1] = true;	
				break;
			case 'Y': if(whiteTurn == BLACK)
						drawRequest[0] = true;
					  else
						drawRequest[1] = true;
					if(drawRequest[0] && drawRequest[1])
						return 4;
				break;
			case 'N': if(whiteTurn == BLACK)
						drawRequest[1] = false;
					  else
						drawRequest[0] = false;				
		}	
	}
	return 0;
}

void chessEngine(){
	bool nobodyWon, whiteTurn = true, castlingCheck[6], drawRequest[2]; //castlingCheck values --> 0: wKingHasMoved 1: bKingHasMoved 2: wLeftRook 3: bLeftRook 4: wRightRook 5: bRightRook
	                                                                 //drawRequest values --> 0: blackOfferedDraw, 1: whiteOfferedDraw
 	char move[8], fileName[100], graphicBoard[GROWS][GCOLUMNS], pieceToPromove = 0, playerInteraction = 0; 
	int i, endCondition = 0, valid = -1, moveCoordsData[4]; //moveCoordsData contains the original coords and the destination coords of the piece
	PIECES pieceToBeMoved = EMPTY;
	
	for(i = 0; i<5; i++)
		castlingCheck[i] = false;
	
	drawRequest[0] = drawRequest[1] = false;

	initializeBoard();
	initializeGraphicBoard(graphicBoard);
	deleteStoredCheckMoves();
	deleteStoredPossibleMoves();
	
 	fileNameGenerator(fileName);
	
	do{
		system("cls");
		
		showGraphicBoard(graphicBoard);
		
		showInvalidMoveWarning(valid);
		
		printf("Type [resign] to resign!\n");
		printf("Type [Draw] to offer a draw!\n");
		printf("Possible Moves Board\t\tCheck Moves Board\n");
		
		if(pieceToBeMoved != KING)
			deleteStoredCheckMoves();
			
		showDebugBoards(moveCoordsData[XPIECE], moveCoordsData[YPIECE]);
		
		inputInterface(whiteTurn, drawRequest);
		
		moveCoordsData[XPIECE] = moveCoordsData[YPIECE] = -2; //I need to initialize xpiece e ypiece (used for pieceReverseFinder function)
	
		getInput(move, 8);
		
		valid = analyzeInput(move, castlingCheck, whiteTurn, &pieceToBeMoved, &pieceToPromove, &playerInteraction, moveCoordsData);
		
		if(valid == -100)
			endCondition = playerInteractionHandler(drawRequest, playerInteraction, whiteTurn);
		
		if(valid == -1){
			
			if(pieceToBeMoved == PAWN)
				promotionHandler(moveCoordsData[XDESTINATION], moveCoordsData[YDESTINATION], move, pieceToPromove, whiteTurn);
			else
				castlingHandler(moveCoordsData[YPIECE],  pieceToBeMoved, whiteTurn, castlingCheck);
			
			writeMove(fileName, move);
			
			endCondition = checkEndOfGame(!whiteTurn);

			whiteTurn = !whiteTurn;
		}
		
	}while(endCondition == 0);
	
	system("cls");
	showGraphicBoard(graphicBoard);

	switch(endCondition){
		case 1: printf("Nobody wins, draw by stalemate.");
				writeMove(fileName, "DRAW");
			break;
		case 2:
			if(whiteTurn == WHITE){
				printf("BLACK ");
				writeMove(fileName, "BLACK_WON");
			}
			else{
				printf("WHITE ");
				writeMove(fileName, "WHITE_WON");
			}
			printf("player wins by checkmate!");
			break;
		case 3:	
			if(whiteTurn == WHITE){
				printf("BLACK ");
				writeMove(fileName, "BLACK_WON");
			}
			else{
				printf("WHITE ");
				writeMove(fileName, "WHITE_WON");
			}
			printf("wins by resignation!");
			break;
		case 4: printf("Nobody wins, draw by agreement.");
				writeMove(fileName, "DRAW_");
	}
	
	printf("\n\nPress any key to continue...");
	getch();
	handleInput();
}

//games saver
void fileNameGenerator(char fileName[]){ 
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(fileName, "game %d-%02d-%02d %02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
}

void writeMove(char fileName[], char move[]){
	
	char path[MAXLENPERGAME+100];
	
	getcwd(path, MAXLENPERGAME+100); //I get the path where the .exe is located
	
	strcat(path, "\\Games\\");
	
	strcat(path, fileName);
	
	FILE *fp = fopen(path, "a");
	
	if (fp == NULL)
		{
		    printf("Error opening the file %s", fp);
		    Sleep(1000);
		}else{
			
		    fprintf(fp, move);
		    fprintf(fp, "\n");
		    fclose(fp);
		}
}

//Game Viewer

void printGameViewerInterface(char gameMoves[][MAXLENPERWRITTENMOVE], int moveNumber, bool whiteTurn, int endOfGame){
	char move[MAXLENPERWRITTENMOVE];
	
	printf("\n");
	printf("\t      Press ESC to exit\n\n");
			
	if(endOfGame == 0){
		if(moveNumber != 0){
			if(whiteTurn == true)
				printf("\t     White's move: %s", strcpy(move, gameMoves[moveNumber]));
			else
				printf("\t     Black's move: %s", strcpy(move, gameMoves[moveNumber]));
		}
	}else{
		if(endOfGame != -8){
			if(endOfGame == -7)
				printf("\t\tWhite player wins!", strcpy(move, gameMoves[moveNumber]));
			else
				printf("\t\tBlack player wins!", strcpy(move, gameMoves[moveNumber]));
		}else{
			printf("\t\t   Draw!", strcpy(move, gameMoves[moveNumber]));
		}
		
	}
	
	printf("\n\n");
	printf("     [Left arrow] ------ [Right arrow]\n");
	printf("    Previous move  ----    Next move\n");
	
}

int loadGame(FILE *file, Board gameStates[], char gameMoves[][MAXLENPERWRITTENMOVE], int* endOfGame){
	bool whiteTurn = true, castlingCheck[6];
	int pattern, moveCoordsData[4], count = 1, index, castling, i;
	char firstPart[4] = {' '}, secondPart[3] = {' '}, move[MAXLENPERWRITTENMOVE], *ch, pieceToBePromoted;
	PIECES pieceToBeMovedTemp;
	
	fseek(file, 0, SEEK_SET); 
	initializeBoard();
	*endOfGame = 0;
	
	for(i = 0; i<5; i++)
		castlingCheck[i] = false;
	
	memcpy(&gameStates[0], &board, sizeof(Board)); 
	
	if ( file != NULL ){
		while (fscanf(file, "%s\n", move) != EOF){ 
			//-------------------move analysis-----------------------
			
			pieceToBePromoted = -1; 
			
			if(strstr(move, "=") != NULL){ 
				ch = strchr(move, '='); 
				index = (int)(ch - move); 
				
				pieceToBePromoted = move[index+1];
				move[index] = '\0';
			}
			
			castling = 0;
			pattern = isMoveValid(move);
			
			switch(pattern){
				case 1: castling = 1;
					break;
				case 2: castling = 2;
					break;
				case 8: 
						firstPart[0] = move[0];
						firstPart[1] = move[1];
						firstPart[2] = '\0';
							
						secondPart[0] = move[2];
						secondPart[1] = move[3];
						secondPart[3] = '\0';
					break;
				case -6: *endOfGame = -6;//if the black player wins
						return count-1;
				case -7: *endOfGame = -7;//if the white player wins
						return count-1;
				case -8: *endOfGame = -8;//if draw
						return count-1;

				default: 
						getFirstPart(firstPart, move);
						if(strlen(move) > 3)
							getSecondPart(secondPart, move); 
			}
			
			if(castling == 0){
				if(firstPart[0] != tolower(firstPart[0])){ 
					switch(firstPart[0]){
						case 'K': pieceToBeMovedTemp = KING;
							break;
						case 'Q': pieceToBeMovedTemp = QUEEN;
							break; 
						case 'R': pieceToBeMovedTemp = ROOK;
							break;
						case 'B': pieceToBeMovedTemp = BISHOP;
							break;
						case 'N': pieceToBeMovedTemp = KNIGHT;
							break;
					}
				}else{
					pieceToBeMovedTemp = PAWN;
				}
				
				pieceFinder(pattern, firstPart, secondPart, moveCoordsData, pieceToBeMovedTemp, whiteTurn);
				//-------------------end of move analysis-----------------------
			
				pieceMover(moveCoordsData, whiteTurn, pieceToBeMovedTemp); 
				
			}else{
				if(castling == 1)
					castlingMover(castlingCheck, 'l', whiteTurn);
				else
					castlingMover(castlingCheck, 's', whiteTurn);
				
			}
			
			if(pieceToBePromoted != -1){ //in case of promotion i need to replace the pawn with the wanted piece
				switch(pieceToBePromoted){
						case 'Q': board.squares[moveCoordsData[XDESTINATION]][moveCoordsData[YDESTINATION]].pezzo = QUEEN; 
							break; 
						case 'R': board.squares[moveCoordsData[XDESTINATION]][moveCoordsData[YDESTINATION]].pezzo = ROOK;
							break;
						case 'B': board.squares[moveCoordsData[XDESTINATION]][moveCoordsData[YDESTINATION]].pezzo = BISHOP;
							break;
						case 'N': board.squares[moveCoordsData[XDESTINATION]][moveCoordsData[YDESTINATION]].pezzo = KNIGHT;
							break;
					}
			}
				
			memcpy(&gameStates[count], &board, sizeof(Board)); //i save the current status of the board 
		 	strcpy(gameMoves[count], move);
			 	
			count++;
			whiteTurn = !whiteTurn;
		}
		return count-1;
	}
	return 0;
}

void gameViewer(char gamesList[][MAXLENPERGAME], int index){

	Board gameStates[200];
	char gamePath[MAXLENPERGAME+100], ch, graphicBoard[GROWS][GCOLUMNS], gameMoves[200][MAXLENPERWRITTENMOVE];
	int moveNumber = 0, movesOfGame, endOfGame, endOfGameClone = 0;
	bool whiteTurn = false;
	
	getcwd(gamePath, MAXLENPERGAME+100); 
	strcat(gamePath, "\\Games\\");
	strcat(gamePath, gamesList[index]);
	
	FILE *fp = fopen(gamePath, "r");
	
	movesOfGame = loadGame(fp, gameStates, gameMoves, &endOfGame); //I load the entire game into a matrix array, then i obtain the number of moves of the game. 
	
	initializeGraphicBoard(graphicBoard); 
	system("cls");
	
	memcpy(&board, &gameStates[0], sizeof(Board)); //Loading the starting status
	showGraphicBoard(graphicBoard);
	printGameViewerInterface(gameMoves, moveNumber, whiteTurn, endOfGameClone);
	do{
		ch = getch();
		
		if(ch == 27)
			handleInput();
		
		ch = getch();
		switch(ch){
			case 75: if(moveNumber != 0)  //left
						moveNumber--;
					break;
			case 77: moveNumber++; //right
					break;
		}
		system("cls");
		
		if(ch == 27)
			handleInput();
		
		if(moveNumber > movesOfGame){
			moveNumber = movesOfGame;
			endOfGameClone = endOfGame;
		}else{
			endOfGameClone = 0; 
			if(moveNumber < 0)
				moveNumber = 0;	
			else
				whiteTurn = !whiteTurn;
		}
			
		memcpy(&board, &gameStates[moveNumber], sizeof(Board)); 
		
		showGraphicBoard(graphicBoard);
		printGameViewerInterface(gameMoves, moveNumber, whiteTurn, endOfGameClone);
		
	}while(ch != 13);
	
}

int getNumberOfGames(char gamesList[][MAXLENPERGAME]){
	int i = 0, count = 0;
	
	while(i<MAXGAMES){
		if(gamesList[i][0] != ' ')
			count ++;
		i++;
	}
	
	return count;
	
}

void initializeGamesList(char gamesList[][MAXLENPERGAME]){  
	int i;
	for(i = 0; i<MAXGAMES; i++)
		strcpy(gamesList[i], " ");

}

void cloneArray(char clone[][35], char original[][MAXLENPERGAME]){ 
    int i;
    for(i=0;i<getNumberOfGames(original);i++)
    	 strcpy(clone[i], original[i]);
       
}

void getFilesInGamesFolder(char gamesList[][MAXLENPERGAME]){
	DIR *d; 
	int len, rows;
	struct dirent *dir; //I create the buffer where the directory's data will be put 
	const char *last_four;
	char path[MAXLENPERGAME+10];

	getcwd(path, MAXLENPERGAME+10); 
	strcat(path, "\\Games");
		
	d = opendir(path);
	if (d) { //if the directory exists
	   while ((dir = readdir(d)) != NULL) {
	   	len = strlen(dir->d_name); //i get the .txt file name's lenght
	   	last_four = &dir->d_name[len-4]; //i check the last 4 characters of the file 
	   	
	   	if(strncmp(last_four, ".txt", 4) == 0) //if the file ends with .txt
	   		strcpy(gamesList[getNumberOfGames(gamesList)], dir->d_name); 
		}
	closedir(d);
	}
}

void printInterfaceGameViewer(int choice, char gamesList[][MAXLENPERGAME], char gamesListClone[][MAXLENPERGAME]){
	
	bool firstTime = true;
	int i, numberOfGames = getNumberOfGames(gamesList);
	
	cloneArray(gamesListClone, gamesList);
	
	system("cls");
	printf("Which match do you want to rewatch?\n\n");
	
	if(numberOfGames>0){
		for(i = 0; i<numberOfGames; i++){
			if(i == choice){
				strcat(gamesListClone[i], " <--");
			}
			printf("%s\n", gamesListClone[i]);
		}
	}else{
		printf("\nNo games has been found!\n");
	}
	
	
	printf("\nPress [ESC] to go back");

}

void handleInputGameViewer(){ 
	char ch, gamesList[MAXGAMES][MAXLENPERGAME], gamesListClone[MAXGAMES][MAXLENPERGAME];
	int index = 0, numberOfGames; 

	initializeGamesList(gamesList);

	getFilesInGamesFolder(gamesList);
	numberOfGames = getNumberOfGames(gamesList);
		
	printInterfaceGameViewer(index, gamesList, gamesListClone);
	
	do{
		ch=getch();

		switch(ch)
		{
			case 80: //down
				index++;
				break;		
			case 72:	//up
				index--;
				break;
		}
					
		if(index == numberOfGames)
			index = 0;
		if(index == -1)
			index = numberOfGames-1;
		
		printInterfaceGameViewer(index, gamesList, gamesListClone);
	
	}while(ch!=13 && ch!=27); //until esc or enter are pressed
		
	if(ch==27)
		handleInput();
	else
		gameViewer(gamesList, index);
	
}

void printInterface(int choice){ 
	
	system("cls");
	printf("Select your choice!\n\n");
	
	switch(choice){
		case 1: printf("Start a new game <--\n");
				printf("Load a game\n");
			break;
		case 2: printf("Start a new game\n");
				printf("Load a game <--\n");
			break;
	}
	
}

void handleInput(){ 
	
	char ch; int index =1;
	
	printInterface(index);
	do{
		ch=getch();
		switch(ch)
		{
			case 80: //down
				index++;
				break;		
			case 72:	//up
				index--;
				break;
		}
				
		if(index == 3)
			index = 1;
		if(index == 0)
			index = 2;
				
		printInterface(index);
		
	}while(ch!=13); //until enter is pressed
	
	if(index == 1)
		chessEngine();
	else
		handleInputGameViewer();
	
}
 
int main(){
	
	struct stat s = {0}; 
	
	if (stat("Games", &s) == -1) {
    	mkdir("Games");
	}   
  
	handleInput();
}
