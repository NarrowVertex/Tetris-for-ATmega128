#include "tetris.h"

Tile map[HEIGHT][WIDTH];
Cursor cursor;

void init_map(void)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if ((y == 0 || y == HEIGHT - 1) || (x == 0 || x == WIDTH - 1))
                map[y][x] = 1;
            else
                map[y][x] = 0;
        }
    }
}

void init_cursor(void)
{
    cursor.x = 6;
    cursor.y = 3;
    cursor.block.type = 1;
    cursor.block.rotate = 0;
    set_block(&cursor.block, cursor.block.type, cursor.block.type);
}

void init_game(void)
{
    init_map();
    init_cursor();
}


void set_block(Block* block, char type, char rotate)
{
    switch (type) {
    case 0:                 // T
        switch (rotate) {
        case 0:
            block->array[0] = 0b0000;
            block->array[1] = 0b0100;
            block->array[2] = 0b1110;
            block->array[3] = 0b0000;
            break;

        case 1:
            block->array[0] = 0b00000;
            block->array[1] = 0b00100;
            block->array[2] = 0b00110;
            block->array[3] = 0b00100;
            break;

        case 2:
            block->array[0] = 0b00000;
            block->array[1] = 0b00000;
            block->array[2] = 0b01110;
            block->array[3] = 0b00100;
            break;

        case 3:
            block->array[0] = 0b00000;
            block->array[1] = 0b00100;
            block->array[2] = 0b01100;
            block->array[3] = 0b00100;
            break;
        }
        break;

    case 1:                 // L
        switch (rotate) {
        case 0:
            block->array[0] = 0b00000;
            block->array[1] = 0b00010;
            block->array[2] = 0b01110;
            block->array[3] = 0b00000;
            break;

        case 1:
            block->array[0] = 0b00000;
            block->array[1] = 0b00100;
            block->array[2] = 0b00100;
            block->array[3] = 0b00110;
            break;

        case 2:
            block->array[0] = 0b00000;
            block->array[1] = 0b00000;
            block->array[2] = 0b01110;
            block->array[3] = 0b01000;
            break;

        case 3:
            block->array[0] = 0b00000;
            block->array[1] = 0b01100;
            block->array[2] = 0b00100;
            block->array[3] = 0b00100;
            break;
        }
        break;

    case 2:                 // I
        switch (rotate) {
        case 0:
        case 2:
            block->array[0] = 0b00100;
            block->array[1] = 0b00100;
            block->array[2] = 0b00100;
            block->array[3] = 0b00100;
            break;

        case 1:
        case 3:
            block->array[0] = 0b00000;
            block->array[1] = 0b00000;
            block->array[2] = 0b01111;
            block->array[3] = 0b00000;
            break;
        }
        break;

    case 3:                 // S
        switch (rotate) {
        case 0:
        case 2:
            block->array[0] = 0b00000;
            block->array[1] = 0b01100;
            block->array[2] = 0b00110;
            block->array[3] = 0b00000;
            break;

        case 1:
            block->array[0] = 0b00000;
            block->array[1] = 0b00100;
            block->array[2] = 0b01100;
            block->array[3] = 0b01000;
            break;

        case 3:
            block->array[0] = 0b00000;
            block->array[1] = 0b00010;
            block->array[2] = 0b00110;
            block->array[3] = 0b00100;
            break;
        }
        break;

    case 4:                 // O
        switch (rotate) {
        case 0:
        case 1:
        case 2:
        case 3:
            block->array[0] = 0b00000;
            block->array[1] = 0b01100;
            block->array[2] = 0b01100;
            block->array[3] = 0b00000;
            break;
        }
        break;
    }
    block->type = type;
    block->rotate = rotate;
}

void fell_block(void)
{
    cursor.y += 1;
}

void cw_block(void)
{
    cursor.block.rotate++;
    if (cursor.block.rotate == 4)
        cursor.block.rotate = 0;

    set_block(&cursor.block, cursor.block.type, cursor.block.rotate);

    if (collision_check())
        ccw_block();
}

void ccw_block(void)
{
    cursor.block.rotate--;
    if (cursor.block.rotate == -1)
        cursor.block.rotate = 3;

    set_block(&cursor.block, cursor.block.type, cursor.block.rotate);

    if (collision_check())
        cw_block();
}

unsigned char move_block(char direction)
{
    cursor.x += direction;
    if (collision_check()) {
        cursor.x -= direction;
        return 0;
    }
    return 1;
}

void pos_block(char x_pos)
{
    int limit_count = 0;
    while(x_pos != cursor.x)
    {
        if(x_pos > cursor.x)
        {
            if(!move_block(-1))
                break;
        }
        else
        {
            if(!move_block(1))
                break;
        }
        limit_count++;
        if(limit_count > 20)
            return;
    }
}


char game_tick(void)
{
    int flag = 0;

    fell_block();
    if (collision_check())
    {
        cursor.y -= 1;
        stack_block();
        flag = clear_line();

        init_cursor();
    }
    return flag;
}

void stack_block(void)
{
    int startX = cursor.x - 2;
    if (startX < 0)
        startX = 0;

    int startY = cursor.y - 2;
    if (startY < 0)
        startY = 0;

    int endX = cursor.x + 2;
    if (endX > WIDTH - 1)
        endX = WIDTH - 1;

    int endY = cursor.y + 1;
    if (endY > HEIGHT - 1)
        endY = HEIGHT - 1;

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (get_block_pixel(cursor.block.array[y - (cursor.y - 2)], x - (cursor.x - 2)) == 1)
                map[y][x] = 1;
        }
    }
}

char clear_line(void)
{
    char flag = 0;
    for (int y = 1; y < HEIGHT - 1; y++)
    {
        if (full_line_check(y))
        {
            for (int y2 = y; y2 > 1; y2--)
            {
                for (int x = 1; x < WIDTH - 1; x++)
                {
                    map[y2][x] = map[y2 - 1][x];
                }
            }
            for (int x = 1; x < WIDTH - 1; x++)
            {
                map[1][x] = 0;
            }
            flag = 1;
        }
    }
    return flag;
}

int collision_check(void)
{
    int startX = cursor.x - 2;
    if (startX < 0)
        startX = 0;

    int startY = cursor.y - 2;
    if (startY < 0)
        startY = 0;

    int endX = cursor.x + 2;
    if (endX > WIDTH - 1)
        endX = WIDTH - 1;

    int endY = cursor.y + 1;
    if (endY > HEIGHT - 1)
        endY = HEIGHT - 1;

    for (int y = startY; y <= endY; y++)
    {
        for (int x = startX; x <= endX; x++)
        {
            if (get_block_pixel(cursor.block.array[y - (cursor.y - 2)], x - (cursor.x - 2)) == 1 && map[y][x] == 1)
                return 1;
        }
    }

    return 0;
}

int full_line_check(int y)
{
    for (int x = 1; x < WIDTH - 1; x++)
    {
        if (map[y][x] == 0)
            return 0;
    }
    return 1;
}

char get_block_pixel(Tile line, int x)
{
    return (line & (1 << (4 - x))) >> (4 - x);
}

char get_map(int x, int y)
{
    return map[y][x];
}
