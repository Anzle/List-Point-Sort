/*
 *	Sangini Shah and Rob Williams
 *	CS214 Systems Programming
 *	Programming Assignment 2- Sorted List
 *	
 *	main.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sorted-list.h"


/*
 *	Vector struct to use in Sorted Linked List
 *	
 */
struct Vector {
	int x;
	int y;
};
typedef struct Vector Vector;

/*
 *	Comparator for Vector struct. Uses magnitude as basis of comparison.
 * 	Returns
 *		0 	if the x and y values or the magnitude (sqrt(x^2 + y^2)) are the same
 *		-1	if the magnitude of v1 is less than the magnitude of v2
 *		1	if the magnitude of v1 is greater than the magnitude of v2
 */
int vector_compare(void * v1, void * v2){
	Vector * vector1 = (Vector *) v1;
	Vector * vector2 = (Vector *) v2;
	
	if((vector1 -> x == vector2 -> x && vector1 -> y == vector2 -> y) ||(sqrt(pow(vector1->x, 2) + pow(vector1->y, 2)) == sqrt(pow(vector2->x, 2) + pow(vector2->y, 2)))){
		return 0;
	}
	else if(sqrt(pow(vector1->x, 2) + sqrt(pow(vector1->y, 2))) < sqrt(pow(vector2->x, 2) + sqrt(pow(vector2->y, 2)))){
		return -1;
	}
	else {
		return 1;
	}
}

/*
 *	Destruct function for Vector struct
 *	This method frees the memory allocated for the vector struct
 */
void vector_destroy(void * v){
	free(v);
}

/*
 * Comparator function for int pointers. 
 */
int int_compare(void * i1, void * i2){
	int * num1 = (int *) i1;
	int * num2 = (int *) i2;
	
	if(*num1 == *num2){
		return 0;
	}
	else if(*num1 < *num2){
		return  -1;
	}
	else{
		return 1;
	}
}

/* 
 * Destroy function for int pointer
 * Does not do anything since the pointer will not be malloc-ed
 */
void int_destroy(void * i){
	return;
}

int main(int argc, char** argv){

	//Instantiate vectors
	Vector * v1 = (Vector *) malloc(sizeof(Vector *));
	Vector * v2 = (Vector *) malloc(sizeof(Vector *));
	Vector * v3 = (Vector *) malloc(sizeof(Vector *));
	Vector * v4 = (Vector *) malloc(sizeof(Vector *));
	Vector * v5 = (Vector *) malloc(sizeof(Vector *));

	v1->x = 1; v1->y=3; //Fourth in list
	v2->x = 0; v2->y=3; //Fifth (smallest magnitude)
	v3->x = 3; v3->y=3; //Second
	v4->x = 2; v4->y=3; //Third
	v5->x = -6; v5->y=3; //First (highest magnitude)
	
	//Array to hold all the vectors
	Vector *vectors[] = {v1, v2, v3, v4, v5};

	//Define function pointers
	int (*v_compare_ptr)(void *, void *) = &vector_compare;
	void (*v_destroy_ptr)(void *) = &vector_destroy;
	
	//Create Sorted List for Vectors
	//Vectors will be sorted into the list based on their magnitude
	//The ordering of the list should be as commented above
	SortedListPtr vector_sl = SLCreate(v_compare_ptr, v_destroy_ptr);
	
	//Insert vectors into the list
	int x;
	for(x = 0; x < 5; x++) {
		if(SLInsert(vector_sl, vectors[x]) == 0){
			printf("Error inserting Vector v%d\n", x);
		}
	}
	
	//Create iterator and print out list values
	printf("\nThe original list:\n");
	SortedListIteratorPtr vector_iter1 = SLCreateIterator(vector_sl);
	while(SLNextItem(vector_iter1) != NULL){
		Vector * v = (Vector *) SLGetItem(vector_iter1);
		if (v)
			printf("x=%d y=%d\n", v->x, v->y);
	}
	
	
	SLDestroyIterator(vector_iter1);
	
	//Create second iterator to iterate one at a time and delete items as well
	printf("\n");
	SortedListIteratorPtr vector_iter2 = SLCreateIterator(vector_sl);
	SortedListIteratorPtr vector_iter3 = SLCreateIterator(vector_sl);
	SLGetItem(vector_iter2);
	Vector * v = (Vector *) SLNextItem(vector_iter2);
	printf("Iterator 2 value: x=%d, y=%d\n", v->x, v->y);
	if(SLRemove(vector_sl, v3) == 0){
		printf("Error deleting Vector v3\n");
	} else {
		printf("Deleted Vector v3: x=3, y=3\n");
	}
	v = (Vector *) SLNextItem(vector_iter2); //w/o this causes err
	printf("Iterator 2 value: x=%d, y=%d\n", v->x, v->y); //w/o this causes err
	printf("\nThe modified list:\n");
	while(SLNextItem(vector_iter3) != NULL){
		Vector * vec = (Vector *) SLGetItem(vector_iter3);
		if (vec)
			printf("x=%d y=%d\n", vec->x, vec->y);
	}
	SLDestroyIterator(vector_iter2);
	SLDestroyIterator(vector_iter3);
	
	SortedListIteratorPtr vector_iter4 = SLCreateIterator(vector_sl);
	if(SLRemove(vector_sl, v5) == 0){
		printf("Error deleting Vector v5\n");
	} else {
		printf("Deleted Vector v5 (head): x=-6, y=3\n");
	}
	SLDestroyIterator(vector_iter4);
	
	SLDestroy(vector_sl);
	
	return 0;
} 
