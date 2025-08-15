/*
 * doubly_linked_list.c
 *
 *  Created on: Jan 3, 2020
 *      Author: diegomtassis
 */

#include "../../inc/fwk/doubly_linked_list.h"

#include <genesis.h>

#include "../../inc/fwk/commons.h"

void DLL_init(DLL* list) {

	list->first = 0;
	list->last = 0;
	list->count = 0;
}

void DLL_release(DLL* list) {

	while (list->first) {
		DLL_remove(list, list->first);
	}

	list->first = 0;
	list->last = 0;
	list->count = 0;
}

DLLNode* DLL_add(DLL* list, const void* e) {

	if (!list) {
		return 0;
	}

	DLLNode* node = MEM_calloc(sizeof(*node));
	node->e = e;
	node->list = list;

	if (list->first) {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;

	} else {
		list->first = node;
		list->last = node;
	}

	list->count++;

	return node;
}

DLLNode* DLL_addAfter(DLL* list, DLLNode* node, const void* e) {

	if (!list || !node || node->list != list) {
		return 0;
	}

	DLLNode* newNode = MEM_calloc(sizeof(*newNode));
	newNode->e = e;
	newNode->list = list;

	newNode->prev = node;
	newNode->next = node->next;

	if (node->next) {
		node->next->prev = newNode;
	} else {
		// node was the last node, update list->last
		list->last = newNode;
	}

	node->next = newNode;

	list->count++;

	return newNode;
}

DLLNode* DLL_find(DLL* list, const void* e) {

	if (!list || !e) {
		return 0;
	}

	DLLNode* node = list->first;

	while (node) {
		if (node->e == e) {
			return node;
		}
		node = node->next;
	}

	return 0;
}

void DLL_remove(DLL* list, DLLNode* node) {

	if (!list || !node || !list->count || node->list != list) {
		return;
	}

	if (list->first == node) {
		list->first = node->next;
		if (list->first) {
			list->first->prev = 0;
		}
	}

	if (list->last == node) {
		list->last = node->prev;
		if (list->last) {
			list->last->next = 0;
		}
	}

	node->e = 0;
	node->list = 0;
	node->prev = 0;
	node->next = 0;

	MEM_free(node);

	list->count--;
}

