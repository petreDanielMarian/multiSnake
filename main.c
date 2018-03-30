#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

#include "main.h"

int main(int argc, char *argv[])
{
	int num_snakes, num_lines, num_cols;
	int **world;
	struct snake *snakes;

	if (argc != 4)
	{
		printf("Need more parameters!");
		exit(1);
	}

	read_data(argv[1], &num_snakes, &snakes, &num_lines, &num_cols, &world);

	// We only take account of run_simulation execution time.
	double start = omp_get_wtime();
	run_simulation(num_lines, num_cols, world, num_snakes, snakes, atoi(argv[3]), argv[2]);
	double end = omp_get_wtime();
	printf("%.9lf\n", end-start);

	print_world(argv[2], num_snakes, snakes, num_lines, num_cols, world);

	return 0;
}


void read_data(char *file_name, int *num_snakes, struct snake **snakes,
	int *num_lines, int *num_cols, int ***world)
{
	int i, j;
	FILE *fp;
	struct snake *s;
	int **w;

	fp = fopen(file_name , "r");
	if (!fp)
	{
		printf("Error opening input file!");
		exit(-1);
	}

	fscanf(fp, "%d", num_snakes);
	s = (struct snake *)malloc(*num_snakes * sizeof(struct snake));
	for (i = 0; i < *num_snakes; i++)
	{
		fscanf(fp, "%d", &s[i].head.line);
		fscanf(fp, "%d", &s[i].head.col);
		fscanf(fp, "%d", &s[i].encoding);
		fscanf(fp, " %c", &s[i].direction);
	}
	*snakes = s;

	fscanf(fp, "%d", num_lines);
	fscanf(fp, "%d\n", num_cols);
	w = (int **)malloc(*num_lines * sizeof(int *));
	for (i = 0; i < *num_lines; i++)
		w[i] = (int *)calloc(*num_cols, sizeof(int));

	for (i = 0; i < *num_lines; i++)
		for (j = 0; j < *num_cols; j++)
			fscanf(fp, "%d ", &w[i][j]);
	*world = w;

	fclose(fp);
}


void print_world(char *file_name, int num_snakes, struct snake *snakes,
	int num_lines, int num_cols, int **world)
{
	int i, j;
	FILE *fp;

	fp = fopen(file_name , "w");
	if (!fp)
	{
		printf("Error opening output file!");
		exit(-1);
	}

	fprintf(fp, "%d\n", num_snakes);
	for (i = 0; i < num_snakes; i++)
		fprintf(fp, "%d %d %d %c\n", snakes[i].head.line, snakes[i].head.col, snakes[i].encoding, snakes[i].direction);

	fprintf(fp, "%d %d\n", num_lines, num_cols);
	for (i = 0; i < num_lines; i++)
	{
		for (j = 0; j < num_cols; j++)
			fprintf(fp, "%d ", world[i][j]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}
