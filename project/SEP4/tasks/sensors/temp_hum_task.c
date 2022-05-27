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

#include <comm_queue.h>


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
		
	}
	
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		puts("TEMP/HUM SENSOR WAKE UP WRONG\n");

		// Something went wrong
		// Investigate the return code further
	}
}

void tempHumTask(void *pvParameters)
{
	EventBits_t eventBits;
	TickType_t xLastWakeTime;
	qPacketType_t sensorPacket;
	const TickType_t xFrequency = 2000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_HUM_TEMP,
		pdTRUE,
		pdTRUE,
		10000/portTICK_PERIOD_MS);
		if(eventBits & BIT_MEASURE_HUM_TEMP){
			printf("T/H measuring...\n");
			if ( HIH8120_OK !=  hih8120_measure() ){
				puts("T/H measure wrong!\n");
			}
			vTaskDelay(20); // Delay for TWI
			last_temp = hih8120_getTemperature_x10();
			sensorPacket.type = PACKET_TYPE_TMP;
			sensorPacket.value = last_temp;
			sendCommQueue(sensorPacket);
			vTaskDelay(50);
			last_hum = hih8120_getHumidity();
			sensorPacket.type = PACKET_TYPE_HUM;
			sensorPacket.value = last_hum;
			sendCommQueue(sensorPacket);
			vTaskDelay(50);
			printf("T: %d H: %d\n", last_temp, last_hum);
		}
	}
}