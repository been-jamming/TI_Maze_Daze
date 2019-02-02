#include <tigcclib.h>
#include "extgraph.h"
#include "maze.h"
#include "sprites.c"

/*
 * Maze Daze graphics
 *
 * by Ben Jones
 * 2/1/2019
 */

unsigned char screen_buffer[LCD_SIZE];

void render_level(level *l){
	int start_x;
	int start_y;
	unsigned int offset_x;
	unsigned int offset_y;
	int x;
	int y;
	int block_x;
	int block_y;

	offset_x = (l->player_x)&0xFF;
	offset_y = (l->player_y)&0xFF;

	start_x = (l->player_x>>8) - 10;
	start_y = (l->player_y>>8) - 6;

	FastClearScreen_R(screen_buffer);

	for(x = start_x; x <= start_x + 21; x++){
		for(y = start_y; y <= start_y + 13; y++){
			if(get_block(l, x, y)){
				block_x = (x - start_x)*8 - (offset_x>>5) - 4;
				block_y = (y - start_y)*8 - (offset_y>>5) - 2;
				ClipSprite8_OR_R(block_x, block_y, 8, wall, screen_buffer);
			}
		}
	}

	switch(l->player_direction){
		case LEFT:
			Sprite8_OR_R(76, 46, 8, player_left1, screen_buffer);
			break;
		case RIGHT:
			Sprite8_OR_R(76, 46, 8, player_right1, screen_buffer);
			break;
		case UP:
			Sprite8_OR_R(76, 46, 8, player_up1, screen_buffer);
			break;
		case DOWN:
			Sprite8_OR_R(76, 46, 8, player_down1, screen_buffer);
			break;
	}

	memcpy(LCD_MEM, screen_buffer, LCD_SIZE);
}
