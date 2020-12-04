#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dict.h"

int initDict(Dictionary *Dict) {
	Dict->numEntries=0;
	Dict->activeReaders=0;
	Dict->activeWriters=0;
	Dict->waitingReaders=0;
	Dict->waitingWriters=0;
	Dict->entries.root = newTrieNode(Dict->entries.root);
	return 1;
}

char lowerCaseify(char c) {
	if (c < 97) return c+32;
	else return c;
}

int checkAlpha(char c) {
	if ((c < 91 || c > 96) && (c > 64 && c < 123)) return 1;
	else return 0;
}

int char2Indx (char c) {
	if (!checkAlpha(c)) return -1;
	else return lowerCaseify(c)-97;
}

int checkLetterInTrieNode(struct TrieNode *node, char c) {
	int indx = char2Indx(c);
	if (node->validLetter & 1<<indx) return 1;
	else return 0;
}

int checkAnyLetterInTrieNode(struct TrieNode *node) {
	if (node->validLetter) return 1;
	else return 0;
}

void markLetterInTrieNode(struct TrieNode *node, char c) {
	int indx = char2Indx(c);
	node->validLetter |= (1 << indx);
}

void unmarkLetterInTrieNode(struct TrieNode *node, char c) {
	int indx = char2Indx(c);
	node->validLetter &= ~(1<<indx);
}

int Dict_Task_Add(Dictionary *Dict, char *word, size_t length) {
	Dict_startWrite(Dict);
	int result = addDict(Dict,word,length);
	Dict_doneWrite(Dict);
	return result;
}

int addDict(struct Dictionary *Dict, char *word, size_t length) {
	osMutexWait(dictTrieLock,osWaitForever);
	int i = 0;
	struct TrieNode *currentNode = Dict->entries.root;
	while (i < length) {
		// trie is empty
		if (currentNode == NULL) {
			Dict->entries.root = (TrieNode *) malloc(sizeof(TrieNode));	
			initTrieNode(Dict->entries.root);
			markLetterInTrieNode(Dict->entries.root,word[i]);
			// create a new node for every marked letter
			Dict->entries.root->next[char2Indx(word[i])] = 
				newTrieNode(Dict->entries.root->next[char2Indx(word[i])]);
			
			currentNode=Dict->entries.root;
			i++;
		// not empty, but current letter not in current node 
		} else if (!checkLetterInTrieNode(currentNode,word[i])) {
			markLetterInTrieNode(currentNode,word[i]);
			// create a new node for every marked letter
			currentNode->next[char2Indx(word[i])] = newTrieNode(currentNode->next[char2Indx(word[i])]);
			currentNode=currentNode->next[char2Indx(word[i])];
			i++;
		// current letter is marked in current node
		} else {
			currentNode=currentNode->next[char2Indx(word[i])];
			i++;
		}
	}
	Dict->numEntries += 1;
	osMutexRelease(dictTrieLock);
	return 1;
}

int Dict_Task_Remove(Dictionary *Dict, char *word, size_t length) {
	Dict_startWrite(Dict);
	int result = removeDict(Dict,word,length);
	Dict_doneWrite(Dict);
	return result;
}

int removeDict(struct Dictionary *Dict, char *word, size_t length) {
	osMutexWait(dictTrieLock,osWaitForever);
	if (!checkDict(Dict,word,length)) {osMutexRelease(dictTrieLock);return 0;}
	TrieNode *dict_root = Dict->entries.root;
	if (dict_root == NULL) {osMutexRelease(dictTrieLock);return 0;}
	TrieNode *current=dict_root, *parent=NULL;
	for (int i = length-1; i >= 0; i--) {
		if (current == parent) {
			unmarkLetterInTrieNode(current,word[i]);
		} else if (i == length-1) { // first time through loop, need to set current node
			int indx=0;
			while (indx < length) {
				parent=current;
				current=current->next[char2Indx(word[indx++])];
			}
			if (checkAnyLetterInTrieNode(current)) {
				osMutexRelease(dictTrieLock);
				return 1;
			}
			free(current);
			unmarkLetterInTrieNode(parent,word[i]);
			current = parent;
			// get node above current again
			parent = dict_root; indx=0;
			while (parent->next[char2Indx(word[indx])] != current && parent != current)
				parent=parent->next[char2Indx(word[indx++])];
		} else {
			free(current);
			unmarkLetterInTrieNode(parent,word[i]);
			current=parent;
			parent = dict_root; int indx=0;
			while (parent->next[char2Indx(word[indx])] != current && parent != current)
				parent=parent->next[char2Indx(word[indx++])];
		}
	}
	Dict->numEntries -= 1;
	osMutexRelease(dictTrieLock);
	return 1;
}

int Dict_Task_Search(Dictionary *Dict, char * word, size_t length) {
	Dict_startRead(Dict);
	int result = checkDict(Dict,word,length);
	Dict_doneRead(Dict);
	return result;
}

int checkDict(struct Dictionary *Dict, char *word, size_t length) {
	int i = 0;
	struct TrieNode *currentNode = Dict->entries.root;
	while (i < length) {
		if (currentNode == NULL) {
			return 0;
		} else if (!checkLetterInTrieNode(currentNode,word[i])) {
			return 0;
		} else {
			currentNode=currentNode->next[char2Indx(word[i])];
			i++;
		}
		//printf("Current letter: %c and %d in table",word[i],checkLetterInTrieNode(currentNode,word[i]));
	}
	return 1;
}

void Dict_startRead(Dictionary *Dict) {
	osMutexWait(dictTrieLock,osWaitForever);
	Dict->waitingReaders++;
	osMutexRelease(dictTrieLock);
	while (Dict->activeWriters > 0 || Dict->waitingWriters > 0) {
		osSemaphoreWait(dictReadSema,osWaitForever);
	}
	osMutexWait(dictTrieLock,osWaitForever);
	osSemaphoreWait(dictWriteSema,osWaitForever);
	Dict->waitingReaders--;
	Dict->activeReaders++;
	osMutexRelease(dictTrieLock);
}

void Dict_doneRead(Dictionary *Dict) {
	osMutexWait(dictTrieLock,osWaitForever);
	Dict->activeReaders--;
	if (Dict->activeReaders == 0 && Dict->waitingWriters > 0) {
		osSemaphoreRelease(dictWriteSema);
}
void Dict_startWrite(Dictionary *Dict);
void Dict_doneWrite(Dictionary *Dict);
