#include <stdint.h>
#define MAX_LETTERS 26

typedef struct TrieNode {
	uint32_t validLetter;
	struct TrieNode *next[MAX_LETTERS];
}TrieNode;

int initTrieNode(TrieNode* newNode);

TrieNode* newTrieNode(TrieNode* newNode);

typedef struct Trie {
	TrieNode *root;
}Trie;
