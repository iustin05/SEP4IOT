/*
 * app_task.c
 *
 * Created: 5/26/2022 12:47:36 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <init_sensors.h>

#include <stdio_driver.h>
#include <packer.h>
#include <status_leds.h>
#include <serial.h>

#include <message_buffer.h>
#include <message_buffers.h>

#include <xevent_groups.h>

void initLED()
{
	// Initiating the LEDs
	DDRA |= _BV(DDA0) | _BV(DDA7);
	status_leds_initialise(5);
}

void appTask(void *pvParameters)
{
	EventBits_t uxBits;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20000 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;
	size_t xBytesSent;
	uint8_t ucArrayToSend[] = { 0, 1, 2, 3 };
	const TickType_t x100ms = pdMS_TO_TICKS( 100 );
    for( ;; )
    {
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		puts("APP Start CO2...\n");
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_CO2);
		uxBits = xEventGroupWaitBits(
				getDataReadyEventGroup(),
				BIT_READY_CO2,
				pdTRUE,
				pdFALSE,    
				xTicksToWait );
		if(uxBits && BIT_READY_CO2){
			puts("APP CO2 measured\n");
		}
		puts("APP Start T/H...\n");
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_HUM_TEMP);
		uxBits = xEventGroupWaitBits(
		getDataReadyEventGroup(),
		BIT_READY_HUM_TEMP,
		pdTRUE,
		pdFALSE,
		xTicksToWait );
		if(uxBits && BIT_READY_HUM_TEMP){
			puts("T/H measured\n");
		}
		puts("APP Start LUX...\n");
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_LUX);
		uxBits = xEventGroupWaitBits(
		getDataReadyEventGroup(),
		BIT_READY_LUX,
		pdTRUE,
		pdFALSE,
		xTicksToWait );
		if(uxBits && BIT_READY_LUX){
			puts("LUX measured\n");
		}
		puts("APP All measuring completed\n");
		
		
    }
}

