/*
 * down_link_task.c
 *
 * Created: 5/26/2022 1:54:41 AM
 *  Author: nordesk
 */
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>

#include <lora_driver.h>
#include <message_buffer.h>
#include <message_buffers.h>

#include <configuration.h>

#define PENDING 1
#define CLEAR 0

int8_t last_cfg;
short newConfig;

void init_task(){
	
}
 
void downLinkTask(void *pvParameters)
{
	init_task();
	lora_driver_payload_t downLinkPayload;
	size_t xReceivedBytes;
	for( ;; )
	{
		vTaskDelay(1000/portTICK_PERIOD_MS);
		xReceivedBytes = xMessageBufferReceive( getDownLinkMessageBuffer(),
		&downLinkPayload,
		sizeof( lora_driver_payload_t ),
		10000/portTICK_PERIOD_MS);
		if( xReceivedBytes == sizeof(lora_driver_payload_t) )
		{
			puts("Downloading...\n");
			last_cfg = downLinkPayload.bytes[0];
			newConfig = PENDING;
			printf("%d", last_cfg);
		}
		if(newConfig == PENDING){
			vTaskDelay(10000/portTICK_PERIOD_MS);
			if( configMutex != NULL )
			{
				/* See if we can obtain the semaphore.  If the semaphore is not
				available wait 10 ticks to see if it becomes free. */
				if( xSemaphoreTake( configMutex, ( TickType_t ) 20 ) == pdTRUE )
				{
					/* We were able to obtain the semaphore and can now access the
					shared resource. */
					
					setServoAngle(last_cfg);
					/* We have finished accessing the shared resource.  Release the
					semaphore. */
					xSemaphoreGive( configMutex );
					printf("DL Servo value updated by remote\n");
				
				}
				else
				{
					printf("DL error setting config\n");
					/* We could not obtain the semaphore and can therefore not access
					the shared resource safely. */
				}
			}
			newConfig = CLEAR;
		}
	}
}

void downLink(){
	
}