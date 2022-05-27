/*
 * message_buffers.c
 *
 * Created: 5/26/2022 12:16:18 PM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <message_buffer.h>
#include <packer.h>

MessageBufferHandle_t upLinkMessageBuffer;
MessageBufferHandle_t downLinkMessageBuffer;

MessageBufferHandle_t getUpLinkMessageBuffer(){
	return upLinkMessageBuffer;
}

MessageBufferHandle_t getDownLinkMessageBuffer(){
	return downLinkMessageBuffer;
}

void initMessageBuffers(){
	upLinkMessageBuffer = xMessageBufferCreate( sizeof(getSendReadyPayload()) + sizeof(size_t) );
	//downLinkMessageBuffer = xMessageBufferCreate( 4 );
	
	if(upLinkMessageBuffer==NULL){
		puts("Buffer failed\n");
	} else {
		puts("Buffer OK\n");
	}
}