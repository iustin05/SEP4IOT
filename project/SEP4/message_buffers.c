/*
 * message_buffers.c
 *
 * Created: 5/26/2022 12:16:18 PM
 *  Author: nordesk
 */ 
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <message_buffer.h>

#include <lora_driver.h>

MessageBufferHandle_t upLinkMessageBuffer;
MessageBufferHandle_t downLinkMessageBuffer;

MessageBufferHandle_t getUpLinkMessageBuffer(){
	return upLinkMessageBuffer;
}

MessageBufferHandle_t getDownLinkMessageBuffer(){
	return downLinkMessageBuffer;
}

void initMessageBuffers(){
	upLinkMessageBuffer = xMessageBufferCreate( sizeof(lora_driver_payload_t) + sizeof(size_t) );
	downLinkMessageBuffer = xMessageBufferCreate( sizeof(lora_driver_payload_t) + sizeof(size_t) );
	
	if(upLinkMessageBuffer==NULL){
		printf("uBuffer failed\n");
	} else {
		printf("uBuff OK\n");
	}
}