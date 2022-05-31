/*
 * packer.h
 *
 * Created: 5/26/2022 1:10:05 AM
 *  Author: nordesk
 */ 


#ifndef PACKER_H_
#define PACKER_H_

#include <lora_driver.h>

int16_t g_temp;
int16_t g_lux;
int16_t g_hum;
int16_t g_co2;

extern lora_driver_payload_t loraPayload;

extern lora_driver_payload_t getSendReadyPayload();


#endif /* PACKER_H_ */