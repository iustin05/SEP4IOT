/*
 * comm_queue.c
 *
 * Created: 5/27/2022 10:21:13 AM
 *  Author: nordesk
 */ 
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>
#include <queue.h>

#define PACKET_TYPE_CO2 1
#define PACKET_TYPE_HUM 2
#define PACKET_TYPE_TMP 3
#define PACKET_TYPE_LUX 4
#define PACKET_TYPE_NULL 5

QueueHandle_t commQUE;

typedef struct qPacketType{
	short value;
	short type;
} qPacketType_t;


void createQueue(){
	commQUE = xQueueCreate( 4, sizeof( qPacketType_t ) );
	if(commQUE != 0){
		printf("QUE OK\n");
	}
}

void sendCommQueue(qPacketType_t queuePacket){
	if(xQueueSend( commQUE,
	( void * ) &queuePacket,
	( TickType_t ) 100 ) != pdPASS )
	{
		printf("QUE sent\n");
	}
}

qPacketType_t receiveCommQueue(){
	qPacketType_t receivedQueuePacket;
	if( xQueueReceive( commQUE,
	&( receivedQueuePacket ),
	( TickType_t ) 10 ) == pdPASS )
	{
	} else {
		receivedQueuePacket.type = PACKET_TYPE_NULL;
	}
	return receivedQueuePacket;
}
