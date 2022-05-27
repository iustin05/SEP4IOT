/*
 * comm_queue.h
 *
 * Created: 5/27/2022 10:21:01 AM
 *  Author: nordesk
 */ 


#ifndef COMM_QUEUE_H_
#define COMM_QUEUE_H_

#define PACKET_TYPE_CO2 1
#define PACKET_TYPE_HUM 2
#define PACKET_TYPE_TMP 3
#define PACKET_TYPE_LUX 4
#define PACKET_TYPE_NULL 5

typedef struct qPacketType{
	short value;
	short type;	
} qPacketType_t;


extern void createQueue();

extern void sendCommQueue(qPacketType_t queuePacket);

extern qPacketType_t receiveCommQueue();


#endif /* COMM_QUEUE_H_ */