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
	uint16_t _tmp;
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl2591_getFullSpectrumRaw(&_tmp)) )
		{
			//printf("\nFull Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("\nFull spectrum overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl259_getVisibleRaw(&_tmp)) )
		{
			//printf("Visible Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Visible overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl2591_getInfraredRaw(&_tmp)) )
		{
			//printf("Infrared Raw:%04X\n", _tmp);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Infrared overflow - change gain and integration time\n");
		}
		
		if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
		{
			last_lux = (int)(_lux * 10);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}
		break;
		
		case TSL2591_OK:
			
		break;
		
		case TSL2591_DEV_ID_READY:
		
		break;
		
		default:
		break;
	}
	printf("[LUX] Result: %d", last_lux);
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
	printf("[LUX] Lux init");
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 21000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	EventBits_t eventBits;
	for( ;; )
	{
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_LUX,
		pdTRUE,
		pdTRUE,
		20000/portTICK_PERIOD_MS);
		if(eventBits & BIT_MEASURE_LUX){
			vTaskDelay(2000);
			printf("[LUX] Measure request received.\nTaking measurement...\n");
			if ( TSL2591_OK != tsl2591_fetchData() )
			{
				// Something went wrong
				// Investigate the return code further
			}
			else
			{
				// The light data will be ready after the driver calls the call back function with TSL2591_DATA_READY.
			}
		} else {
			printf("[W][LUX] Measure listener timed out. Not taking measurement...\n");
		}
		
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}