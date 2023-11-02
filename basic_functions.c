#include "functii.h"

//functie care schimbca doua variabile
void swap(int *x, int *y) {
	int aux;
	aux = (*y);
	(*y) = (*x);
	(*x) = aux;
}

void restore_old_selection(int *x1, int *y1, int *x2, int *y2,
							int c1, int c2, int c3, int c4) {

	(*x1) = c1;
	(*y1) = c2;
	(*x2) = c3;
	(*y2) = c4;
}

//functie pt SELECT
int selection(Image *image, int *x1, int *y1, int *x2, int *y2, char **buffer) {
			//daca nu avem imagine => eroare
			if (check_image_loaded(image) == 0)
				return 0;
			//se face copie pt selectia anterioara in caz de eroare
			int c1 = (*x1), c2 = (*y1), c3 = (*x2), c4 = (*y2);

			(*buffer) = strtok(NULL, "\n");
			//daca avem comanda SELECT ALL devin toate 0
			if (strcmp((*buffer), "ALL") == 0) {
				reset_selection(&(*x1), &(*y1), &(*x2), &(*y2));
				printf("Selected ALL\n");
				return 2;
			}
			//daca avem o comanda invalida => eroare
			if (check_buffer((*buffer)) == 0)
				return 0;

			//daca nu avem 4 numere pt selectie => eroare
			if (sscanf((*buffer), "%d %d %d %d", &(*x1), &(*y1), &(*x2), &(*y2)) != 4) {
				printf("Invalid command\n");
				restore_old_selection(&(*x1), &(*y1), &(*x2), &(*y2), c1, c2, c3, c4);
				return 0;
			}
			
			//daca selectia nu este valida => eroare
			if ((*x1) > image->width || (*x2) > image->width || (*x1) == (*x2) || (*x1) < 0 || (*x2) < 0) {
				printf("Invalid set of coordinates\n");
				restore_old_selection(&(*x1), &(*y1), &(*x2), &(*y2), c1, c2, c3, c4);
				return 0;
			}
			if ((*y1) > image->height || (*y2) > image->height || (*y1) == (*y2) || (*y1) < 0 || (*y2) < 0) {
				printf("Invalid set of coordinates\n");
				restore_old_selection(&(*x1), &(*y1), &(*x2), &(*y2), c1, c2, c3, c4);
				return 0;
			}

			//daca x1 > x2 le schimbam, idem pt y
			if ((*x1) > (*x2))
				swap (&(*x1), &(*x2));
			if ((*y1) > (*y2))
				swap (&(*y1), &(*y2));
			//se afiseaza selectia daca nu sunt erori
			printf("Selected %d %d %d %d\n", (*x1), (*y1), (*x2), (*y2));
			return 1;
		}

//functie pentru salvarea imaginii
void save_img(Image* img, unsigned char **graypixels, RGB **rgbpixels, char *filename) {
	
	//se deschide fisierul
	FILE* fp = fopen(filename, "wb");

	//daca nu a putut fi deschid =>eroare
	if (fp == NULL) {
		printf("Couldn't open the file\n");
		exit(-1);
	}

	//daca nu avem imagine => eroare
	if (check_image_loaded(img) == 0)
		return;

	//verificam tipul imaginii
	if (img->type == 2)
		fprintf(fp, "P%d\n", 2);
	if (img->type == 5)
		fprintf(fp, "P%d\n", 5);
	if (img->type == 3)
		fprintf(fp, "P%d\n", 3);
	if (img->type == 6)
		fprintf(fp, "P%d\n", 6);

	//scriem headerul in fisier
	fprintf(fp, "%d %d\n%d\n", img->width, img->height, img->max_val);

	//in functie de tipul imaginii facem scrierea
	//valorilor pixelilor

	if (img->type == 2) {
		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				fprintf(fp, "%d ", graypixels[i][j]);
			}
			fprintf(fp, "\n");
		}
	}
	if (img->type == 3) {
		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				for (int k = 0; k < 3; k++) {
					if (k == 0)
						fprintf(fp, "%d ", rgbpixels[i][j].r);
					if (k == 1)
						fprintf(fp, "%d ", rgbpixels[i][j].g);
					if (k == 2)
						fprintf(fp, "%d ", rgbpixels[i][j].b);
				}
			}
			fprintf(fp, "\n");
		}
	}
	//daca avem tip binar se foloseste fwrite
	if (img->type == 5) {
		for (int i = 0; i < img->height; i++) {
			fwrite(graypixels[i], sizeof(unsigned char), img->width, fp);
		} 
	}
	if (img->type == 6) {
		for (int i = 0; i < img->height; i++) {
			fwrite(rgbpixels[i], sizeof(RGB), img->width, fp);
		}
	}
	fclose(fp);
	printf("Saved %s\n", filename);
}

//functia clamp
double clamp(double n) {
	if (n > 255)
		n = 255;
	if (n < 0)
		n = 0;
	return n;
}

//functie care elibereaza memoria alocata pixelilor in functie de tip
void my_free_pixels(int image_type, int image_height, unsigned char ***gray, RGB ***rgb) {
	if (image_type == 3 || image_type == 6) {
		for (int i = 0; i < image_height; i++)
			free((*rgb)[i]);
		free((*rgb));
	}
	if (image_type == 2 || image_type == 5) {
		for (int i = 0; i < image_height; i++)
			free((*gray)[i]);
		free((*gray));
	}
}

void reset_old(Image **image, int *old_type, int *old_height) {
	(*image) = NULL;
	(*old_type) = 0;
	(*old_height) = 0;
}

void reset_selection(int *x1, int *y1, int *x2, int *y2) {
	(*x1) = 0;
	(*x2) = 0;
	(*y1) = 0;
	(*y2) = 0;
}

void change_type_ascii(Image **image) {
	if ((*image)->type == 6)
		(*image)->type = 3;
	if ((*image)->type == 5)
		(*image)->type = 2;
}

void change_type_bin(Image **image) {
	if ((*image)->type == 3)
		(*image)->type = 6;
	if ((*image)->type == 2)
		(*image)->type = 5;
}

int check_buffer(char buffer[]) {
	if (buffer == NULL) {
		printf("Invalid command\n");
		return 0;
	}
	return 1;
}

int check_image_loaded(Image *image) {
	if (image == NULL) {
		printf("No image loaded\n");
		return 0;
	}
	return 1;
}