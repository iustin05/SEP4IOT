/*
 * app_task.c
 *
 * Created: 5/26/2022 12:47:36 AM
 *  Author: nordesk
 */ 
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <task.h>

#include <program_config.h>

#include <status_leds.h>

#include <message_buffers.h>

#include <xevent_groups.h>

#include <comm_queue.h>

#include <packer.h>

#include <connect_lwan.h>

#include <leds_numbers_tasks.h>

#include <display_7seg.h>

void connectWAN(){
	status_leds_ledOn(led_ST1);
	initLORAWAN();
}

void appTask(void *pvParameters)
{
	connectWAN();
	EventBits_t uxBits;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	qPacketType_t recievePacket;
	lora_driver_payload_t upLinkPayload;
	
    for( ;; )
    {
		display_7seg_displayHex("A100");
		ledON(LED_APP_TASK_WORK);
		ledON(LED_APP_TASK);
		#ifdef DEBUG_EXTRA_DATA
		puts("APP Start CO2...\n");
		#endif
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_CO2);
		uxBits = xEventGroupWaitBits(
				getDataReadyEventGroup(),
				BIT_READY_CO2,
				pdTRUE,
				pdFALSE,    
				SETTING_TIMEOUT_WAIT_READY_BITS / portTICK_PERIOD_MS );
		if(uxBits && BIT_READY_CO2){
			puts("APP CO2 measured\n");
		}
		#ifdef DEBUG_EXTRA_DATA
		puts("APP Start T/H...\n");
		#endif
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_HUM_TEMP);
		uxBits = xEventGroupWaitBits(
		getDataReadyEventGroup(),
		BIT_READY_HUM_TEMP,
		pdTRUE,
		pdFALSE,
		SETTING_TIMEOUT_WAIT_READY_BITS / portTICK_PERIOD_MS);
		if(uxBits && BIT_READY_HUM_TEMP){
			puts("T/H measured\n");
		}
		#ifdef DEBUG_EXTRA_DATA
		puts("APP Start LUX...\n");
		#endif
		xEventGroupSetBits(getMeasureEventGroup(), BIT_MEASURE_LUX);
		uxBits = xEventGroupWaitBits(
		getDataReadyEventGroup(),
		BIT_READY_LUX,
		pdTRUE,
		pdFALSE,
		SETTING_TIMEOUT_WAIT_READY_BITS / portTICK_PERIOD_MS);
		if(uxBits && BIT_READY_LUX){
			puts("LUX measured\n");
		}
		
		puts("APP All measuring completed\n");
		ledOFF(LED_APP_TASK_WORK);
		display_7seg_displayHex("A200");
		do{
			recievePacket = receiveCommQueue();
			//printf("%d, %d\n", recievePacket.type, recievePacket.value);
			vTaskDelay(100);
			if(recievePacket.type == PACKET_TYPE_NULL) break;
			switch(recievePacket.type){
				case PACKET_TYPE_CO2: {
					g_co2 = recievePacket.value;
					printf("co2 rcv\n");
					break;
				}
				case PACKET_TYPE_HUM: {
					g_hum = recievePacket.value;
					printf("hum rcv\n");
					break;
				}
				case PACKET_TYPE_LUX: {
					g_lux = recievePacket.value;
					printf("lux rcv\n");
					break;
				}
				case PACKET_TYPE_TMP: {
					g_temp = recievePacket.value;
					printf("temp rcv\n");
					break;
				}
				default: {
					printf("RCV err\n");
					break;
				}
			}
		} while(recievePacket.type != PACKET_TYPE_NULL);
		
		printf("APP received queues done\n");
		upLinkPayload = getSendReadyPayload();
		
		vTaskDelay(500);
		
		size_t xBytesSent;
		xBytesSent = xMessageBufferSend(getUpLinkMessageBuffer(), &upLinkPayload, sizeof(upLinkPayload), 1000);
		if( xBytesSent != sizeof( upLinkPayload ) )
		{
			printf("upbuffer - no heap space\n");
		}
		
		#ifdef DEBUG_EXTRA_DATA
		printf("APP data buffer sent\n");
		#endif
		ledOFF(LED_APP_TASK);
		display_7seg_displayHex("9000");
		xTaskDelayUntil( &xLastWakeTime, (SETTING_CYCLE_APP / portTICK_PERIOD_MS) );
    }
}

