#include "functii.h"
int main(void)
{
	char command[1000];
	Image *image = NULL;
	unsigned char **graypixels;
	RGB **rgbpixels;
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0, sel;
	int cmdok = 0, old_height = 0, old_type = 0;
	while (1) {
		scanf("%[^\n]%*c", command);
		cmdok = 0;
		//se verifica ce comanda a fost introdusa
		//daca aceasta este valida cmdok devine 1
		char *buffer = strtok(command, " ");
		//exit
		if (strcmp(buffer, "EXIT") == 0) {
			cmdok = 1;
			//daca nu avem imagine se intrerupe programul
			if (check_image_loaded(image) == 0)
				break;
			//daca avem imagine incarcata se dealoca memoria pentru aceasta
			//si se opreste programul
			my_free_pixels(image->type, image->height, &graypixels, &rgbpixels);
			free(image);
			break;
		}
		if (strcmp(buffer, "LOAD") == 0) {
			cmdok = 1;
			buffer = strtok(NULL, " ");
			if(check_buffer(buffer)) {
				//daca comanda este valida se incepe cautarea pentru fisier
				char imgname[1001];
				sscanf(buffer, "%s", imgname);
				FILE *img = fopen(imgname, "rb");
				if (img == NULL) {
					//daca nu a fost gasit sau nu a putut fi deschis
					//se revine la stadiul in care nu avem nicio imagine in memorie
					printf("Failed to load %s\n", imgname);
					//free la toata memoria
					if (image != NULL)
						my_free_pixels(old_type, old_height, &graypixels, &rgbpixels);
					reset_old(&image, &old_type, &old_height);
				} else {
					//daca s-a putut deschide imaginea eliberam memoria celei vechi
					free(image);
					//incarcam header-ul celei vechi
					image = load_img_header(imgname, img);
					if (check_image_loaded(image)) {
						//daca s-a putut incarca imaginea eliberam pixelii celei vechi
						my_free_pixels(old_type, old_height, &graypixels, &rgbpixels);
						//resetam selectia pentru a fi selectata toata
						reset_selection(&x1, &y1, &x2, &y2);
						//incarcam pixelii imaginii
						load_img_pixels(image, img, &rgbpixels, &graypixels);
						//memoram marimea si tipul imaginii pentru viitoarele
						//eliberari de memorie
						old_height = image->height;
						old_type = image->type;
					}
					fclose(img);
				}
			}
		}
		if (strcmp(buffer, "SELECT") == 0) {
			cmdok = 1;
			sel = selection(image, &x1, &y1, &x2, &y2, &buffer);
		}
		if (strcmp(buffer, "CROP") == 0) {
			cmdok = 1;
			if (check_image_loaded(image)) {
				//daca avem o imagine incarcata se aplica CROP
				//in functie de tipul imaginii
				if (image->type == 2 || image->type == 5) {
					//grayscale
					if (crop_gray(&image, &graypixels, x1, y1, x2, y2) == 1)
						reset_selection(&x1, &y1, &x2, &y2);
				}
				if (image->type == 3 || image->type == 6) {
					//rgb
					if (crop_color(&image, &rgbpixels, x1, y1, x2, y2) == 1)
						reset_selection(&x1, &y1, &x2, &y2);
				}
				//in ambele cazuri, daca operatia s-a efectuat cu succes
				//se selecteaza toata selectia
				old_height = image->height;
				sel = 0;
			}
		}
		if (strcmp(buffer, "HISTOGRAM") == 0) {
			cmdok = 1;
			int x, y;
			//daca comanda este valida se afiseaza histograma
			if (check_histogram(image, buffer, &x, &y) == 1)
				print_histogram(image, graypixels, x, y);
		}
		if (strcmp(buffer, "EQUALIZE") == 0) {
			cmdok = 1;
			equalize_image(&image, &graypixels);
		}
		if (strcmp(buffer, "SAVE") == 0) {
			cmdok = 1;
			char savename[101];
			int t;
			buffer = strtok(NULL, "\n");
			//se citeste toata comanda
			if (check_buffer(buffer)) {
				//daca este valida se pune in savename numele
				//imaginii ce are sa fie salvata
				sscanf(buffer, "%s", savename);
				if (check_image_loaded(image)) {
					//daca avem imagine incarcata se continua procesul
					//in variabila t retinem tipul imaginii
					//pt. ca functiile change_type schimba tipul imaginii in 
					//functie de tipul de salvare
					t = image->type;
					//daca in comanda este prezent subsirul ascii se salveaza ascii
					if (strstr(buffer, " ascii") != NULL)
						change_type_ascii(&image);
					else
						change_type_bin(&image);
					save_img(image, graypixels, rgbpixels, savename);
					image->type = t;
				}
			}
		}
		if (strcmp(buffer, "APPLY") == 0) {
			cmdok = 1;
			int a = check_apply(image, buffer);
			if (a == 1) {
				printf("APPLY GAUSSIAN_BLUR done\n");
				rgbpixels = gaussian_blur(image, &rgbpixels, x1, y1, x2, y2, sel);
			}
			if (a == 2) {
				printf("APPLY BLUR done\n");
				rgbpixels = blur(image, &rgbpixels, x1, y1, x2, y2, sel);
			}
			if (a == 3) {
				printf("APPLY SHARPEN done\n");
				rgbpixels = sharpen(image, &rgbpixels, x1, y1, x2, y2, sel);
			}
			if (a == 4) {
				printf("APPLY EDGE done\n");
				rgbpixels = edge(image, &rgbpixels, x1, y1, x2, y2, sel);
			}
			if (a == 5) {
				printf("APPLY parameter invalid\n");
			}
		}
		if (strcmp(buffer, "ROTATE") == 0) {
			cmdok = 1;
			int check = check_rotate(image, buffer, x1, y1, x2, y2);
			int angle = 1;
			if (check != 1) {
				angle = check;
			}
			if (angle == 90) {
				if (image->type == 2 || image->type == 5) {
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
				}
				if (image->type == 3 || image->type == 6) {
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
				}
				printf("Rotated %d\n", angle);
			}
			if (angle == -90) {
				if (image->type == 2 || image->type == 5) {
					rotate_neg_90_gray(&image, x1, y1, x2, y2, &graypixels);
				}
				if (image->type == 3 || image->type == 6) {
					rotate_neg_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
				}
				printf("Rotated %d\n", angle);
			}
			if (angle == 180) {
				if (image->type == 2 || image->type == 5) {
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
				}
				if (image->type == 3 || image->type == 6) {
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
				}
				printf("Rotated %d\n", angle);
			}
			if (angle == -180)
				printf("Rotated %d\n", angle);
			if (angle == 360 || angle == -360)
				printf("Rotated %d\n", angle);
			if (angle == 0)
				printf("Rotated %d\n", angle);
			if (angle == 270) {
				if (image->type == 2 || image->type == 5) {
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
					rotate_90_gray(&image, x1, y1, x2, y2, &graypixels);
				}
				if (image->type == 3 || image->type == 6) {
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
					rotate_90_rgb(&image, x1, y1, x2, y2, &rgbpixels);
				}
				printf("Rotated %d\n", angle);
			}
			if (angle == -270)
				printf("Rotated %d\n", angle);
		}
		if (cmdok == 0)
			printf("Invalid command\n");
	}
	return 0;
}
