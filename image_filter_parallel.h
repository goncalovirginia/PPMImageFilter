

#ifndef IMAGE_FILTER_PARALLEL_H
#define IMAGE_FILTER_PARALLEL_H

/*
Function that identifies the dominant primary colour, e.g. read, green or blue that
has the biggest contribution to the image and transforms the image to have only
that primary. The calculation of the dominant primary is done by summming all primaries'
contributions to all pixels and finding the biggest value.
*/
void *filter_colors_image(void *id);



/* 
Parallel transformation of an image withe size nul_cols*num_lines pixels. 
Each thread has to execute the filter() function
*/
void process_image_parallel(int nthreads, unsigned char *input_image, 
			unsigned long num_cols, unsigned long num_lines, void *(*filter)(void *));


#endif






