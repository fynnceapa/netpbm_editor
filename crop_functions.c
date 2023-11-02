#include "functii.h"

//functie crop pentru o imagine grayscale
int crop_image_gray(Image **image, unsigned char ***pixels, int x1, int y1, int x2, int y2) {
	Image* w = (*image);
	unsigned char **buffer;
	unsigned char **pix = (*pixels);
	//calculez noua inaltime si latime pentru crop
	int new_width = x2 - x1;
	int new_height = y2 - y1;
	//vectori care retin coordonatele fiecarui pixel din selectie
	int lines[new_height], colls[new_width];

	//aloc memorie pentru buffer
	buffer = malloc(new_height * sizeof(unsigned char*));
	if (buffer == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	for (int i = 0; i < new_height; i++) {
		buffer[i] = malloc(new_width * sizeof(unsigned char));
		if (buffer[i] == NULL) {
			perror("Couldn't allocate memory\n");
			exit(0);
		}
	}

	for (int i = 0; i < new_height; i++)
		lines[i] = y1++;
	for (int i = 0; i < new_width; i++)
		colls[i] = x1++;
	
	//in buffer retin pixelii selectati
	for (int i = 0; i < new_height; i++) {
		for(int j = 0; j < new_width; j++) {
			buffer[i][j] = pix[lines[i]][colls[j]];
		}
	}
	//eliberez memoria matricei vechi
	for (int i = 0; i < w->height; i++)
		free(pix[i]);
	free(pix);

	//realoc memorie pentru noile dimensiuni
	pix = malloc(new_height * sizeof(unsigned char*));
	if (pix == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	for (int i = 0; i < new_height; i++) {
		pix[i] = malloc(new_width * sizeof(unsigned char));
		if (pix[i] == NULL) {
			perror("Couldn't allocate memory\n");
			exit(0);
		}
	}

	w->height = new_height;
	w->width = new_width;

	//in noua imagine pun pixelii
	for (int i = 0; i < w->height; i++) {
		for (int j = 0; j < w->width; j++)
			pix[i][j] = buffer[i][j];
	}

	//eliberez memoria pt buffer
	for (int i = 0; i < new_height; i++)
		free(buffer[i]);
	free(buffer);
	(*image) = w;
	(*pixels) = pix;
	printf("Image cropped\n");
	return 1;
}

//functie pentru imagine color, idem cea cu grayscale
int crop_image_color(Image **image, RGB ***pixels, int x1, int y1, int x2, int y2) {
	RGB **buffer;
	int new_width = x2 - x1;
	int new_height = y2 - y1;

	int lines[new_height], colls[new_width];

	buffer = malloc(new_height * sizeof(RGB*));
	if (buffer == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	for (int i = 0; i < new_height; i++) {
		buffer[i] = malloc(new_width * sizeof(RGB));
		if (buffer[i] == NULL) {
			perror("Couldn't allocate memory\n");
			exit(0);
		}
	}
	
	for (int i = 0; i < new_height; i++)
		lines[i] = y1++;
	for (int i = 0; i < new_width; i++)
		colls[i] = x1++;
	
	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
				buffer[i][j].r = (*pixels)[lines[i]][colls[j]].r;
				buffer[i][j].g = (*pixels)[lines[i]][colls[j]].g;
				buffer[i][j].b = (*pixels)[lines[i]][colls[j]].b;
		}
	}
	for (int i = 0; i < (*image)->height; i++)
		free((*pixels)[i]);
	free(*pixels);

	(*pixels) = malloc(new_height * sizeof(RGB*));
	if ((*pixels) == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	for (int i = 0; i < new_height; i++) {
		(*pixels)[i] = malloc(new_width * sizeof(RGB));
		if ((*pixels)[i] == NULL) {
			perror("Couldn't allocate memory\n");
			exit(0);
		}
	}
	
	(*image)->height = new_height;
	(*image)->width = new_width;

	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
			(*pixels)[i][j].r = buffer[i][j].r;
			(*pixels)[i][j].g = buffer[i][j].g;
			(*pixels)[i][j].b = buffer[i][j].b;
		}
	}
	for (int i = 0; i < new_height; i++)
		free(buffer[i]);
	free(buffer);
	printf("Image cropped\n");
	return 1;
}

//functie care verifica selectia si daca avem imagine incarcata si aplica crop
int crop_gray(Image** image, unsigned char ***graypixels, int x1, int y1, int x2, int y2) {
	if ((x2 - x1) == 0 && (y2 - y1) == 0) {
		printf("Image cropped\n");
		return 0;
	}
	if ((*image) == NULL) {
		printf("No image loaded\n");
		return 0;
	}
	crop_image_gray(&(*image), &(*graypixels), x1, y1, x2, y2);
	return 1;
}

int crop_color(Image** image, RGB*** rgbpixels, int x1, int y1, int x2, int y2) {
	if ((x2 - x1) == 0 && (y2 - y1) == 0) {
		printf("Image cropped\n");
		return 0;
	}
	if ((*image) == NULL) {
		printf("No image loaded\n");
		return 0;
	}
	crop_image_color(&(*image), &(*rgbpixels), x1, y1, x2, y2);
}
