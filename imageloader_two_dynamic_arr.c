/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//This is the implementation of "Dynamic Pointer Array of Single-Dynamic Allocated Structs" under:
	//https://stackoverflow.com/questions/15397728/c-pointer-to-array-of-pointers-to-structures-allocation-deallocation-issues
	//It is the more efficient implementation compared to "Dynamic Array of Dynamically Allocated Structs"
	//However, we need the fexiblity of "Dynamic Array of Dynamically Allocated Structs"
	//So this implemetation is deprecated.
	FILE *fp = fopen(filename, "r");

	char buf[20];
	int num1, num2, num3;

	//Read general description of ppm file
	fscanf(fp, "%s %d %d %d", buf, &num1, &num2, &num3);

	//Define the Image
	Image *image = malloc(sizeof(Image));
	image->rows = num2;
	image->cols = num1;
	image->image = calloc(num1 * num2, sizeof(Color *));
	Color *arr = calloc(num1 * num2, sizeof(Color));

	for (int i = 0; i < image->rows * image->cols && fscanf(fp, "%d %d %d", &num1, &num2, &num3) != EOF; i++)
	{
		arr[i].R = num1;
		arr[i].G = num2;
		arr[i].B = num3;
		(image->image)[i] = arr + i;
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("%d\n", 255);
	
	for (int i = 0; i < image->cols * image->rows; i++)
	{
		printf("%3d ", image->image[i]->R);
		printf("%3d ", image->image[i]->G);
		printf("%3d", image->image[i]->B);
		if (i % image->cols != image->cols - 1)
			printf("   ");
		else
			printf("\n");
	}
	
}

//Frees an image
void freeImage(Image *image)
{
	free((image->image)[0]);
	free(image->image);
	free(image);
}
