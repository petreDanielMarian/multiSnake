#include <string.h>
#include <omp.h>
#include <stdlib.h>

#include "main.h"

struct coord getPos(int initPosLine, int initPosCol, int num_lines,
		int num_cols, char direction) {
	struct coord pos;

	pos.line = initPosLine;
	pos.col = initPosCol;

	if (direction == 'N') {
		if (pos.line > 0) {
			pos.line = pos.line - 1;
		} else {
			pos.line = num_lines - 1;
		}
	} else if (direction == 'E') {
		if (pos.col < num_cols - 1) {
			pos.col = pos.col + 1;
		} else {
			pos.col = 0;
		}
	} else if (direction == 'S') {
		if (pos.line < num_lines - 1) {
			pos.line = pos.line + 1;
		} else {
			pos.line = 0;
		}
	} else if (direction == 'V') {
		if (pos.col > 0) {
			pos.col = pos.col - 1;
		} else {
			pos.col = num_cols - 1;
		}
	}

	return pos;
}

struct coord getTail(int num_lines, int num_cols, int** world, int headLine,
		int headCol, int code) {

	int i;
	char direction[4] = { 'N', 'E', 'S', 'V' };
	struct coord actualTile, verify;
	char arrivedFrom = 'O';
	int tailFound = 0, stop = 0;

	actualTile.line = headLine;
	actualTile.col = headCol;

	while (0 != 1) {
		tailFound = 1;
		for (i = 0; i < 4; i++) {
			verify = getPos(actualTile.line, actualTile.col, num_lines,
					num_cols, direction[i]);

			if (world[verify.line][verify.col] == code) {
				if (arrivedFrom != direction[i]) {

					if (direction[i] == 'N') {
						arrivedFrom = 'S';
					} else if (direction[i] == 'S') {
						arrivedFrom = 'N';
					} else if (direction[i] == 'E') {
						arrivedFrom = 'V';
					} else if (direction[i] == 'V') {
						arrivedFrom = 'E';
					}

					actualTile.line = verify.line;
					actualTile.col = verify.col;
					tailFound = 0;
					break;
				}
			}
		}
		if (tailFound == 1) {
			break;
		}
	}
	return actualTile;
}

void run_simulation(int num_lines, int num_cols, int **world, int num_snakes,
		struct snake *snakes, int step_count, char *file_name) {
	int i, j, k, r, gameOver = 0, stop = 0, collision = 0;
	char direction[4] = { 'N', 'E', 'S', 'V' };
	struct coord movePos[num_snakes];
	struct coord tailPos[num_snakes];
	struct coord newTail;

	omp_set_num_threads(2);
	// omp_set_num_threads(4);
	// omp_set_num_threads(6);
	// omp_set_num_threads(8);


	// find all the tails
	#pragma omp parallel for
	for (i = 0; i < num_snakes; i++) {
		tailPos[i] = getTail(num_lines, num_cols, world, snakes[i].head.line,
				snakes[i].head.col, snakes[i].encoding);
	}

	// make a copy of the world matrix
	
	int **worldCopy = (int **) malloc(sizeof(int *) * num_lines);
	#pragma omp parallel for shared(worldCopy)
	for (i = 0; i < num_lines; i++) {
		worldCopy[i] = (int*) malloc(sizeof(int) * num_cols);
	}

	#pragma omp parallel for shared(worldCopy)
	for (i = 0; i < num_lines; i++) {
		memcpy(worldCopy[i], world[i], num_cols * sizeof(int));
	}

	// start the game
	for (j = 0; j < step_count; j++) {

		// delete the tails
		#pragma omp parallel for private(stop, newTail, k)
		for (i = 0; i < num_snakes; i++) {
			for (k = 0; k < 4; k++) {
				stop = 0;
				worldCopy[tailPos[i].line][tailPos[i].col] = 0;

				newTail = getPos(tailPos[i].line, tailPos[i].col, num_lines,
						num_cols, direction[k]);

				if (worldCopy[newTail.line][newTail.col]
						== snakes[i].encoding) {
					tailPos[i] = newTail;
					stop = 1;
				}
				if (stop == 1) {
					k = 4;
				}
			}
		}

		// update the location of the snake's head
		#pragma omp parallel for
		for (i = 0; i < num_snakes; i++) {

			movePos[i].line = snakes[i].head.line;
			movePos[i].col = snakes[i].head.col;

			if (snakes[i].direction == 'N') {
				if (movePos[i].line > 0) {
					movePos[i].line = movePos[i].line - 1;
				} else {
					movePos[i].line = num_lines - 1;
				}
			} else if (snakes[i].direction == 'E') {
				if (movePos[i].col < num_cols - 1) {
					movePos[i].col = movePos[i].col + 1;
				} else {
					movePos[i].col = 0;
				}
			} else if (snakes[i].direction == 'S') {
				if (movePos[i].line < num_lines - 1) {
					movePos[i].line = movePos[i].line + 1;
				} else {
					movePos[i].line = 0;
				}
			} else if (snakes[i].direction == 'V') {
				if (movePos[i].col > 0) {
					movePos[i].col = movePos[i].col - 1;
				} else {
					movePos[i].col = num_cols - 1;
				}
			}

			if (worldCopy[movePos[i].line][movePos[i].col] == 0) {

				worldCopy[movePos[i].line][movePos[i].col] = snakes[i].encoding;

			} else {
				collision = 1;
				gameOver = 1;
				i = num_snakes;
			}
		}

		// get the right output out of the game
		if (collision == 0) {

			#pragma omp parallel for
			for (i = 0; i < num_lines; i++) {
				memcpy(world[i], worldCopy[i], num_cols * sizeof(int));
			}

			#pragma omp parallel for
			for (i = 0; i < num_snakes; i++) {
				snakes[i].head = movePos[i];
			}
		} else {
			j = step_count;
		}

	}
}
