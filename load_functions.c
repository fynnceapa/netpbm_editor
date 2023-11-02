#include "functii.h"

//functie care ignora comentariile in timpul citirii
void ignore_comments(FILE* fp) {
	int ch;
	char line[100];
	
	//se citeste tot pana la un spatiu sau final de linie
	while ((ch = fgetc(fp)) != EOF && isspace(ch));

	//daca primul caracter este # (indicator de comentariu)
	//se citeste toata linia si se reaplica recursiv functia
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		ignore_comments(fp);
	}
	else
		fseek(fp, -1, SEEK_CUR);
	//daca nu este #, se muta cursorul la linia de dinainte
}

//functie care determina tipul imaginii
int what_type (char magic_word[]) {
	int type;
	if (magic_word[1] == '2') {
		type = 2;
	}
	if (magic_word[1] == '3') {
		type = 3;
	}
	if (magic_word[1] == '5') {
		type = 5;
	}
	if (magic_word[1] == '6') {
		type = 6;
	}
	return type;
}

//functie care citeste header-ul imaginii
Image *load_img_header(char *imgname, FILE* img) {
	char magic_word[3];
	ignore_comments(img);
	fgets(magic_word, 3, img);
	int type = what_type(magic_word);

	int width, height, max_val;

	ignore_comments(img);
	fscanf(img, "%d %d", &width, &height);

	ignore_comments(img);
	fscanf(img, "%d", &max_val);

	ignore_comments(img);

	//aloc memorie pentru structura imaginii
	Image *image = malloc(sizeof(Image));
	if (image == NULL) {
		perror("Couldn't allocate memory\n");
		exit(0);
	}
	//pun datele in structura
	image->type = type;
	image->width = width;
	image->height = height;
	image->max_val = max_val;
	printf("Loaded %s\n", imgname);
	return image;
}

//functie care citeste valorile pixelilor imaginii in functie de tipul ei
void load_img_pixels(Image* image, FILE* img, RGB ***rgbpixels, unsigned char ***graypixels) {
	if (image->type == 2) {
		(*graypixels) = malloc(image->height * sizeof(unsigned char *));
		if ((*graypixels) == NULL) {
			printf("Couldn't allocate memory\n");
			exit(0);
		}
		for (int i = 0; i < image->height; i++) {
			(*graypixels)[i] = malloc(image->width * sizeof(unsigned char));
			if ((*graypixels)[i] == NULL) {
				printf("Couldn't allocate memory\n");
				exit(0);
			}
		}
		for (int i = 0; i < image->height; i++) {
			for(int j = 0; j < image->width; j++) {
				fscanf(img, "%hhu", &(*graypixels)[i][j]);
			}
		}
	}

	if (image->type == 5) {
		(*graypixels) = malloc(image->height * sizeof(unsigned char *));
		if ((*graypixels) == NULL) {
			printf("Couldn't allocate memory\n");
			exit(0);
		}
		for (int i = 0; i < image->height; i++) {
			(*graypixels)[i] = malloc(image->width * sizeof(unsigned char));
			if ((*graypixels)[i] == NULL) {
				printf("Couldn't allocate memory\n");
				exit(0);
			}
			fread((*graypixels)[i], sizeof(unsigned char), image->width, img);
		}
	}

	if (image->type == 3) {
		(*rgbpixels) = malloc(image->height * sizeof(RGB*));
		if ((*rgbpixels) == NULL) {
			printf("Couldn't allocate memory\n");
			exit(0);
		}
		for (int i = 0; i < image->height; i++) {
			(*rgbpixels)[i] = malloc(image->width * sizeof(RGB));
		}
		if ((*rgbpixels) == NULL) {
			printf("Couldn't allocare memory\n");
			exit(0);
		}
		for (int i = 0; i < image->height; i++) {
			for (int j = 0; j < image->width; j++) {
				for (int k = 0; k < 3; k++) {
					if (k == 0)
						fscanf(img, "%hhu", &(*rgbpixels)[i][j].r);
					if (k == 1)
						fscanf(img, "%hhu", &(*rgbpixels)[i][j].g);
					if (k == 2)
						fscanf(img, "%hhu", &(*rgbpixels)[i][j].b);
				}
			}
		}
	}
	if (image->type == 6) {
		RGB **rgbimage = *rgbpixels;
		rgbimage = malloc(image->height * sizeof(RGB*));
		if (rgbimage == NULL) {
			printf("Couldn't allocate memory\n");
			exit(0);
		}
		for (int i = 0; i < image->height; i++) {
			rgbimage[i] = malloc(image->width * sizeof(RGB));
			if (rgbimage == NULL) {
				printf("Couldn't allocare memory\n");
				exit(0);
			}
			fread(rgbimage[i], sizeof(RGB), image->width, img);
		}
	*rgbpixels = rgbimage;
	}
}
