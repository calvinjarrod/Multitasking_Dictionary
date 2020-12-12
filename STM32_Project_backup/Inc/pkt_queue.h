#include <stdint.h>

#define PKT_QUEUE_SIZE 20
#define MAX_WORD_LENGTH 10
#define CMD_LENGTH 1

typedef struct UART_Pkt {
	uint8_t data[MAX_WORD_LENGTH+CMD_LENGTH];
	uint8_t dataSize;
	int8_t error;
}UART_Pkt;

typedef struct Pkt_Queue {
	uint8_t capacity;
	uint8_t size;
	uint8_t front;
	uint8_t rear;
	UART_Pkt * Packets[PKT_QUEUE_SIZE];
} Pkt_Queue;

void Pkt_Queue_Init(Pkt_Queue * Pkt_buffer);
int Pkt_Queue_Push(Pkt_Queue * Pkt_buffer,UART_Pkt * newPacket);
UART_Pkt * Pkt_Queue_Pop(Pkt_Queue * Pkt_buffer);

