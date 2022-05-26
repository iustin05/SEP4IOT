/*
 * co2_task.c
 *
 * Created: 5/26/2022 1:55:07 AM
 *  Author: nordesk
 */
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <mh_z19.h>
uint16_t last_ppm;
mh_z19_returnCode_t rc;

void mh_z19CallBack(uint16_t ppm){
	last_ppm = ppm;
}

void setCO2Callback() {
	mh_z19_initialise(ser_USART3); 
	mh_z19_injectCallBack(mh_z19CallBack);
}
 
void CO2Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		
		rc = mh_z19_takeMeassuring();
		if (rc != MHZ19_OK)
		{
			// Something went wrong
		}
		printf("CO2: %d", last_ppm);
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}