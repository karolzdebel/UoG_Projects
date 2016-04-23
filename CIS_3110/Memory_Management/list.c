#include "list.h"

/*********************************************************************
 * FUNCTION NAME: printNodes
 * PURPOSE: Print node, and all the following nodes in order.
 * ARGUMENTS: . Node that is linked to preceeding nodes(Node *node).
 *			  . Function that prints data in that node and takes
 * 				the data as an argument (void (*printFunct)(void *)).
 *********************************************************************/
static void printNodes(Node *node, void (*printFunct)(void *));

static void printNodes(Node *node, void (*printFunct)(void *)){
	printFunct(node->data);
	if (node->next){
		printNodes(node->next,printFunct);
	}
}

extern Node *getHead(List *list){
	return list->first;
}

extern Node *getNext(Node *node){
	return node->next;
}

extern void *getData(Node *node){
	return node->data;
}

extern void insert(List *list,void *add, int pos
	,void *(*copyFunct)(void *)){
	Node *next,*temp=NULL;

	/*Get copy*/
	void *copy = copyFunct(add);

	/*Add to first position*/
	if (pos == 1){
		if (list->first){
			temp = list->first;
		}
		list->first = malloc(sizeof(Node));
		list->first->next = NULL;
		list->first->data = copy;
		if (temp){
			list->first->next = temp;
		}
	}

	else{
		/*Traverse to node before position in the list*/
		next = getHead(list);
		for (int i=0;i<pos-2;i++){
			next = getNext(next);
		}
		/*Add to middle*/
		if (next->next){		
			temp = next->next;
			next->next = malloc(sizeof(Node));
			next->next->data = copy;
			next->next->next = temp;
		}
		/*Add to last*/
		else{
			next->next = malloc(sizeof(Node));
			next->next->data = copy;
			next->next->next = NULL;
		}

	}

	list->count++;
}

extern void removeData(List *list,int pos
	,void (*freeData)(void *)){

	Node *next,*temp;

	/*Remove from position 1*/
	if (pos == 1){
		temp = list->first;
		if (list->count > 1){
			list->first = list->first->next;
		}
		else{
			list->first = NULL;
		}
		freeData(getData(temp));
		free(temp);
		list->count--;
		return;
	}

	/*Remove from middle or end*/
	next = getHead(list);
	for (int i=0;i<pos-2;i++){
		next = getNext(next);
	} 
	temp = next->next;
	/*Remove from middle*/
	if (pos < list->count){
		next->next = next->next->next;
	}
	/*Remove from last*/
	else{
		next->next = NULL;
	}
	freeData(getData(temp));
	free(temp);
	list->count--;
}

extern void *get(List *list, int pos
	,void *(*copyFunct)(void *)){

	/*Copy of data*/
	return copyFunct(getData(getNode(list,pos)));
}

extern void removeSoft(List *list,int pos){
	Node *next,*temp;

	/*Remove from position 1*/
	if (pos == 1){
		temp = list->first;
		if (list->count > 1){
			list->first = list->first->next;
		}
		else{
			list->first = NULL;
		}
		free(temp);
		list->count--;
		return;
	}

	/*Remove from middle or end*/
	next = getHead(list);
	for (int i=0;i<pos-2;i++){
		next = getNext(next);
	}
	temp = next->next;
	/*Remove from middle*/
	if (pos < list->count){
		next->next = next->next->next;
	}
	/*Remove from last*/
	else{
		next->next = NULL;
	}
	free(temp);
	list->count--;
}

extern Node *getNode(List *list,int pos){
	Node *next;

	next = getHead(list);
	for (int i=0;i<pos-1;i++){
		next = getNext(next);
	}
	return next;
}

extern void freeNodesHard(Node *node, void (*freeData)(void *)){
	if (node->next){
		freeNodesHard(node->next,freeData);
	}
	freeData(node->data);
	free(node);
}

extern void freeNodesSoft(Node *node){
	if (node->next){
		freeNodesSoft(node->next);
	}
	free(node);
}

extern void freeNodeHard(Node *node, void (*freeData)(void *)){
	freeData(node->data);
	free(node);
}

extern void destroyList(List **list, void (*freeData)(void *)){
	if ((*list)->first){
		freeNodesHard((*list)->first,freeData);
	}
	free(*list);
}

extern void freeSoft(List **list){
	if ((*list)->first){
		freeNodesSoft((*list)->first);
	}
	free(*list);
}

extern void createList(List **list){
	*list = malloc(sizeof(List));
	(*list)->count = 0;
	(*list)->first = NULL;
}

extern void printList(List list, void (*printFunct)(void*)){
	if (list.first){
		printNodes(list.first,printFunct);
	}
}

extern void mergeList(List *list, List *add){
	Node *nodeOrig,*headAdd;

	headAdd = getHead(add);

	/*If original list is empty*/
	if (!list->first){
		list->first = headAdd;
	}
	/*Add to end of current list*/
	else{
		nodeOrig = getHead(list);
		while(getNext(nodeOrig)){
			nodeOrig = getNext(nodeOrig);
		} 
		nodeOrig->next = getHead(add);
	}
	free(add);
}

extern void printNode(Node node, void (*printFunct)(void*)){
 	printFunct(node.data);
}

extern bool empty(List *list){
	return (list->count == 0);
}

extern void push(List *que, void *data
	,void *(*copyFunct)(void *)){	
	insert(que,data, que->count+1,copyFunct);
}

extern void *pop(List *que){
	void *data = getData(getNode(que,1));
	removeSoft(que,1);
	return data;
}

extern void *getTop(List *queue,void *(*copyFunct)(void*)){
	return get(queue,1,copyFunct);
}

extern List *copyList(List *list, void *(*copyFunct)(void*)){
	List *copy;
	void *data;

	createList(&copy);
	for (int i=1;i<=list->count;i++){
		data = get(list,i,copyFunct);
		push(copy,data,copyFunct);
		free(data);
	}
	return copy;
}