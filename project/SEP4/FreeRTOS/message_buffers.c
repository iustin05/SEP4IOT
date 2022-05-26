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

#include <message_buffer.h>

MessageBufferHandle_t upLinkMessageBuffer;
MessageBufferHandle_t downLinkMessageBuffer;

void initMessageBuffers(){
	upLinkMessageBuffer = xMessageBufferCreate( 10 );
	downLinkMessageBuffer = xMessageBufferCreate( 4 );
}