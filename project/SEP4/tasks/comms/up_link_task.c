/*
 * up_link_task.c
 * 
 * Created: 5/26/2022 1:53:46 AM
 *  Author: nordesk
 */
#include <iot_io.h>

#include <program_config.h>

#include <ATMEGA_FreeRTOS.h>

#include <message_buffers.h>

#include <status_leds.h>

#include <display_7seg.h>

#include <lora_driver.h>

void init_task_uplink(){
	printf("Up OK\n");
}
 
void upLinkTask(void *pvParameters)
{
	init_task_uplink();
	
	lora_driver_payload_t upLinkPayload;
	size_t xReceivedBytes;
	for( ;; )
	{
		xReceivedBytes = xMessageBufferReceive( getUpLinkMessageBuffer(),
		&upLinkPayload,
		sizeof( lora_driver_payload_t ),
		SETTING_TIMEOUT_UPLINK/portTICK_PERIOD_MS);
		if( xReceivedBytes == sizeof(lora_driver_payload_t) )
		{
			display_7seg_displayHex("B100");
			status_leds_fastBlink(led_ST3);
			#ifndef DEBUG_NONETWORK
				puts("UpLink..\n");
				printf("Upload %s\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &upLinkPayload)));
				// vTaskDelay(50);
			#endif
			// printf("H: 0x%08x 0x%08x\n", upLinkPayload.bytes[0], upLinkPayload.bytes[1]);
			display_7seg_displayHex("9000");
			status_leds_ledOff(led_ST3);
		}
	}
}