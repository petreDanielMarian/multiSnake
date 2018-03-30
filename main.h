#ifndef __MAIN_H__
#define __MAIN_H__

struct coord
{
	int line;
	int col;
};

struct snake
{
	struct coord head;
	int encoding;
	char direction;
};

void print_world(char *file_name, int num_snakes, struct snake *snakes,
	int num_lines, int num_cols, int **world);

void read_data(char *file_name, int *num_snakes, struct snake **snakes,
	int *num_lines, int *num_cols, int ***world);

void run_simulation(int num_lines, int num_cols, int **world, int num_snakes,
	struct snake *snakes, int step_count, char *file_name);

#endif