#include <iostream>
#include <stdlib.h>
#include <time.h>
#include<limits.h>
#include "../include/algorithm.h"
using namespace std;
#define AIdepth 4
#define min(x, y) (x < y ? x : y)
#define max(x, y) (x > y ? x : y)

Player my_player('r');
int valid_spots[AIdepth + 1][30][2] = {0};
int valid_spots_num[AIdepth + 1];
int best_x, best_y;

void set_valid_spots(Board board, Player player, int depth){
    char color = player.get_color();

    valid_spots_num[depth] = 0;
    for(int row = 0;row < ROW;row++){
        for(int col = 0;col < COL;col++){
            if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w'){
                valid_spots[depth][valid_spots_num[depth]][0] = row;
                valid_spots[depth][valid_spots_num[depth]][1] = col;
                valid_spots_num[depth]++;
            }else continue;
        }
    }
}

int state_value(Board board, Player player){
    char color = player.get_color();

    int vector[8][2] = {{0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}};
    bool record[ROW][COL];
    int value = 0, wflag = 1, lflag = 1;

    //record contigous blocks.
    for(int row = 0;row < ROW;row++){
        for(int col = 0;col < COL;col++) record[row][col] = false;
    }   
 
    for(int row = 0;row < ROW;row++){
        for(int col = 0;col < COL;col++){
            //not lose yet. it is my cell.
            if(board.get_cell_color(row,col) == color) {
                lflag = 0;

                bool surrounded = false;
                int my_orbs = board.get_orbs_num(row, col);
                value += my_orbs;
                for(int i = 0;i < 8;i++){
                    int sur_row = row + vector[i][0];
                    int sur_col = col + vector[i][1];
                    if(sur_row < 0 || sur_row > 4 || sur_col < 0 || sur_col > 5 ) continue;
                    else if(board.get_cell_color(sur_row, sur_col) == color){
                        if(my_orbs + 1 == board.get_capacity(row, col) && board.get_orbs_num(sur_row, sur_col) + 1 == board.get_capacity(sur_row, sur_col) && !record[sur_row][sur_col]){
                            value += 2 * board.get_orbs_num(sur_row, sur_col);
                            record[sur_row][sur_col] == true;
                        }else continue;
                    }else if(board.get_orbs_num(sur_row, sur_col) + 1 == board.get_capacity(sur_row, sur_col)){
                        surrounded = true;
                        int sur_orbs = board.get_orbs_num(sur_row, sur_col);
                        value -=  (8 - board.get_capacity(sur_row, sur_col));
                    }
                }
                //not surrounded
                if(!surrounded){
                    if(row == 0 || row == 4 || col == 0 || col == 5){
                        if((row == 0 && col == 0) || (row == 4 && col == 0) || (row == 0 && col == 5) || (row == 4 && col == 5)){
                            if(my_orbs + 1 == board.get_capacity(row, col)) value -= 3 * my_orbs;
                            else value += 3 * my_orbs;      
                        }else{
                            if(my_orbs + 1 == board.get_capacity(row, col)) value -= 2 * my_orbs;
                            else value += 2 * my_orbs;   
                        }
                    }
                }
                
            }
            //not win yet. if it is enemy's cell
            if(board.get_cell_color(row,col) != color && board.get_cell_color(row,col) != 'w') {
                wflag = 0;
                int enemy_orbs = board.get_orbs_num(row, col);
                value -= enemy_orbs;
            }
        }
    }
    if(wflag) value += 10000;
    else if(lflag) value -= 10000;
    return value;
}

int minmax(Board board, Player player, int depth, int a, int b){
    if(depth == 0 || valid_spots_num[depth] == 0){
        return state_value(board, my_player);
    }else if(my_player.get_color() == player.get_color()){
        //change to next player.
        char next_color = (player.get_color() == 'r') ? 'b' : 'r';
        Player next_player(next_color);

        //choose the max.
        int tmp_a = a;
        for(int i = 0;i < valid_spots_num[depth];i++){
            Board tmpB = board;
            tmpB.place_orb(valid_spots[depth][i][0], valid_spots[depth][i][1], &player);
            if(depth > 1) set_valid_spots(tmpB, next_player, depth - 1);
            a = max(a, minmax(tmpB, next_player, depth - 1, a, b));
            if(tmp_a != a && depth == AIdepth){
                best_x = valid_spots[depth][i][0];
                best_y = valid_spots[depth][i][1];
                tmp_a = a;
            }
            if(b <= a) break;
        }
        return a;
    }else{
        //change to next player.
        char next_color = (player.get_color() == 'r') ? 'b' : 'r';
        Player next_player(next_color);

        //choose the min
        for(int i = 0;i < valid_spots_num[depth];i++){
            Board tmpB = board;
            tmpB.place_orb(valid_spots[depth][i][0], valid_spots[depth][i][1], &player);
            if(depth > 1) set_valid_spots(tmpB, next_player, depth - 1);
            b = min(b, minmax(tmpB, next_player, depth - 1, a, b));
            if(b <= a) break;
        }
        return b; 
    }
}

void algorithm_A(Board board, Player player, int index[]){
    //first set disc
    my_player = player;
    set_valid_spots(board, my_player, AIdepth);
    //minmax
    minmax(board, player, AIdepth, INT_MIN, INT_MAX);
    index[0] = best_x;
    index[1] = best_y;
}