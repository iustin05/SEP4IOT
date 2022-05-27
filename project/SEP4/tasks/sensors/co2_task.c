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

#include <event_groups.h>
#include <xevent_groups.h>

#include <stdio_driver.h>
#include <mh_z19.h>

#include <comm_queue.h>

uint16_t last_ppm;
mh_z19_returnCode_t rc;
short ready;

void mh_z19CallBack(uint16_t ppm){
	last_ppm = ppm;
	ready = 1;
}

void setCO2Callback() {
	puts("CO2 OK");
	mh_z19_initialise(ser_USART3); 
	mh_z19_injectCallBack(mh_z19CallBack);
}
 
void CO2Task(void *pvParameters)
{
	EventBits_t eventBits;
	TickType_t xLastWakeTime;
	qPacketType_t sensorPacket;
	const TickType_t xFrequency = 2000/portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		ready = 0;
		xTaskDelayUntil( &xLastWakeTime, xFrequency );
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_CO2,
		pdTRUE,
		pdTRUE,
		20000/portTICK_PERIOD_MS);
		if(eventBits & BIT_MEASURE_CO2){
			printf("CO2 Measuring...\n");
			rc = mh_z19_takeMeassuring();
			if (rc != MHZ19_OK)
			{
				puts("E CO2 Failed\n");
			}
			while(!ready){
				vTaskDelay(50);
			}
			sensorPacket.type = PACKET_TYPE_CO2;
			sensorPacket.value = last_ppm;
			sendCommQueue(sensorPacket);
			vTaskDelay(50);
			printf("CO2: %d\n", last_ppm);
		} else {
			printf("W CO2 Timeout\n");
		}
	}
}