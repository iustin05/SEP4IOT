/*
 * up_link_task.c
 *
 * Created: 5/26/2022 1:53:46 AM
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
#include <message_buffers.h>

#include <lora_driver.h>

void init_task_uplink(){
	printf("Uplink OK\n");
	
}
 
void upLinkTask(void *pvParameters)
{
	init_task_uplink();
	lora_driver_payload_t upLinkPayload;
	size_t xReceivedBytes;
	for( ;; )
	{
		vTaskDelay(1000/portTICK_PERIOD_MS);
		xReceivedBytes = xMessageBufferReceive( getUpLinkMessageBuffer(),
		&upLinkPayload,
		sizeof( lora_driver_payload_t ),
		10000/portTICK_PERIOD_MS);
		if( xReceivedBytes == sizeof(lora_driver_payload_t) )
		{
			puts("UpLink START\n");
			// printf("H: 0x%08x 0x%08x\n", upLinkPayload.bytes[0], upLinkPayload.bytes[1]);
			vTaskDelay(50);
			printf("Upload Message > %s <\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &upLinkPayload)));
		}
	}
}