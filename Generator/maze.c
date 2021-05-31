#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CORNER '+'
#define WALL_H '-'
#define WALL_V '|'
#define ROOM ' '

typedef struct _room {
	int set, down, right;
} room;

void generator(room**, int, int);
void set_correction(room**, int, int);

int main() {
	FILE* output = fopen("maze.maz", "w");
	int n = 0, m = 0;
	room** maze;

	scanf("%d %d", &n, &m);

	maze = malloc(sizeof(room*) * m);
	for (int i = 0; i < m; i++)
		maze[i] = malloc(sizeof(room) * n);

	generator(maze, n, m);

	for (int i = 0; i < 2 * m + 1; i++) {
		for (int j = 0; j < 2 * n + 1; j++) {
			if (!(i % 2)) {
				if (!(j % 2)) {
					fprintf(output, "%c", CORNER);
				}
				else {
					if (i == 0 || i == 2 * m || maze[(i - 1) / 2][(j - 1) / 2].down == 0)
						fprintf(output, "%c", WALL_H);
					else
						fprintf(output, "%c", ROOM);
				}
			}
			else {
				if (!(j % 2)) {
					if (j == 0 || j == 2 * n || maze[(i - 1) / 2][(j - 1) / 2].right == 0)
						fprintf(output, "%c", WALL_V);
					else
						fprintf(output, "%c", ROOM);
				}
				else {
					fprintf(output, "%c", ROOM);
				}
			}
		}
		fprintf(output, "\n");
	}

	for (int i = 0; i < m; i++)
		free(maze[i]);
	free(maze);
	fclose(output);
	return 0;
}

void generator(room** maze, int n, int m) {
	int set_number = 1;
	int set_index = 0;
	int index = 0;
	srand(time(NULL));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			maze[i][j].down = 1;
			maze[i][j].right = 0;
		}
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (i == 0) {
				maze[i][j].set = j + 1;
				index++;
			}
			else if (maze[i - 1][j].down) maze[i][j].set = maze[i - 1][j].set;
			else { maze[i][j].set = index++; }
			if (j < n - 1) maze[i][j].right = rand() % 2;
			if (j > 0 && maze[i][j - 1].right) maze[i][j].set = maze[i][j - 1].set;
		}
		for (int j = 0; j < n; j++) {
			set_correction(maze, j, i);
			if (i < m - 1) {
				maze[i][j].down = rand() % 2;
				if (i > 0 && maze[i - 1][j].down) maze[i][j].down = 0;
				if (maze[i][j].down) set_index = 1;
				if (set_index == 0 && i < m - 1 && maze[i][j].set != maze[i][j + 1].set) {
					maze[i][j].down = 1;
				}
			}
			else if (j > 0 && maze[i][j - 1].set != maze[i][j].set) {
				maze[i][j - 1].right = 1;
			}
			set_index = 0;

			if (i > 0 && j > 0 && maze[i - 1][j - 1].right && maze[i - 1][j - 1].down && maze[i][j - 1].right && maze[i - 1][j].down) {
				switch (rand() % 4) {
				case 0: maze[i - 1][j - 1].down = 0; break;
				case 1: maze[i - 1][j - 1].right = 0; break;
				case 2: maze[i][j - 1].right = 0; break;
				case 3: maze[i - 1][j].down = 0; break;
				}
			}
		}
	}
}

void set_correction(room** maze, int n, int m) {
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			if (maze[i - 1][j].down) maze[i][j].set = maze[i - 1][j].set;
			if (maze[i][j - 1].right) maze[i][j].set = maze[i][j - 1].set;
		}
	}
}