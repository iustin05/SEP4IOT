/*
 * co2_task.c
 *
 * Created: 5/26/2022 1:55:07 AM
 *  Author: nordesk
 */
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <program_config.h>

#include <xevent_groups.h>

#include <mh_z19.h>

#include <leds_numbers_tasks.h>

#include <comm_queue.h>

uint16_t last_ppm;
mh_z19_returnCode_t rc;
short ready;

void mh_z19CallBack(uint16_t ppm){
	last_ppm = ppm;
	ready = 1;
}

void setCO2Callback() {
	puts("CO2 OK\n");
	mh_z19_initialise(ser_USART3); 
	mh_z19_injectCallBack(mh_z19CallBack);
}
 
void CO2Task(void *pvParameters)
{
	EventBits_t eventBits;
	qPacketType_t sensorPacket;
        mh_z19_takeMeassuring();
	for(;;){
		ready = 0;
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_CO2,
		pdTRUE,
		pdTRUE,
		SETTING_TIMEOUT_CO2/portTICK_PERIOD_MS);
		ledON(LED_CO2_TASK);
		// display_7seg_displayHex("C02");
		if(eventBits & BIT_MEASURE_CO2){
			printf("CO2 Measuring...\n");
			rc = mh_z19_takeMeassuring();
			if (rc != MHZ19_OK)
			{
				puts("E CO2 Failed\n");
			}
			while(!ready){ // wait for measurement to be ready
				vTaskDelay(50);
			}
			sensorPacket.type = PACKET_TYPE_CO2;
			sensorPacket.value = last_ppm;
			sendCommQueue(sensorPacket); // add the result to the queue
			#ifdef DEBUG_EXTRA_DATA
			printf("CO2: %d\n", last_ppm);
			#endif
		}
		ledOFF(LED_CO2_TASK);
	}
}
