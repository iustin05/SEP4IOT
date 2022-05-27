/*
 * packer.c
 *
 * Created: 5/26/2022 1:09:37 AM
 *  Author: nordesk
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <string.h>

#include <stdio_driver.h>
#include <serial.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>

#include <lora_driver.h>
#include <lora_driver_utils.h>

int8_t g_temp;
int8_t g_lux;
int8_t g_hum;
int8_t g_co2;

lora_driver_payload_t loraPayload;

void setTEMP(int8_t temp){
	g_temp = temp;
}

void setLUX(int8_t lux){
	g_lux = lux;
}

void setHUM(int8_t hum){
	g_hum = hum;
}

void setCO2(int8_t co2){
	g_co2 = co2;
}

lora_driver_payload_t getSendReadyPayload(){
	
	loraPayload.len = 8;
	loraPayload.portNo = 2;
	
	loraPayload.bytes[0] = g_hum >> 8;
	loraPayload.bytes[1] = g_hum & 0xFF;
	loraPayload.bytes[2] = g_temp >> 8;
	loraPayload.bytes[3] = g_temp & 0xFF;
	loraPayload.bytes[4] = g_co2 >> 8;
	loraPayload.bytes[5] = g_co2 & 0xFF;
	loraPayload.bytes[6] = g_lux >> 8;
	loraPayload.bytes[7] = g_lux & 0xFF;
	
	return loraPayload; 
}





