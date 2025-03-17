#ifndef __tetris_h
#define __tetris_h

#include "delay.h"
#include <time.h>
#include <stdlib.h>

#define WIDTH 14
#define HEIGHT 20

typedef unsigned char Tile;

enum BlockType { T, L, I, S, O };

typedef struct {
    Tile array[4];
    char type;
    char rotate;
} Block;

typedef struct {
    char x, y;
    Block block;
} Cursor;

void init_map(void);
void init_cursor(void);
void init_game(void);

void set_block(Block* block, char type, char rotate);
void fell_block(void);
void cw_block(void);
void ccw_block(void);
unsigned char move_block(char direction);
void pos_block(char x_pos);

char game_tick(void);
void stack_block(void);
char clear_line(void);

int collision_check(void);
int full_line_check(int y);
char get_block_pixel(Tile line, int x);
char get_map(int x, int y);

#endif
