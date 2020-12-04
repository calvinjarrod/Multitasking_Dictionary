#include <stdlib.h>
#include "pkt_queue.h"

// call this function to initialize the packet queue
void Pkt_Queue_Init(Pkt_Queue * Pkt_buffer) {
	Pkt_buffer->capacity = PKT_QUEUE_SIZE;
	Pkt_buffer->size = 0;
	Pkt_buffer->front = 0;
	Pkt_buffer->rear = 0;
}

int Pkt_Queue_Push(Pkt_Queue * Pkt_buffer,UART_Pkt * newPacket) {
	if (Pkt_buffer->capacity == Pkt_buffer->size) return -1; // -1 means the
															 // Pkt_buffer is full
	else if ((Pkt_buffer->size > Pkt_buffer->capacity) ||
		((Pkt_buffer->size == 0 || Pkt_buffer->size == 1) !=
		(Pkt_buffer->front == Pkt_buffer->rear)))
		return -2; // -2 means there's a Pkt_buffer error
	else {
		if (Pkt_buffer->size == 0) Pkt_buffer->Packets[Pkt_buffer->rear] = newPacket;
		else Pkt_buffer->Packets[(Pkt_buffer->rear + 1) % Pkt_buffer->capacity] = newPacket;
		Pkt_buffer->size++;
		if (Pkt_buffer->size == 1) return 0;
		Pkt_buffer->rear++;
		if (Pkt_buffer->rear == Pkt_buffer->capacity) Pkt_buffer->rear = 0;
	}
	return 0; // 0 means correct funcitoning of Pkt_buffer and packet was properly
	 		  // pushed into it
}

UART_Pkt * Pkt_Queue_Pop(Pkt_Queue * Pkt_buffer) {
	UART_Pkt * temp;
	if (Pkt_buffer->size == 0) {
		temp = (UART_Pkt *)malloc(sizeof(UART_Pkt)*2);
		temp->error = -1; // -1 in first byte of packet struct data members means
											// Pkt_buffer is empty
		return temp;
	} else if (
		(Pkt_buffer->size > Pkt_buffer->capacity) ||
		((Pkt_buffer->size == 0 || Pkt_buffer->size == 1) !=
		(Pkt_buffer->front == Pkt_buffer->rear))) {

		temp = (UART_Pkt *)malloc(sizeof(UART_Pkt)*2);
		temp->error = -2; // -2 means error with Queue
		return temp;
	}	else {
		Pkt_buffer->size--;
		// if front and rear are already the same just return
		if (Pkt_buffer->size == 0) return Pkt_buffer->Packets[Pkt_buffer->front];
		Pkt_buffer->front++;
		if (Pkt_buffer->front == Pkt_buffer->capacity) {
			Pkt_buffer->front = 0;
			return Pkt_buffer->Packets[Pkt_buffer->capacity - 1];
		}
		return Pkt_buffer->Packets[Pkt_buffer->front - 1];
	}
	
}
