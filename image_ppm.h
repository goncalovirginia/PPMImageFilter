
#ifndef IMAGE_PPM_H
#define IMAGE_PPM_H

enum RGB {RED, GREEN, BLUE}; // RED==0, GREEN==1, BLUE==2
#define RGBV 3
#define NEUTRAL 4

/*  
Function to input an image in PPM ASCII format, RGB
Header: P3, witdh == dimX; height == dimY; color == maximum value for RGB; comment lines (optional)
Number of bytes created to store an image == width*height*3
*/
void input_ppm( char *name, int *width, int *height, int *color, unsigned char **input_image);


/*  
Function to output an image in PPM ASCII format, RGB
Header: P3, witdh == dimX; height == dimY; color == maximum value for RGB
Number of bytes in image buffer passed as input == width*height*3
*/
void output_ppm(char *name, int width, int height, int color, unsigned char *image);


//Prints the intensity values for each primary colour
void print_primary_intensity(  unsigned long primary[RGBV] );

// Compares colour intensities
unsigned int compare_colors( unsigned long primary[RGBV] );

// Filters (resets) non-dominant primary colours
void filter_colors( unsigned char * image_pixel, unsigned int dominant_primary );

#endif










