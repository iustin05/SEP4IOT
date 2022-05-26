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

void init_task_uplink(){
	
}
 
void upLinkTask(void *pvParameters)
{
	init_task_uplink();
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(10000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		
		puts("UpLInk");
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}