/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Global variables
int rows;
int cols;

//Return index of cell in the image given (row, col)
//row & col can exceed the boundary
int getIndex(int row, int col)
{
	return ((row + rows) % rows) * cols + (col + cols) % cols;
}

//Return arr of pointers to Color neighbors of a cell
//Arr index in neighbors: 0 1 2
//		          7   3
//	                  6 5 4
void getNeighbors(Image *image, Color **neighbors, int row, int col)
{	
	int cols = image->cols;
	int rows = image->rows;
	neighbors[0] = image->image[getIndex(row - 1, col - 1)];
	neighbors[1] = image->image[getIndex(row - 1, col)];
	neighbors[2] = image->image[getIndex(row - 1, col + 1)];
	neighbors[3] = image->image[getIndex(row, col + 1)];
	neighbors[4] = image->image[getIndex(row + 1, col + 1)];
	neighbors[5] = image->image[getIndex(row + 1, col)];
	neighbors[6] = image->image[getIndex(row + 1, col - 1)];
	neighbors[7] = image->image[getIndex(row, col - 1)];
}

int ruleLookup(uint32_t rule, int bitSum, int live)
{
	if (live)
		return rule>>(9 + bitSum) & 1;
	else
		return rule>>bitSum & 1;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color **neighbors = calloc(8, sizeof(Color *));
	Color *c = malloc(sizeof(Color));
	getNeighbors(image, neighbors,row, col);
	Color *current = image->image[getIndex(row, col)];
	
	int colorValR = 0, colorValG = 0, colorValB = 0;
	int bitSumR = 0, bitSumG = 0, bitSumB = 0;
	int power = 1;

	//loop over 8 bits of color val
	for (int i = 0; i < 8; i++)
	{
		bitSumR = bitSumG = bitSumB = 0;
		//loop over 8 neighbors of the cell
		for (int j = 0; j < 8; j++)
		{
			bitSumR += (neighbors[j]->R>>i) & 1;
			bitSumG += (neighbors[j]->G>>i) & 1;
			bitSumB += (neighbors[j]->B>>i) & 1;
		}
		
		colorValR += ruleLookup(rule, bitSumR, current->R>>i & 1) * power;
		colorValG += ruleLookup(rule, bitSumG, current->G>>i & 1) * power;
		colorValB += ruleLookup(rule, bitSumB, current->B>>i & 1) * power;
		power *= 2;
	}

	c->R = colorValR;
	c->G = colorValG;
	c->B = colorValB;

	free(neighbors);

	return c;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = malloc(sizeof(Image));
        newImage->cols = image->cols;
        newImage->rows = image->rows;
        newImage->image = calloc(image->rows * image->cols, sizeof(Color *));
	
	//initialize global variables
	cols = image->cols;
	rows = image->rows;

        for (int i = 0; i < image->rows * image->cols; i++)
        {
                newImage->image[i] = evaluateOneCell(image, i / image->cols, i % image->cols, rule);
        }
        return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
                printf("usage: %s filename\n",argv[0]);
                printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
                exit(-1);
        }
        Image *image = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 0);
        Image *newImage = life(image, rule);
        writeData(newImage);
        freeImage(image);
        freeImage(newImage);
        return 0;

}
