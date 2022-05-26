/*
 * temp_hum_task.c
 *
 * Created: 5/26/2022 1:55:41 AM
 *  Author: nordesk
 */ 
#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include <stdio_driver.h>
#include <serial.h>
#include <hih8120.h>

#include <xevent_groups.h>

uint16_t last_temp;
uint16_t last_hum;

uint16_t getTemp(){
	return last_temp;
}

uint16_t getHum(){
	return last_hum;
}

void initTempHum(){
	if ( HIH8120_OK == hih8120_initialise() )
	{
		puts("TEMP/HUM SENSOR INITIALIZED\n");
	}
	
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		puts("TEMP/HUM SENSOR WAKE UP WRONG\n");

		// Something went wrong
		// Investigate the return code further
	}
	puts("TEMP/HUM SENSOR WAKE UP COMPLETE\n");
}

void tempHumTask(void *pvParameters)
{
	EventBits_t eventBits;
	for(;;){
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_HUM_TEMP,
		pdTRUE,
		pdTRUE,
		10000/portTICK_PERIOD_MS);
		if(eventBits & BIT_MEASURE_HUM_TEMP){
			vTaskDelay(500);
			printf("[HUM/TEMP] Measure request received.\nTaking measurement...\n");
			if ( HIH8120_OK !=  hih8120_measure() ){
				puts("[E][TEMP/HUM] SENSOR MEASURE WRONG\n");
			}
			vTaskDelay(10); // Delay for TWI
			last_temp = hih8120_getTemperature_x10();
			last_hum = hih8120_getHumidity();
			
			printf("[HUM/TEMP] Results T: %d H: %d\n", last_temp, last_hum);
			
			printf("[HUM/TEMP] Going to sleep for interval.\n");
		} else {
			printf("[W][HUM/TEMP] Measure listener timed out. Taking no measurement...\n");
		}
	}
}