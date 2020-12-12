#include "cmsis_os.h"
#include "pkt_queue.h"

#define CMD_QUEUE_SIZE 20

typedef enum INSTRUCT {
	INSERT,DELETE,CHECK,EMPTY,ERR
}INSTRUCT;

typedef struct Dict_CMD {
	INSTRUCT instruction;
	uint8_t word[MAX_WORD_LENGTH+CMD_LENGTH];
	uint8_t wordSize;
} Dict_CMD;

typedef struct Dict_CMD_Queue {
	uint8_t capacity;
	uint8_t size;
	uint8_t front;
	uint8_t rear;
	
	// buffer of dictionary commands
	Dict_CMD * Dictionary_Commands[CMD_QUEUE_SIZE];
	
} Dict_CMD_Queue;

void Dict_CMD_Queue_Init(Dict_CMD_Queue * buffer);
int Dict_CMD_Queue_Push(Dict_CMD_Queue * buffer, Dict_CMD * newCmd);
Dict_CMD * Dict_CMD_Queue_Pop(Dict_CMD_Queue * buffer);

extern osMutexId cmdQueueLock;
extern osThreadId DictionaryManagerHandle;
extern osThreadId defaultTaskHandle;
