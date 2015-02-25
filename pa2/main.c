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
	Vector * v = (Vector *) SLGetItem(vector_iter1);
	while(v){
		if (v){
			printf("x=%d y=%d\n", v->x, v->y);
		}
		v = (Vector *) SLNextItem(vector_iter1);
	}
	SLDestroyIterator(vector_iter1);
	
	//Create second iterator to iterate to a node in the middle of a list.
	//Destroy that node and print value of iterator. 
	//This value should be updated to point to the next item in the list.
	//A third iterator is used to run through list and show that it has been modified since it was created.
	printf("\n");
	SortedListIteratorPtr vector_iter2 = SLCreateIterator(vector_sl);
	SortedListIteratorPtr vector_iter3 = SLCreateIterator(vector_sl);
	v = (Vector *) SLNextItem(vector_iter2); //move to second node in list
	printf("Iterator 2 value: x=%d, y=%d\n", v->x, v->y);
	if(SLRemove(vector_sl, v3) == 0){
		printf("Error deleting Vector v3 (second in list)\n");
	} else {
		printf("Deleted Vector v3: x=3, y=3\n");
	}
	v = (Vector *) SLGetItem(vector_iter2);
	printf("Iterator 2 value: x=%d, y=%d\n", v->x, v->y);
	
	//Print out the modified list
	printf("\nThe modified list:\n");
	v = (Vector *) SLGetItem(vector_iter3);
	while(v){
		if (v){
			printf("x=%d y=%d\n", v->x, v->y);
		}	
		v = (Vector *) SLNextItem(vector_iter3);
	}
	SLDestroyIterator(vector_iter2);
	SLDestroyIterator(vector_iter3);
	
	//Create a fourth iterator that points to the head of the list. 
	//Remove the head of the list as well as the tail of the list. 
	//Use the iterator to run through the modified list, then destroy it.
	SortedListIteratorPtr vector_iter4 = SLCreateIterator(vector_sl);
	if(SLRemove(vector_sl, v5) == 0){
		printf("\nError deleting Vector v5\n");
	} else {
		printf("\nDeleted Vector v5 (head): x=-6, y=3\n");
	}
	if(SLRemove(vector_sl, v2) == 0){
		printf("Error deleting Vector v2\n");
	} else {
		printf("Deleted Vector v2 (tail): x=0, y=3\n");
	}
	printf("\nThe modified list:\n");
	v = (Vector *) SLGetItem(vector_iter4);
	while(v){
		if (v){
			printf("x=%d y=%d\n", v->x, v->y);
		}	
		v = (Vector *) SLNextItem(vector_iter4);
	}
	SLDestroyIterator(vector_iter4);
	
	//Try to remove something not in the list
	printf("\nTrying to remove v2 again, which is no longer in the list:\n");
	if(SLRemove(vector_sl, v2) == 0){
		printf("Error deleting Vector v2\n");
	} else {
		printf("Deleted Vector v2 (end): x=0, y=3\n");
	}
	
	//Create more vectors for list in order to run more tests
	Vector * v6 = (Vector *) malloc(sizeof(Vector *));
	Vector * v7 = (Vector *) malloc(sizeof(Vector *));
	Vector * v8 = (Vector *) malloc(sizeof(Vector *));
	Vector * v9 = (Vector *) malloc(sizeof(Vector *));

	v6->x = 15; v6->y=2; //Will be the new head
	v7->x = 4; v7->y=5; //Second
	v8->x = 1; v8->y=4; //Fourth
	v9->x = 0; v9->y=0; //Tail
	
	//Creates an iterator that will point to the initial head of the list
	//The value of the iterator will not change even if there are nodes inserted
	//in front of the head.
	SortedListIteratorPtr vector_iter5 = SLCreateIterator(vector_sl);
	v = (Vector *) SLGetItem(vector_iter5);
	printf("\nIterator 5 value (original head): x=%d, y=%d\n", v->x, v->y);
	if(SLInsert(vector_sl, v6)!=0){
		printf("Inserted new head into list.\n");
		
		//Create new iterator to show that the head of the list has changed
		SortedListIteratorPtr vector_iter6 = SLCreateIterator(vector_sl);
		v = (Vector *) SLGetItem(vector_iter5);
		printf("Iterator 5 value (present from original list): x=%d, y=%d\n", v->x, v->y);
		v = (Vector *) SLGetItem(vector_iter6);
		printf("Iterator 6 value (created for new list): x=%d, y=%d\n", v->x, v->y);
		
		//Insert other nodes into the list and print out the list using iterator 6
		SLInsert(vector_sl, v7);
		SLInsert(vector_sl, v8);
		SLInsert(vector_sl, v9);
		
		printf("\nAdded new vectors to the list.\nThe current state of the list:\n");
		Vector * v = (Vector *) SLGetItem(vector_iter6);
		while(v){
			if (v){
				printf("x=%d y=%d\n", v->x, v->y);
			}
			v = (Vector *) SLNextItem(vector_iter6);
		}
		SLDestroyIterator(vector_iter5);
		SLDestroyIterator(vector_iter6);
		
		/*
		 * Creating three iterators pointing to three different consecutive nodes say A, B, C
		 * Going to remove nodes B and C 
		 * SLNextItem() should then return the same node for iterators B and C
		 */
		printf("\nCreating three iterators for the same list:\n");
		SortedListIteratorPtr vector_iterA = SLCreateIterator(vector_sl);
		SortedListIteratorPtr vector_iterB = SLCreateIterator(vector_sl);
		SortedListIteratorPtr vector_iterC = SLCreateIterator(vector_sl);
		if(vector_iterA && vector_iterB && vector_iterC){
			printf("Created Iterator A.\n");
			printf("Created Iterator B.\n");
			printf("Created Iterator C.\n");
			
			SLNextItem(vector_iterA); //Points to the second item in the list
			
			SLNextItem(vector_iterB);
			SLNextItem(vector_iterB); //Points to the third item in the list
			
			SLNextItem(vector_iterC);
			SLNextItem(vector_iterC);
			SLNextItem(vector_iterC); //Points to the fourth item in the list
			
			Vector * vA = (Vector *) SLGetItem(vector_iterA);
			Vector * vB = (Vector *) SLGetItem(vector_iterB);
			Vector * vC = (Vector *) SLGetItem(vector_iterC);
			
			printf("\nIterator A value (node A): x=%d, y=%d\n", vA->x, vA->y);
			printf("Iterator B value (node B): x=%d, y=%d\n", vB->x, vB->y);
			printf("Iterator C value (node C): x=%d, y=%d\n", vC->x, vC->y);
			
			if(SLRemove(vector_sl, vB) == 1){
				printf("\nNode B removed from list.\n");
			}
			if(SLRemove(vector_sl, vC) == 1){
				printf("Node C removed from list.\n");
			}
			
			//Values of iterators after deletion
			vA = (Vector *) SLGetItem(vector_iterA);
			vB = (Vector *) SLGetItem(vector_iterB);
			vC = (Vector *) SLGetItem(vector_iterC);
			printf("\nValues of iterators after deletion:\n");
			printf("Iterator A value (node A): x=%d, y=%d\n", vA->x, vA->y);
			printf("Iterator B value (node D): x=%d, y=%d\n", vB->x, vB->y);
			printf("Iterator C value (node D): x=%d, y=%d\n", vC->x, vC->y);
			
			SLDestroyIterator(vector_iterA);
			SLDestroyIterator(vector_iterB);
			SLDestroyIterator(vector_iterC);
			
		} else {
			printf("Error Creating Iterators.\n");
		}
	}
	else {
		printf("Error inserting new head.\n");
	}
	
	SLDestroy(vector_sl);
	
	
	//Create an iterator for empty list using the integers
	SortedListPtr empty_sl = (SortedListPtr) malloc(sizeof(SortedListPtr));
	
	int (*i_compare_ptr)(void *, void *) = &int_compare;
	void (*i_destroy_ptr)(void *) = &int_destroy;
	
	empty_sl = SLCreate(i_compare_ptr, i_destroy_ptr);
	if(empty_sl){
		printf("\nCreated a new empty list.\n");
		SortedListIteratorPtr empty_iter = SLCreateIterator(empty_sl);
		if(empty_iter){
			printf("Created an iterator for the empty list.\n");
			int * intptr = (int *) SLGetItem(empty_iter);
			if(!intptr){
				printf("Iterator is pointing to null.\n");
			}
			else {
				printf("Value of iterator: %d", *intptr);
			}
		}
		else{
			printf("Error creating iterator for empty list.\n");
		}
		SLDestroyIterator(empty_iter);
	}
	else{
		printf("Error creating a new empty list.\n");
	}
	SLDestroy(empty_sl);
	
	// Create a null list and attempt to assign it an iterator
	SortedListPtr int_sl = NULL;
	SortedListIteratorPtr int_iter = SLCreateIterator(int_sl);
	if(int_iter){
		printf("\nWell, that ain't right. We just created an iterator for a null list.\n");
		SLDestroyIterator(int_iter);
	} else{
		printf("\nError creating iterator for null list.\n");
	}
	
	printf("\nTest Cases Complete.\n\n");
	return 0;
} 
