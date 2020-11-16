#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "image_ppm.h"
#include "image_filter_parallel.h"


int main(int argc, char *argv[]) {
	unsigned char *image;
	int nthreads, width, height, maxColorIntensity;
	
	if (argc != 4) {
		printf("Usage: %s input_file output_file nthreads\n", argv[0]);
		return 1;
	}

	// read original image
	input_ppm(argv[1], &width, &height, &maxColorIntensity, &image);

	if ((nthreads = atoi(argv[3])) <= 0 || (height % nthreads) != 0) { 
		printf("Number of threads must be positive and multiple of the image's lines\n");
		printf("nthreads:%d width:%d height:%d height %% nthreads:%d\n", nthreads, width, height, height % nthreads);
		exit(0);
	}
	
	// transforms image
	process_image_parallel(nthreads, image, width, height, filter_colors_image);
	
	// writes processed image
	output_ppm(argv[2], width, height, maxColorIntensity, image);

	return 0;
}








