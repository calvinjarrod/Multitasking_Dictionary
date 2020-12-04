#include <stdlib.h>
#include "cmd_queue.h"

void Dict_CMD_Queue_Init(Dict_CMD_Queue * buffer) {
	buffer->capacity = CMD_QUEUE_SIZE;
	buffer->size = 0;
	buffer->front = 0;
	buffer->rear = 0;
}

int Dict_CMD_Queue_Push(Dict_CMD_Queue * buffer,Dict_CMD * newCmd) {
	osMutexWait(cmdQueueLock,osWaitForever);
	if (buffer->capacity == buffer->size) return -1; // -1 means the buffer is full
	else if ((buffer->size > buffer->capacity) ||
		((buffer->size == 0 || buffer->size == 1) !=
		(buffer->front == buffer->rear))) {
		osMutexRelease(cmdQueueLock);
		return -2; // -2 means there's a buffer error
	} else {
		if (buffer->size == 0) buffer->Dictionary_Commands[buffer->rear] = newCmd;
		else buffer->Dictionary_Commands[(buffer->rear + 1) % buffer->capacity] = newCmd;
		buffer->size++;
		if (buffer->size == 1) {
			osMutexRelease(cmdQueueLock);
			return 0;
		}
		buffer->rear++;
		if (buffer->rear == buffer->capacity) buffer->rear = 0;
	}
	osMutexRelease(cmdQueueLock);
	return 0; // 0 means correct funcitoning of buffer and H_CMD was properly
						// pushed into it
}

Dict_CMD * Dict_CMD_Queue_Pop(Dict_CMD_Queue * buffer) {
	osMutexWait(cmdQueueLock,osWaitForever);
		Dict_CMD * temp;
	if (buffer->size == 0) {
		// empty buffer
		temp = (Dict_CMD*)malloc(sizeof(Dict_CMD));
		temp->instruction = EMPTY;  
		osMutexRelease(cmdQueueLock);
		return temp;
	} else if ((buffer->size > buffer->capacity) ||
		((buffer->size == 0 || buffer->size == 1) !=
		(buffer->front == buffer->rear))) {
		// buffer error
		temp = (Dict_CMD*)malloc(sizeof(Dict_CMD));
		temp->instruction = ERR;  
		osMutexRelease(cmdQueueLock);
		return temp;
	}	else {
		buffer->size--;
		// if front and rear are already the same just return
		if (buffer->size == 0) {
			osMutexRelease(cmdQueueLock);
			return buffer->Dictionary_Commands[buffer->front];
		}
		buffer->front++;
		if (buffer->front == buffer->capacity) buffer->front = 0;
		osMutexRelease(cmdQueueLock);
		return buffer->Dictionary_Commands[buffer->front - 1];
	}
}
