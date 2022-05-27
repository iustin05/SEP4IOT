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

 void init_task(){
 
 }
 
void downLinkTask(void *pvParameters)
{
	
	init_task();
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 2000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("Downloading\n");
	}
}

void downLink(){
	
}