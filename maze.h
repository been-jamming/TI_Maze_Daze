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

unsigned int add_coord(maze *m, unsigned char x, unsigned char y, unsigned char direction);

void remove_coord(maze *m, unsigned int coord_id);

maze *create_maze();

void generate_maze(maze *m);

level *create_level();

void set_block(level *l, unsigned char x, unsigned char y);

void delete_block(level *l, unsigned char x, unsigned char y);

unsigned char get_block(level *l, char x, char y);

void display_level(level *l);

void build_level(level *l, maze *m);

level *generate_level();
