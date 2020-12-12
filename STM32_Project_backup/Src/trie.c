#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"

int initTrieNode(TrieNode* newNode) {
	newNode->validLetter=0;
	return 1;
}	

TrieNode* newTrieNode(TrieNode* newNode) {
	newNode=(TrieNode*) malloc(sizeof(TrieNode));
	initTrieNode(newNode);
	return newNode;
}
