#include "functii.h"

int check_rotate(Image* image, char buffer[], int x1, int y1, int x2, int y2) {
	if (image == NULL) {
		printf("No image loaded\n");
		return 1;
	}
	if ((x2 - x1) != (y2 - y1)){
		if ((x2 - x1) != image->width && (y2 - y1) != image->height) {
			printf("The selection must be square\n");
			return 1;
		}
	}
	buffer = strtok(NULL, " ");
	int angle;
	if (sscanf(buffer, "%d", &angle) == 1) {
		if (angle == 90 || angle == -90) {
			return angle;
		}
		if (angle == 180 || angle == -180) {
			return angle;
		}
		if (angle == 270 || angle == -270) {
			return angle;
		}
		if (angle == 360 || angle == -360) {
			return angle;
		}
		if (angle == 0) {
			return angle;
		}
		printf("Unsupported rotation angle\n");
		return 1;
	} else {
		printf("Invalid command\n");
		return 1;
	}
}

void rotate_90_gray(Image** image, int x1, int y1, int x2, int y2, unsigned char ***gray) {
	unsigned char **rotate;
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		rotate = malloc((*image)->width * sizeof(unsigned char *));
		for (int i = 0; i < (*image)->width; i++) {
			rotate[i] = malloc((*image)->height * sizeof(unsigned char));
		}
	} else {
		rotate = malloc((*image)->height * sizeof(unsigned char *));
		for (int i = 0; i < (*image)->height; i++) {
			rotate[i] = malloc((*image)->width * sizeof(unsigned char));
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			for (int j = 0; j < (*image)->width; j++) {
				rotate[j][(*image)->height - 1 - i] = (*gray)[i][j];
			}
		}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				rotate[j][(y2 - y1) - 1 - i] = (*gray)[i][j];
			}
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			free((*gray)[i]);
		}
		free((*gray));
		swap(&(*image)->height, &(*image)->width);
		(*gray) = rotate;
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				(*gray)[i][j] = rotate[i][j];
			}
		}
		for (int i = 0; i < (*image)->height; i++) {
			free(rotate[i]);
		}
		free(rotate);
	}
}

void rotate_neg_90_gray(Image** image, int x1, int y1, int x2, int y2, unsigned char ***gray) {
	unsigned char **rotate;
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		rotate = malloc((*image)->width * sizeof(unsigned char *));
		for (int i = 0; i < (*image)->width; i++) {
			rotate[i] = malloc((*image)->height * sizeof(unsigned char));
		}
	} else {
		rotate = malloc((*image)->height * sizeof(unsigned char *));
		for (int i = 0; i < (*image)->height; i++) {
			rotate[i] = malloc((*image)->width * sizeof(unsigned char));
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			for (int j = 0; j < (*image)->width; j++) {
				rotate[i][(*image)->width - 1 - j] = (*gray)[j][i];
			}
		}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				rotate[i][(x2 - x1) - 1 - j] = (*gray)[j][i];
			}
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			free((*gray)[i]);
		}
		free((*gray));
		swap(&(*image)->height, &(*image)->width);
		(*gray) = rotate;
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				(*gray)[i][j] = rotate[i][j];
			}
		}
		for (int i = 0; i < (*image)->height; i++) {
			free(rotate[i]);
		}
		free(rotate);
	}
}

void rotate_90_rgb(Image** image, int x1, int y1, int x2, int y2, RGB ***rgb) {
	RGB **rotate;
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		rotate = malloc((*image)->width * sizeof(RGB *));
		for (int i = 0; i < (*image)->width; i++) {
			rotate[i] = malloc((*image)->height * sizeof(RGB));
		}
	} else {
		rotate = malloc((*image)->height * sizeof(RGB *));
		for (int i = 0; i < (*image)->height; i++) {
			rotate[i] = malloc((*image)->width * sizeof(RGB));
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)){
		for (int i = 0; i < (*image)->height; i++) {
			for (int j = 0; j < (*image)->width; j++) {
				rotate[j][(*image)->height - 1 - i].r = (*rgb)[i][j].r;
				rotate[j][(*image)->height - 1 - i].g = (*rgb)[i][j].g;
				rotate[j][(*image)->height - 1 - i].b = (*rgb)[i][j].b;
			}
		}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				rotate[j][(y2 - y1) - 1 - i].r = (*rgb)[i][j].r;
				rotate[j][(y2 - y1) - 1 - i].g = (*rgb)[i][j].g;
				rotate[j][(y2 - y1) - 1 - i].b = (*rgb)[i][j].b;
			}
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			free((*rgb)[i]);
		}
		free((*rgb));
		swap(&(*image)->height, &(*image)->width);
		(*rgb) = rotate;
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				(*rgb)[i][j].r = rotate[i][j].r;
				(*rgb)[i][j].g = rotate[i][j].g;
				(*rgb)[i][j].b = rotate[i][j].b;

			}
		}
		for (int i = 0; i < (*image)->height; i++) {
			free(rotate[i]);
		}
		free(rotate);
	}
	
}

void rotate_neg_90_rgb(Image** image, int x1, int y1, int x2, int y2, RGB ***rgb) {
	RGB **rotate;
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)){
		rotate = malloc((*image)->width * sizeof(RGB *));
		for (int i = 0; i < (*image)->width; i++) {
			rotate[i] = malloc((*image)->height * sizeof(RGB));
		}
	} else {
		rotate = malloc((*image)->height * sizeof(RGB *));
		for (int i = 0; i < (*image)->height; i++) {
			rotate[i] = malloc((*image)->width * sizeof(RGB));
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)){
		for (int i = 0; i < (*image)->height; i++) {
			for (int j = 0; j < (*image)->width; j++) {
				rotate[i][(*image)->width - 1 - j].r = (*rgb)[j][i].r;
				rotate[i][(*image)->width - 1 - j].g = (*rgb)[j][i].g;
				rotate[i][(*image)->width - 1 - j].b = (*rgb)[j][i].b;

			}
		}
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				rotate[i][(x2 - x1) - 1 - j].r = (*rgb)[j][i].r;
				rotate[i][(x2 - x1) - 1 - j].g = (*rgb)[j][i].g;
				rotate[i][(x2 - x1) - 1 - j].b = (*rgb)[j][i].b;
			}
		}
	}
	if ((x1 == 0 && x2 == 0 && y1 ==0 && y2 == 0) || (x1 == 0 && y1 == 0 && x2 == (*image)->width && y2 == (*image)->height)) {
		for (int i = 0; i < (*image)->height; i++) {
			free((*rgb)[i]);
		}
		free((*rgb));
		swap(&(*image)->height, &(*image)->width);
		(*rgb) = rotate;
	} else {
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				(*rgb)[i][j] = rotate[i][j];
			}
		}
		for (int i = 0; i < (*image)->height; i++) {
			free(rotate[i]);
		}
		free(rotate);
	}
}
