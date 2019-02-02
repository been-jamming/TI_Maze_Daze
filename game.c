#define USE_TI89
#define SAVE_SCREEN
#include <tigcclib.h>
#include "maze.h"
#include "render.h"
#include "game.h"

/*
 * Maze Daze game
 *
 * by Ben Jones
 * 2/1/2019
 */

unsigned char collision(level *l, unsigned int x, unsigned int y){
	unsigned char block_x;
	unsigned char block_y;

	block_x = x>>8;
	block_y = y>>8;
	
	if(!(x&0xFF) && !(y&0xFF)){
		return get_block(l, block_x, block_y);
	} else if(!(x&0xFF)){
		return get_block(l, block_x, block_y) || get_block(l, block_x, block_y + 1);
	} else if(!(y&0xFF)){
		return get_block(l, block_x, block_y) || get_block(l, block_x + 1, block_y);
	} else {
		return get_block(l, block_x, block_y) || get_block(l, block_x + 1, block_y) || get_block(l, block_x, block_y + 1) || get_block(l, block_x + 1, block_y + 1);
	}
}

void move_player(level *l, unsigned char direction, int amount){
	if((l->player_direction == LEFT || l->player_direction == RIGHT) && (direction == UP || direction == DOWN)){
		if(l->player_x&0x80){
			l->player_x += 0x100;
		}
		l->player_x &= ~0xFFU;
	} else if((l->player_direction == UP || l->player_direction == DOWN) && (direction == LEFT || direction == RIGHT)){
		if(l->player_y&0x80){
			l->player_y += 0x100;
		}
		l->player_y &= ~0xFFU;
	}
	
	switch(direction){
		case UP:
			if(collision(l, l->player_x, l->player_y - amount)){
				l->player_y &= ~(0xFFU);
			} else {
				l->player_y -= amount;
			}
			break;
		case DOWN:
			if(collision(l, l->player_x, l->player_y + amount)){
				l->player_y = (l->player_y + amount)&~0xFFU;
			} else {
				l->player_y += amount;
			}
			break;
		case LEFT:
			if(collision(l, l->player_x - amount, l->player_y)){
				l->player_x &= ~(0xFFU);
			} else {
				l->player_x -= amount;
			}
			break;
		case RIGHT:
			if(collision(l, l->player_x + amount, l->player_y)){
				l->player_x = (l->player_x + amount)&~0xFFU;
			} else {
				l->player_x += amount;
			}
			break;
	}
}

DEFINE_INT_HANDLER (update){
	if(_keytest(RR_UP)){
		move_player(global_level, UP, 50);
		global_level->player_direction = UP;
		do_render = 1;
	}
	if(_keytest(RR_DOWN)){
		move_player(global_level, DOWN, 50);
		global_level->player_direction = DOWN;
		do_render = 1;
	}
	if(_keytest(RR_LEFT)){
		move_player(global_level, LEFT, 50);
		global_level->player_direction = LEFT;
		do_render = 1;
	}
	if(_keytest(RR_RIGHT)){
		move_player(global_level, RIGHT, 50);
		global_level->player_direction = RIGHT;
		do_render = 1;
	}
	if(_keytest(RR_ESC)){
		do_quit = 1;
	}
}

void _main(){
	randomize();
	clrscr();
	global_level = generate_level();
	display_level(global_level);
	ngetchx();
	render_level(global_level);
	old_int_5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_5, update);
	do_render = 0;
	do_quit = 0;
	while(!do_quit){
		if(do_render){
			render_level(global_level);
		}
	}
	SetIntVec(AUTO_INT_5, old_int_5);
	free(global_level);
}

