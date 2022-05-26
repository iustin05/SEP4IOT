/*
 * app_tasks.h
 *
 * Created: 5/26/2022 1:35:44 AM
 *  Author: nordesk
 */ 


#ifndef APP_TASKS_H_
#define APP_TASKS_H_

// Tasks

void appTask(void *pvParameters);

void CO2Task(void *pvParameters);

void tempHumTask(void *pvParameters);

void luxTask(void *pvParameters);

void servoTask(void *pvParameters);

void upLinkTask(void *pvParameters);

void downLinkTask(void *pvParameters);

#endif /* APP_TASKS_H_ */