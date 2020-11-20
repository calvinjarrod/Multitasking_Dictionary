#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LETTERS 26

typedef struct TrieNode {
	uint32_t validLetter;
	struct TrieNode *next[MAX_LETTERS];
}TrieNode;

int initTrieNode(TrieNode* newNode) {
	newNode->validLetter=0;
}	

TrieNode* newTrieNode(TrieNode* newNode) {
	newNode=(TrieNode*) malloc(sizeof(TrieNode));
	initTrieNode(newNode);
	return newNode;
}

typedef struct Trie {
	TrieNode *root;
}Trie;

typedef struct Dictionary {
	Trie entries;
	uint32_t numEntries;
}Dictionary;

int initDict(Dictionary *Dict) {
	Dict->numEntries=0;
	Dict->entries.root = newTrieNode(Dict->entries.root);
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
			Dict->entries.root = (TrieNode *) malloc(sizeof(TrieNode));	
			initTrieNode(Dict->entries.root);
			markLetterInTrieNode(Dict->entries.root,word[i]);
			// create a new node for every marked letter
			Dict->entries.root->next[char2Indx(word[i])] = newTrieNode(Dict->entries.root->next[char2Indx(word[i])]);
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
}

int removeDict(struct Dictionary *Dict, char *word, size_t length) {
	TrieNode *dict_root = Dict->entries.root;
	if (dict_root == NULL) return 0;
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
			if (checkAnyLetterInTrieNode(current)) return 1;
			free(current);
			unmarkLetterInTrieNode(parent,word[i]);
			current = parent;
			// get node above current again
			parent = dict_root; indx=0;
			while (parent->next[char2Indx(word[indx])] != current)
				parent=parent->next[char2Indx(word[indx++])];
		} else {
			free(current);
			unmarkLetterInTrieNode(parent,word[i]);
			current=parent;
			parent = dict_root; int indx=0;
			// seg faulting here... seems to keep going forever
			while (parent->next[char2Indx(word[indx])] != current)
				parent=parent->next[char2Indx(word[indx++])];
		}
	}
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


int main(void) {
	Dictionary dict;
	initDict(&dict);

	char *words = "thebothandis";
	uint8_t lengths[4]={3,4,3,2};
	char *notInTrie = "that";

	int indx=0;
	for (int i = 0; i < 4; i++) {
		addDict(&dict,(words+indx),lengths[i]);
		for (int j = lengths[i]; j > 0; j--) {
			printf("%c",words[indx++]);
		}
		printf(" ");
	}
	printf("\n");
	printf("Delete \"the\":%d\n",removeDict(&dict,words,3));
	printf("\n");
	printf("%d\n",checkDict(&dict,words,lengths[0]));
	printf("%d\n",checkDict(&dict,(words+3),lengths[1]));
	printf("%d\n",checkDict(&dict,(words+7),lengths[2]));
	printf("%d\n",checkDict(&dict,(words+10),lengths[3]));
	printf("%d\n",checkDict(&dict,notInTrie,4));
	
}



