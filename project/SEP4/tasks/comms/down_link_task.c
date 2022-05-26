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

 void init_task(){
 
 }
 
void downLinkTask(void *pvParameters)
{
	init_task();
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(10000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}