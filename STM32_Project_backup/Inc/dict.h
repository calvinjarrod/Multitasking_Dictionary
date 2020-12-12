#include <stdint.h>
#include "cmsis_os.h"
#include "trie.h"

typedef struct Dictionary {
	Trie entries;
	uint8_t activeWriters;
	uint8_t activeReaders;
	uint8_t waitingWriters;
	uint8_t waitingReaders;
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

extern osMutexId_t dictTrieLock;
