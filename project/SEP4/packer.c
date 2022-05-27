/*
 * packer.c
 *
 * Created: 5/26/2022 1:09:37 AM
 *  Author: nordesk
 */ 

#include <stdio.h>
#include <avr/io.h>

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
	
}





