/*
 * lux_task.c
 *
 * Created: 5/26/2022 1:55:21 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <tsl2591.h>

#include <xevent_groups.h>

uint16_t last_lux;

void tsl2591Callback(tsl2591_returnCode_t rc){
	float _lux;
	if(rc == TSL2591_DATA_READY){
		tsl2591_getLux(&_lux);
		printf("Lux: %d\n", _lux);
	}
	last_lux = _lux;
	
}

void setLuxCallback(){
	if ( TSL2591_OK == tsl2591_initialise(tsl2591Callback) )
	{
		// Driver initilised OK
		// Always check what tsl2591_initialise() returns
	}
}

void init_task_lux(){
	if ( TSL2591_OK == tsl2591_enable() )
	{
		// The power up command is now send to the sensor - it can be powered down with a call to tsl2591_disable()
	}
}

void luxTask(void *pvParameters)
{
	init_task_lux();
	EventBits_t eventBits;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 2000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_LUX,
		pdTRUE,
		pdTRUE,
		20000/portTICK_PERIOD_MS);
		if(eventBits & BIT_MEASURE_LUX){
			printf("Lux measure\n");
			if ( TSL2591_OK != tsl2591_fetchData() )
			{
				// Something went wrong
				// Investigate the return code further
			}
			else
			{
				// The light data will be ready after the driver calls the call back function with TSL2591_DATA_READY.
			}
		}
	}
}