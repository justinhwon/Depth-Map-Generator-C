/*
 * Project 1-1: Quadtree Compression
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
	//get index of first coordinate
	int index = (y*map_width)+x;
	//get value at first coordinate
	unsigned char first_value = depth_map[index];
	int x1;
	int y1;
	//loop through all relevant coordinates
	for (x1 = x; x1 < x + section_width; x1++){
		for (y1 = y; y1 < y + section_width; y1++){
			//find index of current coordinate
			unsigned char curr_index = (y1*map_width) + x1;
			//if any value differs from first, not homogenous
			if(depth_map[curr_index] != first_value){
				return 0;
			}
		}
	}
	//otherwise is homogenous
    return 1;
}

qNode *create_quad_node(unsigned char *depth_map, int map_width, int x, int y, int section_width){
	//Create node
	qNode* node = malloc(sizeof(qNode));
	if (node == NULL) {
		allocation_failed();
	}
	//Set node size
	node->size = section_width;
	//Set up other parameters
	node->x = x;
	node->y = y;

	//If size is 1, automatically leaf
	if(node->size == 1){
		node->leaf = 1;
		int index = (y*map_width)+x;
		node->gray_value = depth_map[index];
		return node;
	}


	//If homogenous return node with right color
	if (homogenous(depth_map, map_width, x, y, section_width)){
		//get index of first coordinate
		int index = (y*map_width)+x;
		node->leaf = 1;
		node->gray_value = depth_map[index];
		return node;
	}
	//Create child nodes
	else{
		node->leaf = 0;
		node->gray_value = -1;
		node->child_NW = create_quad_node(depth_map, map_width, x, y, section_width/2);
		node->child_NE = create_quad_node(depth_map, map_width, x + section_width/2, y, section_width/2);
		node->child_SE = create_quad_node(depth_map, map_width, x + section_width/2, y + section_width/2, section_width/2);
		node->child_SW = create_quad_node(depth_map, map_width, x, y + section_width/2, section_width/2);
	}
	return node;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    /* YOUR CODE HERE */

	//Create root node
	qNode* root = malloc(sizeof(qNode));
	if (root == NULL) {
		allocation_failed();
	}
	//Set root size
	root->size = map_width;
	//Set up other parameters
	root->x = 0;
	root->y = 0;

	//If size is 0 or map is NULL, return NULL
	if(root->size <= 0 || depth_map == NULL){
		return NULL;
	}

	//If size is 1, automatically leaf
	if(root->size == 1){
		root->leaf = 1;
		root->gray_value = depth_map[0];
		return root;
	}

	//If homogenous return node with right color
	if (homogenous(depth_map, map_width, 0, 0, map_width)){
		root->leaf = 1;
		root->gray_value = depth_map[0];
		return root;
	}
	//Create child nodes
	else{
		root->leaf = 0;
		root->gray_value = -1;
		root->child_NW = create_quad_node(depth_map, map_width, 0, 0, map_width/2);
		root->child_NE = create_quad_node(depth_map, map_width, map_width/2, 0, map_width/2);
		root->child_SE = create_quad_node(depth_map, map_width, map_width/2, map_width/2, map_width/2);
		root->child_SW = create_quad_node(depth_map, map_width, 0, map_width/2, map_width/2);
	}
    
    return root;
}





void free_qtree(qNode *qtree_node) {
	//check for null
	if (qtree_node == NULL){
		return;
	}

	//If leaf delete node and return
    if(qtree_node->leaf == 1){
    	free(qtree_node);
    	return;
    }
    //If not call free_qtree on all children before deleting node
    else{
    	free_qtree(qtree_node->child_NW);
    	free_qtree(qtree_node->child_NE);
    	free_qtree(qtree_node->child_SE);
    	free_qtree(qtree_node->child_SW);
    }
    free(qtree_node);
}

/*
typedef struct quadtree qNode;
struct quadtree {
    bool leaf;
    int size;
    int x;
    int y;
    int gray_value; // 0-255 for leaves; -1 for internal nodes.
    qNode *child_NW, *child_NE, *child_SE, *child_SW;
};

*/

	/*
	//Else begin recursive call
	else{
		root->leaf = 0;
		root->gray_value = -1;

		// allocate memory for 4 new nodes
	    // check if allocation succeeds
	    qNode* node_NW = malloc(sizeof(qNode));
	    if (node_NW == NULL) {
			allocation_failed();
		}
		root->child_NW = node_NW;

	    qNode* node_NE = malloc(sizeof(qNode));
	    if (node_NE == NULL) {
			allocation_failed();
		}
		root->child_NE = node_NE;

	    qNode* node_SE = malloc(sizeof(qNode));
	    if (node_SE == NULL) {
			allocation_failed();
		}
		root->child_SE = node_SE;

	    qNode* node_SW = malloc(sizeof(qNode));
	    if (node_SW == NULL) {
			allocation_failed();
		}
		root->child_SW = node_SW;
	}
	*/