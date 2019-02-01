/*
 * Prim's Algorithm on Ti89 for maze generation
 *
 * by Ben Jones
 * 1/30/2019
 */

#define UP 1
#define LEFT 2
#define DOWN 4
#define RIGHT 8

typedef struct coord coord;

struct coord{
	unsigned char x;
	unsigned char y;
	unsigned char direction;
};

typedef struct maze maze;

struct maze{
	unsigned char squares[50][50];
	coord unexplored[2500];
	unsigned int num_unexplored;
};

typedef struct level level;

struct level{
	unsigned char blocks[13][99];
	unsigned char visible[13][99];
	unsigned int player_x;
	unsigned int player_y;
	unsigned char player_direction;
};
