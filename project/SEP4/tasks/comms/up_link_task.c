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

void init_task_uplink(){
	printf("Uplink init\n");
	
}
 
void upLinkTask(void *pvParameters)
{
	init_task_uplink();
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 2000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t ucRxData[ 4 ];
	size_t xReceivedBytes;
	const TickType_t xBlockTime = pdMS_TO_TICKS( 10000 );
	for( ;; )
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		xReceivedBytes = xMessageBufferReceive( getUpLinkMessageBuffer(),
		( void * ) ucRxData,
		sizeof( ucRxData ),
		xBlockTime );
		if( xReceivedBytes > 0 )
		{
			puts("UpLink\n");
		}
		vTaskDelay(10000);
	}
}