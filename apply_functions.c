#include "functii.h"

//in cazul in care selectia contine marginea pozei, o marim/scadem cu un pixel
void image_margin(Image* image, int *x1, int *y1, int *x2, int *y2) {
	if ((*x1) == 0)
		(*x1)++;
	if ((*x2) == image->width)
		(*x2)--;
	if ((*y1) == 0)
		(*y1)++;
	if ((*y2) == image->height)
		(*y2)--;
}

void temp_to_rgb(RGB ***rgb, RGB **temp, int i, int j) {
	(*rgb)[i][j].r = temp[i][j].r;
	(*rgb)[i][j].g = temp[i][j].g;
	(*rgb)[i][j].b = temp[i][j].b;
}

//functie care aplica un kernel in matricea temporaara de pixeli
void apply_kernel_in_temp(Image *image, double kernel[3][3], RGB **rgbpixels, RGB ***temp) {
	double new_pixel_r = 0, new_pixel_b = 0, new_pixel_g = 0;
	for (int i = 1; i < image->height - 1; i++) {
		for (int j = 1; j < image->width - 1; j++) {
			new_pixel_b = 0;
			new_pixel_g = 0;
			new_pixel_r = 0;
			//ne "plimbam" pe un cadran de cate trei elemente
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					new_pixel_r += rgbpixels[i + k][j + l].r * kernel[k + 1][l + 1];
					new_pixel_g += rgbpixels[i + k][j + l].g * kernel[k + 1][l + 1];
					new_pixel_b += rgbpixels[i + k][j + l].b * kernel[k + 1][l + 1];
				}
			}
			//in temp pun valorile noului pixel rotunjit superior si aplicand clamp
			(*temp)[i][j].r = round(clamp(new_pixel_r));
			(*temp)[i][j].g = round(clamp(new_pixel_g));
			(*temp)[i][j].b = round(clamp(new_pixel_b));
		}
	}
}

RGB** gaussian_blur(Image *image, RGB ***rgbpixels, int x1, int y1, int x2, int y2, int ok) {
	double kernel[3][3] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] /= 16;
	RGB **temp = malloc(image->height * sizeof(RGB*));
	if (temp == NULL) {
		printf("Couldn't allocate memory\n");
		exit(-1);
	}
	for (int i = 0; i < image->height; i++)
		temp[i] = malloc(image->width * sizeof(RGB));
	//aplicam kernel ul pe toata imaginea
	apply_kernel_in_temp(image, kernel, (*rgbpixels), &temp);
	//daca nu avem selectie copiem temp in rgb
	if (ok != 1) {
		for (int i = 1; i < image->height - 1; i++) {
			for (int j = 1; j < image->width - 1; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	} else {
		//daca avem selectie copiem doar pixelii de care avem nevoie
		image_margin(image, &x1, &y1, &x2, &y2);
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	}
	
	for (int i = 0; i < image->height; i++) {
		free(temp[i]);
	}
	free(temp);
	return (*rgbpixels);
}

RGB** edge(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok) {
	double kernel[3][3] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	RGB **temp = malloc(image->height * sizeof(RGB*));
	if (temp == NULL) {
		printf("Couldn't allocate memory\n");
		exit(-1);
	}
	for (int i = 0; i < image->height; i++)
		temp[i] = malloc(image->width * sizeof(RGB));
	apply_kernel_in_temp(image, kernel, (*rgbpixels), &temp);
	if (ok != 1) {
		for (int i = 1; i < image->height - 1; i++) {
			for (int j = 1; j < image->width - 1; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	} else {
		image_margin(image, &x1, &y1, &x2, &y2);
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	}
	
	for (int i = 0; i < image->height; i++) {
		free(temp[i]);
	}
	free(temp);
	return (*rgbpixels);
}

RGB** sharpen(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok) {
	double kernel[3][3] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	RGB **temp = malloc(image->height * sizeof(RGB*));
	if (temp == NULL) {
		printf("Couldn't allocate memory\n");
		exit(-1);
	}
	for (int i = 0; i < image->height; i++)
		temp[i] = malloc(image->width * sizeof(RGB));
	apply_kernel_in_temp(image, kernel, (*rgbpixels), &temp);
	if (ok != 1) {
		for (int i = 1; i < image->height - 1; i++) {
			for (int j = 1; j < image->width - 1; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	} else {
		image_margin(image, &x1, &y1, &x2, &y2);
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	}
	
	for (int i = 0; i < image->height; i++) {
		free(temp[i]);
	}
	free(temp);
	return (*rgbpixels);
}

RGB** blur(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok) {
	double kernel[3][3] = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			kernel[i][j] /= 9.00;
	RGB **temp = malloc(image->height * sizeof(RGB*));
	if (temp == NULL) {
		printf("Couldn't allocate memory\n");
		exit(-1);
	}
	for (int i = 0; i < image->height; i++)
		temp[i] = malloc(image->width * sizeof(RGB));
	apply_kernel_in_temp(image, kernel, (*rgbpixels), &temp);
	if (ok != 1) {
		for (int i = 1; i < image->height - 1; i++) {
			for (int j = 1; j < image->width - 1; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	} else {
		image_margin(image, &x1, &y1, &x2, &y2);
		for (int i = y1; i < y2; i++) {
			for (int j = x1; j < x2; j++) {
				temp_to_rgb(&(*rgbpixels), temp, i, j);
			}
		}
	}
	
	for (int i = 0; i < image->height; i++) {
		free(temp[i]);
	}
	free(temp);
	return (*rgbpixels);
}

//functie care verifica comanda apply
int check_apply(Image* image, char buffer[]) {
	if (image == NULL) {
		printf("No image loaded\n");
		return 0;
	}
	buffer = strtok(NULL, "\n");
	if (buffer == NULL) {
		printf("Invalid command\n");
		return 0;
	}
	if (image->type == 2 || image->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	if (strcmp(buffer, "GAUSSIAN_BLUR") == 0) {
		return 1;
	}
	if (strcmp(buffer, "BLUR") == 0) {
		return 2;
	}
	if (strcmp(buffer, "SHARPEN") == 0) {
		return 3;
	}
	if (strcmp(buffer, "EDGE") == 0) {
		return 4;
	}
	return 5;
}