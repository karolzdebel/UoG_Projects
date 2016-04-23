#ifndef _LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/*List node*/
typedef struct Node {
	void *data;		//Address of element in the list
	struct Node *next;			//Next node in the list
} Node;

/*List of elements*/
typedef struct List {
	int count;			//Number of elements in the list
	Node *first;		//First element in the list
	Node *last;
} List;

/*********************************************************************
 * FUNCTION NAME: insert
 * PURPOSE: Add element to the list.
 * ARGUMENTS: . List for the element to be added to(List *list).
 *			  . Element to be added(void *add).
 *********************************************************************/
extern void insert(List *list,void *add,int pos
	,void *(*copyFunct)(void *));
/*********************************************************************
 * FUNCTION NAME: remove
 * PURPOSE: Remove node from the list and free associated data.
 * ARGUMENTS: . List the node is being removed from(List *list).
 *			  . Position to be removed from (int pos).
 *			  . Function which frees the 
 *				data(void (*freeData)(void *)).
 *********************************************************************/
extern void removeData(List *list,int pos
	,void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: get
 * PURPOSE: Gets copy of data at a position in the list.
 * ARGUMENTS: . List which contains the data.
 *			  . Position that  the data is located at.
 *			  . Function which gets a copy of the  
 *				data(void (*getCopy)(void *)).
 *********************************************************************/
extern void *get(List *list, int pos
	,void *(*copyFunct)(void *));
/*********************************************************************
 * FUNCTION NAME: removeListSoft
 * PURPOSE: Remove node from the list without freeing the data.
 * ARGUMENTS: . List the node is being removed from(List *list).
 *			  . Position to be removed from (int pos).
 *********************************************************************/
extern void removeSoft(List *list,int pos);
/*********************************************************************
 * FUNCTION NAME: createList
 * PURPOSE: Free memory allocated to list.
 * ARGUMENTS: . List to be free'd (List **list).
 * 			  . Function that frees node data.
 *********************************************************************/
extern void createList(List **list);
/*********************************************************************
 * FUNCTION NAME: mergeList
 * PURPOSE: Add contents of one list to the other and free the other.
 * ARGUMENTS: . List to be be added to (List *list).
 * 			  . List being added than free'd(List *add).
 *********************************************************************/
extern void mergeList(List *list, List *add);
/*********************************************************************
 * FUNCTION NAME: copyList
 *********************************************************************/
extern List *copyList(List *list, void *(*copyFunct)(void*));
/*********************************************************************
 * FUNCTION NAME: getListNode
 * PURPOSE: Get any node in the list.
 * ARGUMENTS: . List which contains the node (List **list).
 * 			  . Position of the node.
 * RETURNS: Address of the node found at given position
 *			in the list(Node *).
 *********************************************************************/
extern Node *getNode(List *list,int pos);
/*********************************************************************
 * FUNCTION NAME: freeListHard
 * PURPOSE: Free memory allocated to list and all data.
 * ARGUMENTS: . List to be free'd (List **list).
 * 			  . Function that frees node data.
 *********************************************************************/
extern void destroyList(List **list, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: freeListSoft
 * PURPOSE: Free memory allocated to list but not data.
 * ARGUMENTS: . List to be free'd (List **list).
 *********************************************************************/
extern void freeSoft(List **list);
/*********************************************************************
 * FUNCTION NAME: printList
 * PURPOSE: Print all list data.
 * ARGUMENTS: . List to be printed (List list).
 * 			  . Function that prints node 
 *              data(void (*printFunct)(void*)).
 *********************************************************************/
extern void printList(List list, void (*printFunct)(void*));
/*********************************************************************
 * FUNCTION NAME: printNode
 * PURPOSE: Print node data.
 * ARGUMENTS: . Node to be printed (Node node).
 * 			  . Function that prints node 
 *			    data (void (*printFunct)(void*)).
 *********************************************************************/
 extern void printNode(Node node, void (*printFunct)(void*));
/*********************************************************************
 * FUNCTION NAME: getHead
 * PURPOSE: Gets the head of the list.
 * ARGUMENTS: . Address of the list which contains the head(List *list).
 * RETURNS:   . Address of the head of the list(Node *).
 *********************************************************************/
extern Node *getHead(List *list);
/*********************************************************************
 * FUNCTION NAME: emptyList
 * PURPOSE: Checks whether list is empty.
 * ARGUMENTS: . List to be checked(List *list).
 * RETURNS: Whether the list is empty(Bool).
 *********************************************************************/
extern bool empty(List *list);
/*********************************************************************
 * FUNCTION NAME: getNext
 * PURPOSE: Gets next node in the list.
 * ARGUMENTS: . Address of the previous node to the one you
 *              want(Node node).
 * RETURNS:   . Address of the next node in the list(Node *).
 *********************************************************************/
extern Node *getNext(Node *node);
/*********************************************************************
 * FUNCTION NAME: getData
 * PURPOSE: Get the data stored inside a node.
 * ARGUMENTS: . Address of the node which stores the data(Node *node).
 * RETURNS:   . Address of the data(void *).
 *********************************************************************/
extern void *getData(Node *node);
/*********************************************************************
 * FUNCTION NAME: freeNodes
 * PURPOSE: Free the node, and all nodes after it.
 * ARGUMENTS: . Node that is linked to preceeding nodes(Node *node).
 *			  . Function that frees data in that node and takes
 * 				the data as an argument (void (*freeData)(void *)).
 *********************************************************************/
extern void freeNodesHard(Node *node, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: freeNodeHard
 * PURPOSE: Free node and its data.
 * ARGUMENTS: . Node to be free'd(Node *node).
 *			  . Function which free's on the 
 *				node(void (*freeData)(void *)).
 *********************************************************************/
 extern void freeNodeHard(Node *node, void (*freeData)(void *));
/*********************************************************************
 * FUNCTION NAME: pop
 * PURPOSE: Remove last node in the queue.
 * ARGUMENTS: . Queue that the node is being removed 
 *              from(Queue *que).
 * RETURNS: Address of the node that was removed (Node *).
 *********************************************************************/
extern void *pop(List *que);
/*********************************************************************
 * FUNCTION NAME: push
 * PURPOSE: Add a node to the queue.
 * ARGUMENTS: . Queue the node is being added to(Queue *queue).
 *			  . Node that is being added to the queue(Node *node).
 *********************************************************************/
extern void push(List *queue, void *node
	,void *(*copyFunct)(void *));
/*********************************************************************
 * FUNCTION NAME: getTop
 * PURPOSE: Get top of queue.
 * ARGUMENTS: . Queue which contains the data(List *queue).
 * RETURNS: . Data associated with the top node(void *).
 *********************************************************************/
extern void *getTop(List *queue,void *(*copyFunct)(void*));
#define _LIST_H
#endif