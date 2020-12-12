#include <stdint.h>
#include "cmsis_os.h"
#include "trie.h"

typedef struct Dictionary {
	Trie entries;
	uint32_t numEntries;
}Dictionary;

int initDict(Dictionary *Dict);
char lowerCaseify(char c);
int checkAlpha(char c);
int char2Indx (char c);
int checkLetterInTrieNode(struct TrieNode *node, char c);
int checkAnyLetterInTrieNode(struct TrieNode *node);
void markLetterInTrieNode(struct TrieNode *node, char c);
void unmarkLetterInTrieNode(struct TrieNode *node, char c);

int addDict(struct Dictionary *Dict, char *word, size_t length);
int removeDict(struct Dictionary *Dict, char *word, size_t length);
int checkDict(struct Dictionary *Dict, char *word, size_t length);
