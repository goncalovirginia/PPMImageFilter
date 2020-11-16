
#include <stdio.h>
#include <stdlib.h>

#include "image_ppm.h"

#define MAXC 100

/*  
Function to input an image in PPM ASCII format, RGB
Header: P3, witdh == dimX; height == dimY; color == maximum value for RGB; comment lines (optional)
Number of bytes created to store an image == width*height*3
*/
void input_ppm(char *name, int *width, int *height, int *color, unsigned char **input_image) {
	FILE *f;
	char buffer[MAXC];

	if ((f = fopen(name, "r")) == NULL) {
		perror("Error opening file:");
		exit(1);
	}

	// read header
	int linesize, lines;
	
	// while( fgets( buffer, MAXC, f ) != NULL && buffer[0] == '#' );
	if (feof(f) || fscanf(f, "P3\n") == EOF) {
		fprintf( stderr, "Wrong file format\n" );
		exit(1);
	}

	while(fgets(buffer, MAXC, f) != NULL && buffer[0] == '#');

	if(feof(f) || sscanf( buffer, "%d %d\n", &linesize, &lines ) == EOF) {
		fprintf(stderr, "Wrong file format\n");
		exit(1);
	}

	while(fgets(buffer, MAXC, f) != NULL && buffer[0] == '#');

	if(feof(f) || sscanf(buffer, "%d\n", color) == EOF) {
		fprintf(stderr, "Wrong file format\n");
		exit(1);
	}
	
	#ifdef VERBOSE
	printf("color:%d linesize:%d lines:%d ", *color, linesize, lines);
	printf("linesize*lines*RGBV:%lu\n", (linesize*lines*RGBV*sizeof(unsigned char)));
	#endif
	// create image buffer
	unsigned char *image = calloc(linesize*lines*RGBV, sizeof(unsigned char));
	

	// read image lines 
	int xx, yy;
	for( xx = 0; xx < lines; xx++ ) {
		for( yy = 0; yy < linesize; yy++ ) {
			unsigned int r, g, b;
			fscanf(f, "%u\n%u\n%u\n", &r, &g, &b);
			image[(xx*linesize+yy)*RGBV] = (unsigned char)r;
			image[(xx*linesize+yy)*RGBV+1] = (unsigned char)g; 	
			image[(xx*linesize+yy)*RGBV+2] = (unsigned char)b;
			/* fscanf(f, "%u\n%u\n%u\n", &image[(xx*linesize+yy)*RGBV], 
						&image[(xx*linesize+yy)*RGBV+1], &image[(xx*linesize+yy)*RGBV+2]);
			*/
		}
	}	

	*width = linesize;
	*height = lines;
	*input_image = image;

	fclose( f );
}


/*  
Function to output an image in PPM ASCII format, RGB
Header: P3, witdh == dimX; height == dimY; color == maximum value for RGB
Number of bytes in image buffer passed as input == width*height*3
*/
void output_ppm(char *name, int width, int height, int color, unsigned char *image) {
	FILE *f;

	if((f = fopen( name, "w")) == NULL){
		//fprintf( stderr, "Error opening file %s\n", name);
		perror("Error opening file: ");
		exit(1);
	}

	// write header
	if(fprintf(f, "P3\n%d %d\n%d\n", width, height, color) == EOF) {
		fprintf( stderr, "problem writing file format\n" );
		exit(1);
	}

	// write pixels' colour intensities
	int xx, yy;
	for (xx = 0; xx < height; xx++) {
		for (yy = 0; yy < width; yy++) {
			if (fprintf(f, "%u\n%u\n%u\n", image[(xx*width+yy)*RGBV], image[(xx*width+yy)*RGBV+1], image[(xx*width+yy)*RGBV+2]) < 0) {
				fprintf( stderr, "Error writing file\n" );
				exit(1);
			}
		}
	}	

	fclose( f );
}

//Prints the intensity values for each primary colour
void print_primary_intensity(unsigned long primary[RGBV]) {
	for(int i=RED; i <= BLUE; i++) {
		printf("primary[%i]==%lu ", i, primary[i]);

	}
	printf("\n"); fflush(stdout);
}


// Compares colour intensities
unsigned int compare_colors(unsigned long primary[RGBV]) {
	if(primary[RED] > primary[GREEN] && primary[RED] > primary[BLUE]) {
		return RED;
	}
	else if(primary[GREEN] > primary[RED] && primary[GREEN] > primary[BLUE]) {
		return GREEN;
	}
	else if(primary[BLUE] > primary[RED] && primary[BLUE] > primary[GREEN]) {
		return BLUE;
	}
	return NEUTRAL;
} 

// Filters (resets) non-dominant primary colours
void filter_colors(unsigned char *image_pixel, unsigned int dominant_primary) {
	if(dominant_primary == RED) {
		image_pixel[1] = 0;
		image_pixel[2] = 0;
	}
	else if (dominant_primary == GREEN) {
		image_pixel[0] = 0;
		image_pixel[2] = 0;
	}
	else {
		image_pixel[0] = 0;
		image_pixel[1] = 0;
	}

}
