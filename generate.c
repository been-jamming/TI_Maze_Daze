#define USE_TI89
#define SAVE_SCREEN
#include <tigcclib.h>
#include "extgraph.h"
#include "maze.h"

/*
 * Prim's Algorithm on Ti89 for maze generation
 *
 * by Ben Jones
 * 1/30/2019
 */

unsigned int add_coord(maze *m, unsigned char x, unsigned char y, unsigned char direction){
	m->unexplored[m->num_unexplored] = (coord) {.x = x, .y = y, .direction = direction};

	return m->num_unexplored++;
}

void remove_coord(maze *m, unsigned int coord_id){
	coord temp;

	m->num_unexplored--;
	temp = m->unexplored[m->num_unexplored];
	m->unexplored[m->num_unexplored] = m->unexplored[coord_id];
	m->unexplored[coord_id] = temp;
}

maze *create_maze(){
	maze *m;

	m = calloc(1, sizeof(maze));
	add_coord(m, rand()%48 + 1, rand()%48 + 1, 1<<(rand()%4));

	return m;
}

void generate_maze(maze *m){
	unsigned int square;
	coord c;

	while(m->num_unexplored){
		square = rand()%m->num_unexplored;
		c = m->unexplored[square];

		while(m->num_unexplored && m->squares[c.x][c.y]){
			remove_coord(m, square);
			if(square){
				square--;
			}
			c = m->unexplored[square];
		}
		if(!m->num_unexplored){
			return;
		}

		switch(c.direction){
			case UP:
				m->squares[c.x][c.y] = UP;
				m->squares[c.x][c.y - 1] |= DOWN;
				break;
			case DOWN:
				m->squares[c.x][c.y] = DOWN;
				m->squares[c.x][c.y + 1] |= UP;
				break;
			case LEFT:
				m->squares[c.x][c.y] = LEFT;
				m->squares[c.x - 1][c.y] |= RIGHT;
				break;
			case RIGHT:
				m->squares[c.x][c.y] = RIGHT;
				m->squares[c.x + 1][c.y] |= LEFT;
				break;
		}

		if(c.x && !m->squares[c.x - 1][c.y]){
			add_coord(m, c.x - 1, c.y, RIGHT);
		}
		if(c.x < 49 && !m->squares[c.x + 1][c.y]){
			add_coord(m, c.x + 1, c.y, LEFT);
		}
		if(c.y && !m->squares[c.x][c.y - 1]){
			add_coord(m, c.x, c.y - 1, DOWN);
		}
		if(c.y < 49 && !m->squares[c.x][c.y + 1]){
			add_coord(m, c.x, c.y + 1, UP);
		}

		remove_coord(m, square);
	}
}

level *create_level(){
	level *output;

	output = malloc(sizeof(level));
	
	return output;
}

void set_block(level *l, unsigned char x, unsigned char y){
	l->blocks[x>>3][y] |= 1U<<(x&7);
}

void delete_block(level *l, unsigned char x, unsigned char y){
	l->blocks[x>>3][y] &= ~(1U<<(x&7));
}

unsigned char get_block(level *l, unsigned char x, unsigned char y){
	return l->blocks[x>>3][y]&(1U<<(x&7));
}

void build_level(level *l, maze *m){
	unsigned char x;
	unsigned char y;
	
	for(x = 0; x < 99; x++){
		for(y = 0; y < 99; y++){
			if(!x || !y || x == 98 || y == 98){
				set_block(l, x, y);
			} else if(!(x&1) || !(y&1)){
				if(!(x&1) && !(y&1)){
					set_block(l, x, y);
				} else if(!(x&1)){
					if(!(m->squares[(x - 1)>>1][(y - 1)>>1]&RIGHT)){
						set_block(l, x, y);
					} else {
						delete_block(l, x, y);
					}
				} else if(!(y&1)){
					if(!(m->squares[(x - 1)>>1][(y - 1)>>1]&DOWN)){
						set_block(l, x, y);
					} else {
						delete_block(l, x, y);
					}
				}
			} else {
				delete_block(l, x, y);
			}
		}
	}
}

void display_level(level *l){
	unsigned char x;
	unsigned char y;

	for(x = 0; x < 99; x++){
		for(y = 0; y < 99; y++){
			if(get_block(l, x, y)){
				EXT_SETPIX(LCD_MEM, x + 30, y);
			} else {
				EXT_CLRPIX(LCD_MEM, x + 30, y);
			}
		}
	}
}

level *generate_level(){
	level *output;
	maze *m;

	m = create_maze();
	generate_maze(m);
	output = create_level();
	build_level(output, m);
	free(m);

	output->player.x = (rand()%49 + 1)>>1 - 1;
	output->player.y = (rand()%49 + 1)>>1 - 1;

	return output;
}

void _main(){
	level *l;

	randomize();
	clrscr();
	l = generate_level();
	display_level(l);
	free(l);
	ngetchx();
}
