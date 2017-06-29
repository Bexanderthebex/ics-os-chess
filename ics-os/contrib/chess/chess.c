#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"
#include <stdbool.h>
#include <math.h>
// #include <stdio.h>

/*TODO*/
//1. fix bishop movement [done]
//2. do the en passant features
//3. fix user usability
//4. fix minor bug in castling yellow side

/*
BONUS TODO
1. undo move
*/


//prototypes
struct chess_board * initialize_board();
void draw_board(int col, int row);
void render_board(struct chess_board * board);
bool move(int rowSrc, int colSrc, int rowDest, int colDest, int turn);
bool king(int rowSrc, int colSrc, int rowDest, int colDest);
bool queen(int rowSrc, int colSrc, int rowDest, int colDest);
bool bishop(int rowSrc, int colSrc, int rowDest, int colDest);
bool rook(int rowSrc, int colSrc, int rowDest, int colDest);
bool horse(int rowSrc, int colSrc, int rowDest, int colDest);
bool pawn(int rowSrc, int colSrc, int rowDest, int colDest, int turn);
void render_board(struct chess_board * board);
void render_square(int current_row, int current_col, char temp_piece);
void erase_square(int current_row, int current_col);
void switchPlayer();
void erase();

//constants
#define maxrow 9
#define maxcol 40
#define size 8
/*rank representations in array indexes. a.k.a columns*/
#define a 0				
#define b 1
#define c 2
#define d 3
#define e 4
#define f 5
#define g 6
#define h 7
#define up 'w'
#define left 'a'
#define right 'd'
#define down 's'
#define enter '\n'
#define exit 'e'
// #define undo 'u'
#define start_row_white 7
#define start_row_yellow 0

/*global variables*/
struct chess_move{
	int srcFile;
	int srcRank;							
	int destFile;
	int destRank;
	struct chess_move * next;
};

struct chess_pieces{
	int color;
	char piece;
	int piece_moves_done;
};

struct player{
	struct chess_pieces pieces_captured[16];
	struct chess_move *player_moves_done;
};

struct chess_board{
	struct chess_pieces arr_board[8][8];
	struct chess_move *moves_done;
};

int counter;
int turn;
int winner;
bool endGame = false;
bool exitGame = false;
char player[];

struct chess_board * board_main;
//controls variables
char keypress;
int current_col;
int current_row;
int dest_col;
int dest_row;
int temp_color;
char temp_piece;



//miscellaneous func
int abs(int x) {
	if(x<0) return x *= -1;
	return x;
}

/*=========
	MAIN
===========*/

int main(){
	do{
		int _enter = 0;
		turn = WHITE;
		board_main = initialize_board();
		winner = NULL;
		exitGame = false;
		endGame = false;
		clrscr();
		set_graphics(VGA_320X200X256);
		draw_board(20, 10);
		render_board(board_main);
		write_text("I C S",227,20,LIGHTMAGENTA, 20);
		write_text("C H E S S",210,35,LIGHTMAGENTA, 20);
		write_text("Turn:",230,65,WHITE, 20);
		write_text("[ENTER] CHOOSE", 187, 105, GREEN, 10);
		write_text("[W] UP", 187, 115, GREEN, 10);
		write_text("[A] LEFT", 187, 125, GREEN, 10);
		write_text("[S] DOWN", 187, 135, GREEN, 10);
		write_text("[D] RIGHT", 187, 145, GREEN, 10);
		write_text("[E] Exit", 217, 175, RED, 10);
		do{
			write_text("       2", 217, 85, BLACK, 10);
			write_text("       1", 217, 85, BLACK, 10);
			if(turn == WHITE) write_text("Player 1", 217, 85, turn, 10);
			else write_text("Player 2", 217, 85, turn, 10);
			if(turn == WHITE){									//check first who's turn it is.
				current_col = a;
				current_row = start_row_white;
			}else{
				current_col = a;
				current_row = start_row_yellow;
			}
			temp_piece = board_main->arr_board[current_row][current_col].piece;													
			render_square(current_col, current_row, temp_piece);							//starting square of every color.
			do{		
				if(keypress = (char)getch()){													//if user pressed key
					if(keypress == up){
						erase_square(current_col, current_row);
						if(current_row == 0)
							current_row = 7;
						else
							current_row = (current_row-1) % 8;

						temp_piece = board_main->arr_board[current_row][current_col].piece;
						render_board(board_main);
						render_square(current_col, current_row, temp_piece);
					}else if(keypress == down){
						erase_square(current_col, current_row);
						current_row = (current_row+1) % 8;
						temp_piece = board_main->arr_board[current_row][current_col].piece;
						render_board(board_main);
						render_square(current_col, current_row, temp_piece);
					}else if(keypress == right){
						erase_square(current_col, current_row);
						current_col = (current_col+1) % 8;
						temp_piece = board_main->arr_board[current_row][current_col].piece;
						render_board(board_main);
						render_square(current_col, current_row, temp_piece);
					}else if(keypress == left){
						erase_square(current_col, current_row);
						if(current_col == 0)
							current_col = 7;
						else
							current_col = (current_col-1) % 8;

						temp_piece = board_main->arr_board[current_row][current_col].piece;
						render_board(board_main);
						render_square(current_col, current_row, temp_piece);
					}else if(keypress == enter){
						erase_square(current_col, current_row);
						_enter++;
					}else if(keypress == exit) {
						exitGame = true;
						endGame = true;
						write_text("G A M E   O V E R", 103, 50, LIGHTMAGENTA,50);
						break;
					}
				}
			}while(_enter < 1);

			if(!endGame){
				dest_col = current_col;
				dest_row = current_row;
	
				do{
					if(keypress = (char)getch()){
						if(keypress == up){
							erase_square(dest_col, dest_row);
							if(dest_row == 0)
								dest_row = 7;
							else
								dest_row = (dest_row-1) % 8;
	
							temp_piece = board_main->arr_board[dest_row][dest_col].piece;
							render_board(board_main);
							render_square(dest_col, dest_row, temp_piece);
						}else if(keypress == down){
							erase_square(dest_col, dest_row);
							dest_row = (dest_row+1) % 8;
							temp_piece = board_main->arr_board[dest_row][dest_col].piece;
							render_board(board_main);
							render_square(dest_col, dest_row, temp_piece);
						}else if(keypress == right){
							erase_square(dest_col, dest_row);
							dest_col = (dest_col+1) % 8;
							temp_piece = board_main->arr_board[dest_row][dest_col].piece;
							render_board(board_main);
							render_square(dest_col, dest_row, temp_piece);
						}else if(keypress == left){
							erase_square(dest_col, dest_row);
							if(dest_col == 0)
								dest_col = 7;
							else
								dest_col = (dest_col-1) % 8;
	
							temp_piece = board_main->arr_board[dest_row][dest_col].piece;
							render_board(board_main);
							render_square(dest_col, dest_row, temp_piece);
						}else if(keypress == enter){
							if(dest_col == current_col && dest_row == current_row)
								_enter = 0;
							else
								_enter = 2;
							erase_square(dest_col, dest_row);
						}
					}
				}while(_enter == 1);
	
				if(_enter == 2){
					// erase_square(current_col, current_row);
					if(move(current_row, current_col, dest_row, dest_col, turn)){
						render_board(board_main);
						switchPlayer();
					}
				}
				erase_square(current_col, current_row);
				render_board(board_main);
				_enter = 0;
			}

		}while(!endGame);
		write_text("G A M E   O V E R", 120, 60, LIGHTMAGENTA,50);
		if(winner!=NULL) {
			if(winner == YELLOW) strcpy(player,"Player 2 wins");
			else strcpy(player,"Player 1 wins");
			clrscr();
			set_graphics(VGA_320X200X256);
			write_text("G A M E   O V E R", 73, 50, LIGHTMAGENTA,50);
			write_text(player, 95, 75, LIGHTMAGENTA,10);
			write_text("[N] New game", 100, 100, BLUE, 10);
			write_text("[E] Exit", 120, 115, RED, 10);
			do{
				if(keypress = (char)getch()){
					if(keypress == exit) {
						exitGame = true;
						break;
					}
				}	
			} while(keypress!='n' && !endGame);
		}
		if(exitGame) {
			set_graphics(VGA_TEXT80X25X16);
  			clrscr();
		}
	}while(!exitGame);
	


	return 0;
}

/*=============
	CHECKERS
===============*/

//--------------> QUEEN
bool queen(int rowSrc, int colSrc, int rowDest, int colDest) {
	if(rook(rowSrc,colSrc,rowDest,colDest) ^ bishop(rowSrc,colSrc,rowDest,colDest))		//xor
		return true;
	return false;
}

//--------------> KING
bool king(int rowSrc, int colSrc, int rowDest, int colDest){
	int rowDiff = abs(rowSrc - rowDest);
	int colDiff = abs(colSrc - colDest);
	if(rowDiff == 1 && colDiff == 1 || colDiff == 1 || rowDiff == 1)
		return true;
	if(rowDiff == 0 && colDiff == 2 && board_main->arr_board[rowSrc][colSrc].piece_moves_done == 0
		&& board_main->arr_board[rowDest][colDest].piece == ' ') {
		if( ( (colSrc-colDest > 0 && board_main->arr_board[rowSrc][colSrc-1].piece == ' ') && (board_main->arr_board[rowSrc][colDest-2].piece_moves_done == 0) )
			|| ( (colSrc-colDest < 0 && board_main->arr_board[rowSrc][colSrc+1].piece == ' ') && (board_main->arr_board[rowSrc][colDest+1].piece_moves_done == 0) ) ) 
			return true;
	}
	return false;
}

//--------------> BISHOP
bool bishop(int rowSrc, int colSrc, int rowDest, int colDest) {
	int rowDiff = abs(rowSrc-rowDest);
	int colDiff = abs(colSrc-colDest);
	int x,y;
	if(rowDiff == colDiff) {
		if(board_main->arr_board[rowDest][colDest].piece != ' ')
		for(int i=1; i<rowDiff;i++) {
			if(rowSrc > rowDest)
				x = abs(rowSrc-i);
			else
				x = abs(rowSrc+i);
			if(colSrc > colDest)
				y = abs(colSrc-i);
			else
				y = abs(colSrc+i);
			if(board_main->arr_board[x][y].piece != ' ')
				return false;
		}
		return true;
	}
	return false;
}

//--------------> HORSE
bool horse(int rowSrc, int colSrc, int rowDest, int colDest) {
	int rowDiff = abs(rowSrc - rowDest);
	int colDiff = abs(colSrc - colDest);
	if((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))
		return true; 
	return false;
}

//--------------> ROOK
bool rook(int rowSrc, int colSrc, int rowDest, int colDest) {
	if(rowSrc != rowDest && colSrc != colDest) return false;
	else {
		int op=1;
		if(rowSrc == rowDest) {
			if(colSrc<colDest) op=-1;
			for(int i=1;i<abs(colSrc-colDest);i++)
				if(board_main->arr_board[rowDest][colDest+(i*op)].piece != ' ')
					return false;
			return true;
		} else {
			if(rowSrc<rowDest) op=-1;
			for(int i=1;i<abs(rowSrc-rowDest);i++)
				if(board_main->arr_board[rowDest+(i*op)][colDest].piece != ' ')
					return false;
			return true;
		}
	}
}

//--------------> PAWN
bool pawn(int rowSrc, int colSrc, int rowDest, int colDest, int turn){
	int rowDiff = (rowSrc - rowDest);
	int colDiff = (colSrc - colDest);
	int x;
	// int colDiff = abs(colSrc - colDest);
	//TODO diagonal capture piece check

	//check first if there i a blockade

	if(board_main->arr_board[rowSrc][colSrc].color == YELLOW) x = 1;
	else x = -1;

	if( ((rowDiff < 0 && board_main->arr_board[rowSrc][colSrc].color == YELLOW) ||
		(rowDiff > 0 && board_main->arr_board[rowSrc][colSrc].color == WHITE)) && 
		(colDiff <=2 && colDiff>=-2) ) {
		if(abs(rowDiff) <= 2 && colDiff == 0) { // forward move
			if(abs(rowDiff) == 2) {	// 2 cells
		 		if(board_main->arr_board[rowSrc][colSrc].piece_moves_done != 0)
					return false;
				if(board_main->arr_board[rowSrc+x][colSrc].piece != ' ' || board_main->arr_board[rowSrc+(x*2)][colSrc].piece != ' ')
					return false;
			} else {	// 1 cell
				if(board_main->arr_board[rowDest][colDest].piece != ' ')
					return false;
				if(rowDest == 0 || rowDest == 7){
					erase(190, 20, 100, 30);
					write_text("Choose: ", 190, 0, turn, 0);
					write_text("Q = queen", 190, 10, turn, 0);
					write_text("R = rook", 190, 20, turn, 0);
					write_text("B = bishop", 190, 30, turn, 0);
					write_text("H = knight", 190, 40, turn, 0);
					char temp_char = ' ';
					do{
						if(temp_char = (char)getch())
						{
							if(temp_char == 'q'){
								board_main->arr_board[rowSrc][colSrc].piece = 'Q';
								board_main->arr_board[rowSrc][colSrc].color = turn;
								break;
							}else if(temp_char == 'r'){
								board_main->arr_board[rowSrc][colSrc].piece = 'R';
								board_main->arr_board[rowSrc][colSrc].color = turn;
								break;
							}else if(temp_char == 'b'){
								board_main->arr_board[rowSrc][colSrc].piece = 'B';
								board_main->arr_board[rowSrc][colSrc].color = turn;
								break;
							}else if(temp_char == 'h'){
								board_main->arr_board[rowSrc][colSrc].piece = 'H';
								board_main->arr_board[rowSrc][colSrc].color = turn;
								break;
							}
						}
					}while(1);
					erase(190, 0, 100, 60);
					write_text("I C S",227,20,LIGHTMAGENTA, 20);
					write_text("C H E S S",210,35,LIGHTMAGENTA, 20);
				}
			}
		}
		else if(abs(rowDiff) == 1 && abs(colDiff) == 1) { // diagonal move
			if(board_main->arr_board[rowDest][colDest].piece == ' '){
				if(board_main->arr_board[rowSrc][colDest].piece == 'P'){
					struct chess_move * temp = board_main->moves_done;
					while(temp->next != NULL) {
						temp = temp->next;
					}
					int temp_rowSrc = temp->srcRank;
					int temp_colScr = temp->srcFile;
					int temp_rowDest = temp->destRank;
					int temp_colDest = temp->destFile;

					if((temp_rowSrc - temp_rowDest == -2 && temp_colDest == colDest)
						|| 
						(temp_rowSrc - temp_rowDest == 2 && temp_colDest == colDest) )
					{
						board_main->arr_board[rowSrc][colDest].piece = ' ';
						board_main->arr_board[rowSrc][colDest].color = BLACK;
						erase_square(rowSrc,colDest);
						render_board(board_main);
						return true;
					}
					else
						return false;
				}else
					return false;
			}else if(rowDest == 0 || rowDest == 7){
				erase(190, 20, 100, 30);
				write_text("Choose: ", 190, 0, turn, 0);
				write_text("Q = queen", 190, 10, turn, 0);
				write_text("R = rook", 190, 20, turn, 0);
				write_text("B = bishop", 190, 30, turn, 0);
				write_text("H = knight", 190, 40, turn, 0);
				char temp_char = ' ';
				do{
					if(temp_char = (char)getch())
					{
						if(temp_char == 'q'){
							board_main->arr_board[rowSrc][colSrc].piece = 'Q';
							board_main->arr_board[rowSrc][colSrc].color = turn;
							break;
						}else if(temp_char == 'r'){
							board_main->arr_board[rowSrc][colSrc].piece = 'R';
							board_main->arr_board[rowSrc][colSrc].color = turn;
							break;
						}else if(temp_char == 'b'){
							board_main->arr_board[rowSrc][colSrc].piece = 'B';
							board_main->arr_board[rowSrc][colSrc].color = turn;
							break;
						}else if(temp_char == 'h'){
							board_main->arr_board[rowSrc][colSrc].piece = 'H';
							board_main->arr_board[rowSrc][colSrc].color = turn;
							break;
						}
					}
				}while(1);
				erase(190, 0, 100, 60);
				write_text("I C S",227,20,LIGHTMAGENTA, 20);
				write_text("C H E S S",210,35,LIGHTMAGENTA, 20);
			}
		} else {
			return false;
		}
		return true;
	}
	return false;
}


/*==================
	GAME FUNCTIONS
====================*/

bool move(int rowSrc, int colSrc, int rowDest, int colDest, int turn) {			//TODO add moves done in the 
	bool valid = false;
	int col,x = 0;
	char piece = board_main->arr_board[rowSrc][colSrc].piece;

	if(	rowSrc >= size || colSrc >= size || rowSrc < 0 || colSrc < 0 ) return false;
	else if(rowDest >= size || colDest >= size || rowDest < 0 || colDest < 0) return false;
	else {
		if(board_main->arr_board[rowDest][colDest].color == turn || board_main->arr_board[rowSrc][colSrc].color != turn)
			return false;
		else {
			if(piece == 'K') valid = king(rowSrc,colSrc,rowDest,colDest);
			else if(piece == 'Q') valid = queen(rowSrc,colSrc,rowDest,colDest);
			else if(piece == 'H') valid = horse(rowSrc,colSrc,rowDest,colDest);
			else if(piece == 'B') valid = bishop(rowSrc,colSrc,rowDest,colDest);
			else if(piece == 'R') valid = rook(rowSrc,colSrc,rowDest,colDest);
			else if(piece == 'P') valid = pawn(rowSrc,colSrc,rowDest,colDest, turn);

			if(valid) {
				if(piece == 'K' && rowSrc == rowDest) {
					if(colDest-colSrc==2) {
						x = -1;
						col = colDest+1;
					}
					else {
						x = 1;
						col = colDest-2;
					}
					if( board_main->arr_board[rowSrc][col].piece == 'R' &&
						board_main->arr_board[rowSrc][col].color == turn &&
						board_main->arr_board[rowSrc][col].piece_moves_done == 0) {
							// struct chess_move * temp = board_main->arr_board[rowSrc][col].piece_moves_done;
							// while(temp->next != NULL) {
							// 	temp = temp->next;
							// }
							// temp->next=malloc(sizeof(struct chess_move));
							// temp->next->rank = rowSrc;
							// temp->next->file = colDest+x;
							board_main->arr_board[rowSrc][col].piece_moves_done += 1; 

							board_main->arr_board[rowSrc][colDest+x] = board_main->arr_board[rowSrc][col];
							board_main->arr_board[rowSrc][col].piece = ' ';
							board_main->arr_board[rowSrc][col].color = BLACK;
							erase_square(rowSrc, col);
							render_board(board_main);
							// char i = rowSrc + '0';
							// char k = col + '0';
							// write_char(i, 200, 130, WHITE, 20);
							// write_char(k, 220, 130, WHITE, 20);					
					}
				}
				// TODO insert move done in board_main
				struct chess_move * temp = board_main->moves_done;
				while(temp->next != NULL) {
					temp = temp->next;
				}
				temp->next=malloc(sizeof(struct chess_move));
				temp->next->srcRank = rowSrc;
				temp->next->srcFile = colDest;
				temp->next->destRank = rowDest;
				temp->next->destFile = colDest;

				//increment piece move done
				board_main->arr_board[rowSrc][colSrc].piece_moves_done += 1;

				// move piece
				if(board_main->arr_board[rowDest][colDest].piece=='K') {
					endGame = true;
					winner = turn;
				}
				board_main->arr_board[rowDest][colDest] = board_main->arr_board[rowSrc][colSrc];
				board_main->arr_board[rowSrc][colSrc].color = BLACK;
				board_main->arr_board[rowSrc][colSrc].piece = ' ';				
				return true;
			}
			return false;
		}
	}
}

void switchPlayer() {
	if(turn==WHITE) turn=YELLOW;
	else turn=WHITE;
}


/*==================
	GUI FUNCTIONS
====================*/
void erase(int x, int y, int w, int k){ //basically covers an area with a black rectangle 
   int i,j;
   for (i=y;i<=(y+k);i++)
      for (j=x;j<=(x+w);j++)
         write_pixel(j,i,100);
}

void render_square(int board_x, int board_y, char piece){		//board_x and y are only multipliers
	int x_coordinate = 21 + (20*board_x);
	int y_coordinate = 11 + (20*board_y);
	int x_piece = 27 + (20*board_x);
	int y_piece = 17 + (20*board_y);

	for(int i = 0; i <= 18; i++){
		write_pixel(x_coordinate, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+1, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+2, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+3, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+4, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+5, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+6, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+7, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+8, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+9, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+10, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+11, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+12, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+13, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+14, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+15, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+16, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+17, y_coordinate+i, WHITE);
		write_pixel(x_coordinate+18, y_coordinate+i, WHITE);
	}

	write_char(piece, x_piece, y_piece, BLACK, 0);
}

void erase_square(int board_x, int board_y){		//covers the previously selected square with black square
	int x_coordinate = 21 + (20*board_x);
	int y_coordinate = 11 + (20*board_y);
	int x_piece = 27 + (20*board_x);
	int y_piece = 17 + (20*board_y);

	for(int i = 0; i <= 18; i++){
		write_pixel(x_coordinate, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+1, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+2, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+3, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+4, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+5, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+6, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+7, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+8, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+9, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+10, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+11, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+12, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+13, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+14, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+15, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+16, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+17, y_coordinate+i, BLACK);
		write_pixel(x_coordinate+18, y_coordinate+i, BLACK);
	}
}

struct chess_board * initialize_board(){
	struct chess_board * board = malloc(sizeof(struct chess_board));
	board->moves_done = malloc(sizeof(struct chess_move));
	board->moves_done->next = NULL;


	board->arr_board[0][a].color = YELLOW;
	board->arr_board[0][a].piece = 'R';
	board->arr_board[0][a].piece_moves_done = 0;
	board->arr_board[7][a].color = WHITE;
	board->arr_board[7][a].piece = 'R';
	board->arr_board[7][a].piece_moves_done = 0;
	board->arr_board[0][b].color = YELLOW;
	board->arr_board[0][b].piece = 'H';
	board->arr_board[0][b].piece_moves_done = 0;
	board->arr_board[7][b].color = WHITE;
	board->arr_board[7][b].piece = 'H';
	board->arr_board[7][b].piece_moves_done = 0;
	board->arr_board[0][c].color = YELLOW;
	board->arr_board[0][c].piece = 'B';
	board->arr_board[0][c].piece_moves_done = 0;
	board->arr_board[7][c].color = WHITE;
	board->arr_board[7][c].piece = 'B';
	board->arr_board[7][c].piece_moves_done = 0;
	board->arr_board[0][d].color = YELLOW;
	board->arr_board[0][d].piece = 'Q';
	board->arr_board[0][d].piece_moves_done = 0;
	board->arr_board[7][d].color = WHITE;
	board->arr_board[7][d].piece = 'Q';
	board->arr_board[7][d].piece_moves_done = 0;
	board->arr_board[0][e].color = YELLOW;
	board->arr_board[0][e].piece = 'K';
	board->arr_board[0][e].piece_moves_done = 0;
	board->arr_board[7][e].color = WHITE;
	board->arr_board[7][e].piece = 'K';
	board->arr_board[7][e].piece_moves_done = 0;
	board->arr_board[0][f].color = YELLOW;
	board->arr_board[0][f].piece = 'B';
	board->arr_board[0][f].piece_moves_done = 0;
	board->arr_board[7][f].color = WHITE;
	board->arr_board[7][f].piece = 'B';
	board->arr_board[7][f].piece_moves_done = 0;
	board->arr_board[0][g].color = YELLOW; 
	board->arr_board[0][g].piece = 'H';
	board->arr_board[0][g].piece_moves_done = 0;
	board->arr_board[7][g].color = WHITE; 
	board->arr_board[7][g].piece = 'H';
	board->arr_board[7][g].piece_moves_done = 0;
	board->arr_board[0][h].color = YELLOW; 
	board->arr_board[0][h].piece = 'R';
	board->arr_board[0][h].piece_moves_done = 0;
	board->arr_board[7][h].color = WHITE; 
	board->arr_board[7][h].piece = 'R';
	board->arr_board[7][h].piece_moves_done = 0;

	for(int i = 0; i < 8; i++){//pawns
		board->arr_board[1][i].color = YELLOW; 
		board->arr_board[1][i].piece = 'P';
		board->arr_board[1][i].piece_moves_done = 0;
		board->arr_board[6][i].color = WHITE; 
		board->arr_board[6][i].piece = 'P';
		board->arr_board[6][i].piece_moves_done = 0;
	}	

	for(int i = 0; i < 8; i++){
		board->arr_board[2][i].color = BLACK; 
		board->arr_board[2][i].piece = ' ';
		board->arr_board[3][i].color = BLACK; 
		board->arr_board[3][i].piece = ' ';
		board->arr_board[4][i].color = BLACK; 
		board->arr_board[4][i].piece = ' ';
		board->arr_board[5][i].color = BLACK; 
		board->arr_board[5][i].piece = ' ';
	}

	return board;
}

void render_board(struct chess_board * board){					//TODO implement this using the structure chess_board
	for(int i = 0; i < 8; i++){
		write_char(board->arr_board[0][i].piece, 27+(i*20), 17, board->arr_board[0][i].color, 0);
		write_char(board->arr_board[1][i].piece, 27+(i*20), 37, board->arr_board[1][i].color, 0);
		write_char(board->arr_board[2][i].piece, 27+(i*20), 57, board->arr_board[2][i].color, 0);
		write_char(board->arr_board[3][i].piece, 27+(i*20), 77, board->arr_board[3][i].color, 0);
		write_char(board->arr_board[4][i].piece, 27+(i*20), 97, board->arr_board[4][i].color, 0);
		write_char(board->arr_board[5][i].piece, 27+(i*20), 117, board->arr_board[5][i].color, 0);
		write_char(board->arr_board[6][i].piece, 27+(i*20), 137, board->arr_board[6][i].color, 0);
		write_char(board->arr_board[7][i].piece, 27+(i*20), 157, board->arr_board[7][i].color, 0);
	}
}

void draw_board(int col, int row){						//TODO try to see if you can implement the UI better[x]
	counter = 0;
	int temp = col;

	//vertical lines
	for(int i = row; i <= 170; i++){
		write_pixel(temp, i, (int)WHITE);
		write_pixel(temp+20, i, (int)WHITE);
		write_pixel(temp+40, i, (int)WHITE);
		write_pixel(temp+60, i, (int)WHITE);
		write_pixel(temp+80, i, (int)WHITE);
		write_pixel(temp+100, i, (int)WHITE);
		write_pixel(temp+120, i, (int)WHITE);
		write_pixel(temp+140, i, (int)WHITE);
		write_pixel(temp+160, i, (int)WHITE);
	}

	//horizontall lines
	for(int i = col; i <= 180; i++){
		write_pixel(i, row, (int)WHITE);
		write_pixel(i, row+20, (int)WHITE);
		write_pixel(i, row+40, (int)WHITE);
		write_pixel(i, row+60, (int)WHITE);
		write_pixel(i, row+80, (int)WHITE);
		write_pixel(i, row+100, (int)WHITE);
		write_pixel(i, row+120, (int)WHITE);
		write_pixel(i, row+140, (int)WHITE);
		write_pixel(i, row+160, (int)WHITE);
	}
}

