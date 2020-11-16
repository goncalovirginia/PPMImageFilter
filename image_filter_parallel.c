#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#include "image_ppm.h"

#define TRUE 1
#define FALSE 0

unsigned long primary_colors[RGBV];	// vector to store the sum of intensities for all pixels
unsigned int dominant_primary_color; // dominant/"winner" colour

unsigned char *image;
int width, height, color, lines_per_thread;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;	
pthread_barrier_t fillBarrier;

/*
Function that identifies the dominant primary colour, e.g. read, green or blue that
has the biggest contribution to the image and transforms the image to have only
that primary. The calculation of the dominant primary is done by summming all primaries'
contributions to all pixels and finding the biggest value.
*/
void *filter_colors_image(void *id) {
	unsigned long start = (unsigned long) id * width * lines_per_thread * RGBV; // First pixel to process.
	unsigned long end = start + (width * lines_per_thread * RGBV) - 1;  // Last pixel to process.
#ifdef VERBOSE
	printf("id = %lu, start = %lu, end= %lu\n", (unsigned long)id, start, end);	
#endif

	unsigned long local_primary_colors[RGBV] = {0, 0, 0};

	// Each thread processes its sub-vector
	for (int i = start; i <= end; i++) {
		local_primary_colors[i%RGBV] += image[i];
	}

	//All threads communicate to calculate the three sums for all pixels in the image
	pthread_mutex_lock(&mtx);
	for (int i = RED; i <= BLUE; i++) {
		primary_colors[i] += local_primary_colors[i];
	}
	pthread_mutex_unlock(&mtx);

	// All threads coordinate themselves to identify the dominant primary colour
	pthread_barrier_wait(&fillBarrier);
	
	if ((unsigned long) id == 0) {
		#ifdef VERBOSE
		print_primary_intensity(primary_colors);
		#endif
		dominant_primary_color = compare_colors(primary_colors);
		#ifdef VERBOSE
		printf("dominant_primary_color:%d\n", dominant_primary_color);
		#endif
	}
	
	pthread_barrier_wait(&fillBarrier);

	// All threads filter the non-dominant primary colours in their sub-vectors
	for(int i = start; i <= end; i+=3) {
		filter_colors(image + i, dominant_primary_color);
	}
	
	return NULL;
}


/* Parallel transformation of an image with size num_cols*num_lines pixels. 
Each thread has to execute the filter() function
*/
void process_image_parallel(int nthreads, unsigned char *input_image, unsigned long num_cols, unsigned long num_lines, void *(*filter)(void *)) {
	pthread_t *ids = malloc(sizeof(pthread_t) * nthreads); // Pointer to the vector with the threads' identifiers.

	image = input_image; // The image to be processed
	width = num_cols; // Number of pixels in the yy dimension
	height = num_lines; // Number of pixels in the xx dimension

	lines_per_thread = height / nthreads; // Number of full lines each thread processes 

	// Barrier init
	pthread_barrier_init(&fillBarrier, NULL, nthreads);
	
	// Thread/worker creation
	for (int i = 0; i < nthreads; i++) {
		ids[i] = (unsigned long) i;
		pthread_create(ids + i, NULL, filter_colors_image, (void*)ids[i]);
	}

	// Waits for all threads to conclude
	for (int i = 0; i < nthreads; i++) {
		pthread_join(ids[i], NULL);
	}

	free(ids);
}
