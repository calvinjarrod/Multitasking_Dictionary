#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dict.h"

int initDict(Dictionary *Dict) {
	Dict->numEntries=0;
	Dict->entries.root = NULL;
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

int addDict(struct Dictionary *Dict, char *word, size_t length) {
	int i = 0;
	struct TrieNode *currentNode = Dict->entries.root;
	while (i < length) {
		// trie is empty
		if (currentNode == NULL) {
			/*Dict->entries.root = newTrieNode(Dict->entries.root);	
			markLetterInTrieNode(Dict->entries.root,word[i]);
			// create a new node for every marked letter
			Dict->entries.root->next[char2Indx(word[i])] = 
				newTrieNode(Dict->entries.root->next[char2Indx(word[i])]);
			
			currentNode=Dict->entries.root;
			i++;*/
			return 0;
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
	return 1;
}

int removeDict(struct Dictionary *Dict, char *word, size_t length) {
	if (!checkDict(Dict,word,length)) {;return 0;}
	TrieNode *dict_root = Dict->entries.root;
	if (dict_root == NULL) {return 0;}
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
	return 1;
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
