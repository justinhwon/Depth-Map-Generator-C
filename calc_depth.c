/*
 * Project 1-1: Computing a Displacement Map
 *
 * Feel free to define additional helper functions.
 */

#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {
    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement)
            / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;
}

/* Helper function to return the square euclidean distance between two values. */
unsigned int square_euclidean_distance(unsigned char a, unsigned char b) {
   /*
    * This is an optional helper function which you may find useful. It
    * currently has an incomplete example CUnit test to help with debugging.
    * This test is not graded and is merely for your convenience. See the spec
    * for details on how to use CUnit.
    */
    unsigned int ai = a;
    unsigned int bi = b;
    unsigned int diff = ai - bi;
    unsigned int squared = diff * diff;
    return squared;
}

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {

    //depth map from indices 0 to image_width*image_height
    //col x, row y position is depth_map[y*image_width + x]

    //If maximum_displacement == 0
    if (maximum_displacement == 0){
        int x;
        unsigned char* ptr = depth_map;
        for (x = 0; x < image_width*image_height; x++){
            *ptr = 0;
            ptr++;
        }
    }


    //Output to depth_map for every pixel in left image

    //Copy of ptr to depth_map
    unsigned char* depth = depth_map;
    //Starting from index 0 (0, 0)
    int index = 0;
    int x = 0;
    int y = 0;

    while(index < image_width*image_height){

        //Get x and y coordinate of index (both start at 0)
        x = index % image_width;
        y = index / image_width;

        //Make sure patch is within left image bound
        if (x - feature_width < 0 || x + feature_width >= image_width){
            depth[index] = 0;
        }
        else if (y - feature_height < 0 || y + feature_height >= image_height){
            depth[index] = 0;
        }
        else{
            //actual check for patches
            //patch_size = (sizeof(unsigned char) * feature_width * feature_height) - sizeof(unsigned char);
            //unsigned char* left_patch = malloc(patch_size);


            //Your algorithm should not consider right-image features that lie partially outside the image area.
            //Loop through patches within max displacement (based on centers)
            //int similar_x = -1;
            //int similar_y = -1;
            int smallest_euclid = -1;
            unsigned char norm_disp = 255;
            int xr;
            int yr;
            //Loop through possible patch centers
            for (xr = x - maximum_displacement; xr <= x + maximum_displacement; xr++){
                //Skip patches that would go out of bounds
                if (xr - feature_width < 0 || xr + feature_width >= image_width){
                    continue;
                }
                for (yr = y - maximum_displacement; yr <= y + maximum_displacement; yr++){
                    //Skip patches that would go out of bounds
                    if (yr - feature_height < 0 || yr + feature_height >= image_height){
                        continue;
                    }

                    //Check for euclidean distance of patch
                    //Loop through a patch centered at xr, yr
                    int x1;
                    int y1;
                    int euclid = 0;
                    //Loop over a patch in right
                    for (x1 = xr - feature_width; x1 <= xr + feature_width; x1++){
                        for (y1 = yr - feature_height; y1 <= yr + feature_height; y1++){
                            // Add euclidean distance between points
                            int yL = y - (yr - y1);
                            int xL = x - (xr - x1);
                            euclid += square_euclidean_distance(left[(yL*image_width)+xL], right[(y1*image_width)+x1]);
                        }
                    }
                    //If smallest euclidean distance, new similar patch
                    if (euclid < smallest_euclid || smallest_euclid == -1){
                        smallest_euclid = euclid;
                        norm_disp = normalized_displacement(x-xr, y-yr, maximum_displacement);
                        //similar_x = xr;
                        //similar_y = yr;
                    }
                    //check for smaller normalized displacement if tie
                    else if (euclid == smallest_euclid){
                        unsigned char new_disp = normalized_displacement(x-xr, y-yr, maximum_displacement);
                        if (new_disp < norm_disp){
                            smallest_euclid = euclid;
                            norm_disp = new_disp;
                            //similar_x = xr;
                            //similar_y = yr;
                        }
                    }
                }
            }
            //set depth to the normalized displacement
            depth_map[index] = norm_disp;

            /*
            //Loop through a patch centered at xr, yr
            int x1;
            int y1;
            int euclid = 0;
            //Loop over a patch in right
            for (x1 = xr - feature_width; x1 <= xr + feature_width; x1++){
                for (y1 = yr - feature_height; y1 <= yr + feature_height; y1++){
                    // Add euclidean distance between points
                    euclid += square_euclidean_distance(left[(x1*image_width)+y1], right[(x1*image_width)+y1]);
                }
            }
            //If smallest euclidean distance, new similar patch
            if (euclid < smallest_euclid || smallest_euclid == -1){
                smallest_euclid = euclid;
                norm_disp = normalized_displacement(x-xr, y-yr, maximum_displacement);
                similar_x = xr;
                similar_y = yr;
            }
            //check for smaller normalized displacement if tie
            else if (euclid == smallest_euclidean){
                unsigned char new_disp = normalized_displacement(dx, dy, maximum_displacement);
                if (new_disp < norm_disp){
                    smallest_euclid = euclid;
                    norm_disp = new_disp;
                    similar_x = xr;
                    similar_y = yr;
                }
            }
            */

        }
        
        //Loop through entire image
        index++;
    }
    


    // if not in array bounds, displacement = 0
}
