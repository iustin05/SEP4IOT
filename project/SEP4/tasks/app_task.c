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

#include <xevent_groups.h>

void initLED()
{
	// Initiating the LEDs
	DDRA |= _BV(DDA0) | _BV(DDA7);
	status_leds_initialise(5);
}

void appTask(void *pvParameters)
{
	//initLED();
	//initSensorsAndEvents();
	
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
    for( ;; )
    {
			
			puts("[APP] Time-based checking start ->\n");
			puts("[APP] Sending measurement start event bits...\n");
			xTaskDelayUntil( &xLastWakeTime, xFrequency );
			xEventGroupSetBits(
			getMeasureEventGroup(),
			BIT_MEASURE_ALL );
			
			
    }
}

