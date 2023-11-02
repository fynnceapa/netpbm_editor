#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "image_struct.h"

void apply_kernel_in_temp(Image *image, double kernel[3][3], RGB **rgbpixels, RGB ***temp);
RGB** gaussian_blur(Image *image, RGB ***rgbpixels, int x1, int y1, int x2, int y2, int ok);
RGB** edge(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok);
RGB** sharpen(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok);
RGB** blur(Image* image, RGB*** rgbpixels, int x1, int y1, int x2, int y2, int ok);
int check_apply(Image* image, char buffer[]);
int select_all(Image* image, int *x1, int *y1, int *x2, int *y2);
void swap(int *x, int *y);
double clamp(double n);
int selection(Image* image, int *x1, int *y1, int *x2, int *y2, char **buffer);
void ignore_comments(FILE* fp);
int what_type (char magic_word[]);
Image *load_img_header(char *imgname, FILE* img);
void load_img_pixels(Image* image, FILE* img, RGB ***rgbpixels, unsigned char ***graypixels);
int crop_image_gray(Image **image, unsigned char ***pixels, int x1, int y1, int x2, int y2);
int crop_image_color (Image **image, RGB ***pixels, int x1, int y1, int x2, int y2);
void equalize_image(Image **image, unsigned char ***pixels);
void print_histogram (Image* image, unsigned char **pixels, int x, int y);
int check_histogram(Image *image, char buffer[], int *x, int *y);
void save_img(Image* img, unsigned char **graypixels, RGB **rgbpixels, char *filename);
int crop_gray(Image** image, unsigned char ***graypixels, int x1, int y1, int x2, int y2);
int crop_color(Image** image, RGB*** rgbpixels, int x1, int y1, int x2, int y2);
int check_rotate(Image* image, char buffer[], int x1, int y1, int x2, int y2);
void rotate_90_gray(Image** image, int x1, int y1, int x2, int y2, unsigned char ***gray);
void rotate_90_rgb(Image** image, int x1, int y1, int x2, int y2, RGB ***rgb);
void rotate_neg_90_gray(Image** image, int x1, int y1, int x2, int y2, unsigned char ***gray);
void rotate_neg_90_rgb(Image** image, int x1, int y1, int x2, int y2, RGB ***rgb);
void apply_blur(Image *image, RGB **rgbpixels, RGB ***temp);
void my_free_pixels(int image_type, int image_height, unsigned char ***gray, RGB ***rgb);
void reset_old(Image **image, int *old_type, int *old_height);
void reset_selection(int *x1, int *y1, int *x2, int *y2);
void change_type_ascii(Image **image);
void change_type_bin(Image **image);
int check_buffer(char buffer[]);
int check_image_loaded(Image *image);