/*
	Sangini Shah and Rob Williams
	CS214 Systems Programming
	Programming Assignment 2- Sorted List
	
	sorted-list.c
*/

#include <stdlib.h>
#include "sorted-list.h"

/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list, and a destruct function that gets rid of the objects
 * once they are no longer in the list or referred to in an iterator.
 * 
 * If the function succeeds, it returns a (non-NULL) SortedListT object,
 * otherwise, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
	
	SortedListPtr list = (SortedListPtr) malloc(sizeof(SortedList));
	
	if(cf == NULL || df == NULL || list == NULL)
		return NULL;
		
	list -> comparator = cf;
	list -> destroyer = df;
	list -> head = NULL;
	return list;
}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list){
	
	if(list == NULL)
		return;
		
	SortedListNodePtr current = list -> head;
	while(current != NULL) {
		list -> destroyer(current -> data); //free any dynamically allocated data
		SortedListNodePtr temp = current -> next;
		free(current);
		current = temp;
	}
	free(list);
}

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1, othrewise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj){
	///create the new ListNode
	SortedListNodePtr obj;
	SortedListNodePtr cur,prv; //list iteration
	int res; //the result of the compare
	
	if(!list || !newObj) //We done goofed, something is null
		return 0;

	//Set up the new object
	obj = (SortedListNodePtr)malloc(sizeof(SortedListNode));
	if(!obj)
		return 0; //we didn't get the allocated memory
	
	obj->ref_count = 1;
	obj->flag  = 0;
	obj->data = newObj;
	obj->next = NULL;
	
	
	//If the list is empty, set this object as the head
	if(!list->head){
		obj->next = NULL;
		list->head = obj;
		return 1; //exit function
	}
	//Case: Larger than head
	cur = list->head;
	res = list->comparator(obj->data,cur->data);
	if(res == 0 || res == 1){
		obj->next = cur;
		list->head = obj;
		return 1;
	}else if(!cur->next){
		cur->next = obj;
		return 1;
	}
	
	//General Case: It is in the body of the list
	prv = cur; 
	cur = cur->next;
	while(cur){
		res = list->comparator(obj->data,cur->data);
		switch(res){
			case 1: //obj is larger than cur
			case 0: //obj and cur are the same size, but obj before cur
				prv->next = obj;
				obj->next = cur;
				return 1; //exit function
			case -1: //obj is smaller than cur
				if(!cur->next){ //nothing after cur? Put obj there
					cur->next = obj;
					return 1; //exit function
				}
				prv = cur;
				cur = cur->next;
		}//end switch	
	}//end while

	//I don't know how, but we done messed up
	return 0;
}

/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.  SLRemove may not change the object whose
 * pointer is passed as the second argument.  This allows you to pass
 * a pointer to a temp object equal to the object in the sorted list you
 * want to remove.
 *
 * If the function succeeds, it returns 1, otherwise it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *newObj){
	
	if(list == NULL)
		return 0;
	
	SortedListNodePtr previous = NULL;
	SortedListNodePtr current = list->head;
	while(current) {
		if(list->comparator(current->data, newObj) == 0){
			if(previous == NULL){ //First node in the list
				list->head = current->next;
				
				if(current -> next != NULL) {
					current->next->ref_count = current->next->ref_count + 1;
				}
				
				current -> ref_count = current -> ref_count - 1;
				if(current -> ref_count == 0){
					list->destroyer(current->data);
					if(current->next){
						current->next->ref_count--;
					}
					free(current);
				}
				else
					current -> flag = 1; //flag it for deletion. This assumes it is only in one sorted list, but multiple iterators
				return 1;
			}
			else {
				previous -> next = current -> next;
				
				if(current -> next != NULL) {
					current->next->ref_count = current->next->ref_count + 1;
				}
				
				current -> ref_count = current -> ref_count - 1;
				if(current -> ref_count == 0){
					list->destroyer(current->data);
					if(current->next){
						current->next->ref_count--;
					}
					free(current);
				}
				else
					current -> flag = 1; //flag it for deletion. This assumes it is only in one sorted list, but multiple iterators
				return 1;
			}
		}
		previous = current;
		current = current -> next;
	}
	return 0;
}

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL pointer to a
 * SortedListIterT object, otherwise it returns NULL.  The SortedListT
 * object should point to the first item in the sorted list, if the sorted
 * list is not empty.  If the sorted list object is empty, then the iterator
 * should have a null pointer.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
	
	if(list == NULL)
		return NULL;
		
	SortedListIteratorPtr iterator = (SortedListIteratorPtr) malloc(sizeof(SortedListIterator));
	iterator -> list = list;
	iterator -> curr = list -> head;
	if(iterator->curr){
		iterator -> curr -> ref_count++;
	}
	return iterator;
}

/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affect the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter){
	SLGetItem(iter); //update to prevent memory errors
	//if the current is flagged for deletion and this is it's last pointer, destroy it
	if (iter->curr){
		if (iter->curr->flag == 1 && iter->curr->ref_count == 1){
			iter->list->destroyer(iter->curr);
			if(iter->curr->next)
				iter->curr->next->ref_count--;
			free(iter->curr);
		}
	}
	//destroy the iterator
	free(iter);
}


/*
 * SLNextItem returns the pointer to the data associated with the
 * next object in the list encapsulated by the given iterator.
 * It should return a NULL when the end of the list has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */
void * SLNextItem(SortedListIteratorPtr iter){
	void* ret;

	if(!iter || !iter->curr) //valid iterator?
		return 0;
	
	if(iter->curr && iter->curr->flag == 1) //If the current is flagged for deletion, update the iterator
		SLUpdateIterator(iter);
	
	/*General Case*/
	if(iter->curr){//Still stuff left in the list?
		
		iter->curr->ref_count--; //since we will change the pointer
		iter->curr = iter->curr->next; //advance the iterators position
		
		if (iter->curr){
			iter->curr->ref_count++; //increase the pointer reference
			ret = iter->curr->data; //set the pointer to return the data
			return ret;
		}
		return NULL;
	}
	else //List has been iterated
		return NULL;
		
}


/*
 * SLGetItem returns the pointer to the data associated with the
 * SortedListIteratorPtr.  It should return 0 if the iterator
 * advances past the end of the sorted list.
 * 
 * You need to fill in this function as part of your implementation.
 *
 ** if the cur->ref_count == 1, then the current iterator is the only 
 ** thing pointing to it and thus can be deleted with no risk.
*/

void * SLGetItem( SortedListIteratorPtr iter ){
	if(!iter || !iter->curr) //valid iterator?
		return 0;
	
	if(iter->curr && iter->curr->flag == 1) //If the current is flagged for deletion, update the iterator
		SLUpdateIterator(iter);
	
	if(iter->curr){//Still stuff left in the list?
		void * ret = iter->curr->data; //set the pointer to return the data
		return ret;
	}
	else //List has been iterated
		return 0;
}


/*
 * The Sorted List search function finds the next smaller (or same)item in a
 * Sorted List. This function is not meant for the general public to use
 * This function is called by SLGetItem in the event that someone is a jerk
 * and removed an item from the list using SLNextItem and SLRemove
 * 
 * This function is certifiably awesome
 * If this function returns NULL, then one of two things had occurred
 * First, there was bad input
 * Seconds, there is no element <= data
 */
SortedListNodePtr SLSearch(SortedListPtr list, void* data){
	SortedListNodePtr cur;
	if(!list || !data)
		return NULL; //He's dead. We failed. No Hope.
	
	cur = list->head;
	while(cur && list->comparator(cur->data, data) > 0){
		//increment until you find something smaller than data
		//or you run out of things
		cur = cur->next;
	}
	
	return cur;
}

/*
 * This function will update the given iterator in the even that an item has been
 * removed from the list associated with the iterator. 
 */
void SLUpdateIterator(SortedListIteratorPtr iter){
	if(iter->curr->flag == 1){ //Update the iterator if needed
	
		SortedListNodePtr temp = SLSearch(iter->list, iter->curr->data); //returns the next valid iterator
		
		/*Clean up the old data if needed*/
		if(iter->curr->ref_count == 1){ //this is the last pointer to it,
			iter->list->destroyer(iter->curr->data);//delete the data in cur
			if(iter->curr->next)
				iter->curr->next->ref_count--;
			free(iter->curr);
		}
		/*Re-establish the Iterator for the general algorithm*/
		if(temp)
			temp->ref_count++;
		iter->curr = temp;	
	}
}