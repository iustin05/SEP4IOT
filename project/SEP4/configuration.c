#include <iot_io.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

/*
 * configuration.c
 *
 * Created: 5/26/2022 12:45:43 AM
 *  Author: nordesk
 */

int16_t servoAngle; // -100 -> +100 angle to be used by the servo
SemaphoreHandle_t configMutex;

void setServoAngle(int16_t angle){
	servoAngle = angle;
}

int16_t getServoAngle(void){
	return servoAngle;
}

void initConfiguration(){
	configMutex = xSemaphoreCreateMutex();
	if(configMutex != NULL){
		printf("Mutex OK\n");
	} else {
		printf("Mutex failed!\n");
	}
}