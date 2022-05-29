/*
 * configuration.h
 *
 * Created: 5/26/2022 12:59:56 AM
 *  Author: nordesk
 */ 


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <semphr.h>

SemaphoreHandle_t configMutex;

extern void setServoAngle(int16_t angle);
extern int16_t getServoAngle(void);

#endif /* CONFIGURATION_H_ */