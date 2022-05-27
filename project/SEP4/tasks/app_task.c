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

#include <comm_queue.h>

#include <packer.h>

#define xFrequency (20000 / portTICK_PERIOD_MS)

void initLED()
{
	// Initiating the LEDs
	DDRA |= _BV(DDA0) | _BV(DDA7);
	status_leds_initialise(5);
}

void connectWAN(){
	// TODO CONNECT TO LORAWAN
}

void appTask(void *pvParameters)
{
	EventBits_t uxBits;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	qPacketType_t recievePacket;
	lora_driver_payload_t upLinkPayload;
	const TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;
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
					printf("pRCV err\n");
					break;
				}
			}
		} while(recievePacket.type != PACKET_TYPE_NULL);
		
		printf("APP received queues done\n");
		
		upLinkPayload = getSendReadyPayload();
		
		size_t xBytesSent;
		xBytesSent = xMessageBufferSend(getUpLinkMessageBuffer(), &upLinkPayload, sizeof(upLinkPayload), 100);
		if( xBytesSent != sizeof( upLinkPayload ) )
		{
			printf("upbuffer - no heap space\n");
		}
		
		
    }
}

