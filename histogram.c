#include "functii.h"

int check_histogram(Image *image, char buffer[], int *x, int *y) {
	if (image == NULL) {
		printf("No image loaded\n");
		return 0;
	}
	buffer = strtok(NULL, " ");
	if (buffer == NULL || sscanf(buffer, "%d", &(*x)) != 1) {
		printf("Invalid command\n");
		return 0;
	}
	buffer = strtok(NULL, " ");
	if (buffer == NULL || sscanf(buffer, "%d", &(*y)) != 1) {
		printf("Invalid command\n");
		return 0;
	}
	if (image->type == 3 || image->type == 6) {
		printf("Black and white image needed\n");
		return 0;
	}
	buffer = strtok(NULL, " ");
	if (buffer != NULL) {
		printf("Invalid command\n");
		return 0;
	}
	return 1;
}

void print_histogram (Image* image, unsigned char **pixels, int x, int y) {
	if (y < 2 || y > 256 || (y & (y - 1)) != 0) {
		printf("Invalid set of parameters\n");
		return;
	}

	int *histogram = calloc(y, sizeof(int));
	if (histogram == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}

	int *frv = calloc(256, sizeof(int));
	if (frv == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}

	int perioada = 256 / y;

	for (int i = 0; i < image->height; i++) {
		for (int j = 0; j < image->width; j++) {
			frv[pixels[i][j]]++;
		}
	}
	for (int i = 0; i < 256; i+=perioada) {
		for (int j = 0; j < perioada; j++) {
			histogram[i / perioada] += frv[i + j];
		}
	}
	int maxx = 0;
	for (int i = 0; i < y; i++) {
		if (histogram[i] > maxx)
			maxx = histogram[i];
	}
	for (int i = 0; i < y; i++) {
		printf("%d\t|\t", x * histogram[i] / maxx);
		for (int j = 0; j < x * histogram[i] / maxx; j++) {
			printf("*");
		}
		printf("\n");
	}
	free(histogram);
	free(frv);
}

void equalize_image(Image **image, unsigned char ***pixels) {
	if ((*image) == NULL) {
		printf("No image loaded\n");
		return;
	}
	Image* w = (*image);
	if (w->type == 3 || w->type == 6) {
		printf("Black and white image needed\n");
		return;
	}
	int *frv = calloc(256, sizeof(int));
	if (frv == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	for (int i = 0; i < w->height; i++) {
		for (int j = 0; j < w->width; j++) {
			frv[(*pixels)[i][j]]++;
		}
	}
	double area = w->height * w->width;
	for (int i = 0; i < w->height; i++) {
		for (int j = 0; j < w->width; j++) {
			double new_pixel = 0;
			for (int k = 0; k <= (*pixels)[i][j]; k++) {
				new_pixel += frv[k];
			}
			new_pixel = 255 * new_pixel / area;
			(*pixels)[i][j] = round(clamp(new_pixel));
		}
	}
	free(frv);
	(*image) = w;  
	printf("Equalize done\n");
}