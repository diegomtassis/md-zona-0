/*
 * doubly_linked_list.h
 *
 *  Created on: Jan 3, 2020
 *      Author: diegomtassis
 */

#ifndef INC_FWK_DOUBLY_LINKED_LIST_H_
#define INC_FWK_DOUBLY_LINKED_LIST_H_

#include <types.h>

typedef struct DLL DLL;
typedef struct DLLNode DLLNode;

struct DLLNode {
	const void* e;
	DLLNode* prev;
	DLLNode* next;
	DLL* list;
};

struct DLL {
	DLLNode* first;
	DLLNode* last;
	u8 count;
};

/**
 * @brief initializes the list
 *
 * @param list
 */
void DLL_init(DLL* list);

/**
 * @brief releases the list, removing all the nodes
 *
 * @param list
 */
void DLL_release(DLL* list);

/**
 * @brief adds an element at the end
 *
 * @param list
 * @param e
 * @return the node containing the element
 */
DLLNode* DLL_add(DLL* list, const void* e);

/**
 * @brief adds an element after a given node
 *
 * @param list
 * @param node the node after which to insert the new element
 * @param e the element to add
 * @return the node containing the new element
 */
DLLNode* DLL_addAfter(DLL* list, DLLNode* node, const void* e);

/**
 * @brief finds whether a given element exists in a list
 *
 * @param list
 * @param e
 * @return the node containing the element
 */
DLLNode* DLL_find(DLL* list, const void* e);

/**
 * @brief removes a node form a list
 *
 * @param list
 * @param node
 */
void DLL_remove(DLL* list, DLLNode* node);

#endif /* INC_FWK_DOUBLY_LINKED_LIST_H_ */
