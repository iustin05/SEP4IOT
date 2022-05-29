/*
 * down_link_task.c
 *
 * Created: 5/26/2022 1:54:41 AM
 *  Author: nordesk
 */
#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>

#include <program_config.h>

#include <xevent_groups.h>

#include <display_7seg.h>

#include <lora_driver.h>

#include <message_buffers.h>

#include <status_leds.h>

#include <configuration.h>

#define PENDING 1
#define CLEAR 0

int16_t last_cfg;
short newConfig;

void init_task(){
	printf("DL OK\n");
}
 
void downLinkTask(void *pvParameters)
{
	init_task();
	lora_driver_payload_t downLinkPayload;
	size_t xReceivedBytes;
	for( ;; )
	{
		//printf("DL Check\n");
		xReceivedBytes = xMessageBufferReceive( getDownLinkMessageBuffer(),
		&downLinkPayload,
		sizeof( lora_driver_payload_t ),
		SETTING_TIMEOUT_DOWNLINK/portTICK_PERIOD_MS);
		taskENTER_CRITICAL();
		if( xReceivedBytes == sizeof(lora_driver_payload_t) )
		{
			status_leds_fastBlink(led_ST4);
			status_leds_longPuls(led_ST2);
			display_7seg_displayHex("C100");
			puts("DL ...\n");
			last_cfg = downLinkPayload.bytes[0];
			newConfig = PENDING; // trigger write into configuration
			#ifdef DEBUG_EXTRA_DATA
			printf("%d", downLinkPayload.len);
			for(int i = 0; i<downLinkPayload.len; i++){
				printf("%d\n", downLinkPayload.bytes[i]);
			}
			printf("DL val : %d\n", (int) last_cfg);
			#endif
		}
		if(newConfig == PENDING){
			if( configMutex != NULL )
			{
				if( xSemaphoreTake( configMutex, ( TickType_t ) 500 ) == pdTRUE )
				{
					setServoAngle((int)last_cfg - 100);
					xSemaphoreGive( configMutex );
					#ifdef DEBUG_EXTRA_DATA
					printf("DL - new servo value fetched\n");
					#endif
					xEventGroupSetBits(getMeasureEventGroup(), BIT_SERVO);
					xEventGroupWaitBits(
					getDataReadyEventGroup(),
					BIT_READY_HUM_TEMP,
					pdTRUE,
					pdFALSE,
					1000 / portTICK_PERIOD_MS);
					vTaskDelay(1000/portTICK_PERIOD_MS);
					display_7seg_displayHex("C100");
					status_leds_ledOff(led_ST4);
					status_leds_ledOn(led_ST2);
				}
				else // not able to get the semaphore
				{
					#ifdef DEBUG_EXTRA_DATA
					printf("DL error setting value\n");
					#endif
					display_7seg_displayErr();
					status_leds_ledOff(led_ST4);
				}
			}
			newConfig = CLEAR;
			display_7seg_displayHex("9001");
		}
		taskEXIT_CRITICAL();
	}
}