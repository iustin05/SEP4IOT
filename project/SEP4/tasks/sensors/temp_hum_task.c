/*
 * temp_hum_task.c
 *
 * Created: 5/26/2022 1:55:41 AM
 *  Author: nordesk
 */ 
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <hih8120.h>

#include <xevent_groups.h>

#include <program_config.h>

#include <leds_numbers_tasks.h>

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
		puts("T/H OK\n");	
	}
	if ( HIH8120_OK != hih8120_wakeup() )
	{
		puts("T/H WRONG\n");
	}
}

void tempHumTask(void *pvParameters)
{
	EventBits_t eventBits;
	qPacketType_t sensorPacket;
	for(;;){
		eventBits = xEventGroupWaitBits(getMeasureEventGroup(),
		BIT_MEASURE_HUM_TEMP,
		pdTRUE,
		pdTRUE,
		SETTING_TIMEOUT_TH/portTICK_PERIOD_MS);
		ledON(LED_THF_TASK);
		//display_7seg_displayHex("F 7");
		if(eventBits & BIT_MEASURE_HUM_TEMP){
			printf("T/H measuring...\n");
			if ( HIH8120_OK !=  hih8120_measure() ){
				puts("T/H m. wrong!\n");
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
			#ifdef DEBUG_EXTRA_DATA
			printf("T: %d H: %d\n", last_temp, last_hum);
			#endif
		}
		ledOFF(LED_THF_TASK);
	}
}