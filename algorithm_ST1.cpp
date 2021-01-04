#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
char color_me, color_op;
int bestRow, bestCol;

int evaluator(Board board) {
    int orb_me = 0, orb_op = 0;
    int evaluation = 0;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 6; j++) {
            if (board.get_cell_color(i, j) == color_me) {
                orb_me += board.get_orbs_num(i, j);
            } else if (board.get_cell_color(i, j) == color_op) {
                orb_op += board.get_orbs_num(i, j);
            }
        }
    }
    evaluation = orb_me - orb_op;
    return evaluation;
}

int minimax(int depth, bool is_me, Board board, int alpha, int beta, Player me, Player op) {

    if(depth == 5) 
        return evaluator(board);
    else {
        int bestVal;
        if(is_me) {
            bestVal = -100000;
            bool prune = false;
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 6; j++) {
                    Board new_board = board;
                    if(new_board.place_orb(i, j, &me)) {
                        int value = minimax(depth + 1, false, new_board, alpha, beta, me, op);
                        if(value > bestVal) {
                            bestVal = value;
                            if(depth == 0) {
                                bestRow = i;
                                bestCol = j;
                            }
                        }
                        alpha = (bestVal > alpha) ? bestVal : alpha;
                        if(beta <= alpha) {
                            prune = true;
                            break;
                        }
                    }
                }
                if(prune)
                    break;
            }
            return bestVal;
        } else {
            bestVal = 100000;
            bool prune = false;
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 6; j++) {
                    Board new_board = board;
                    if(new_board.place_orb(i, j, &op)) {
                        int value = minimax(depth + 1, true, new_board, alpha, beta, me, op);
                        bestVal = (value < bestVal) ? value : bestVal;
                        beta = (bestVal < beta) ? bestVal : beta;
                        if(beta <= alpha) {
                            prune = true;
                            break;
                        }
                    }
                }
                if(prune)
                    break;
            }
            return bestVal;
        }
    }  
}

void algorithm_B(Board board, Player player, int index[]){

    //////your algorithm design///////////
    Player blue('b');
    Player red('r');
    Board tmp = board;
    bestRow = 0, bestCol = 0;
    if(player.get_color() == 'r') {
        color_me = 'r';
        color_op = 'b';
        minimax(0, true, tmp, -10000, 10000, red, blue);
    } else {
        color_me = 'b';
        color_op = 'r';
        minimax(0, true, tmp, -10000, 10000, blue, red); 
    }
    
    index[0] = bestRow;
    index[1] = bestCol;
    
}