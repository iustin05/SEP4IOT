/*
 * packer.h
 *
 * Created: 5/26/2022 1:10:05 AM
 *  Author: nordesk
 */ 


#ifndef PACKER_H_
#define PACKER_H_

#include <lora_driver.h>

int8_t g_temp;
int8_t g_lux;
int8_t g_hum;
int8_t g_co2;

lora_driver_payload_t loraPayload;

lora_driver_payload_t getSendReadyPayload();


#endif /* PACKER_H_ */