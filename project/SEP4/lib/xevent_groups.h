/*
 * xevent_groups.h
 *
 * Created: 5/26/2022 9:11:46 AM
 *  Author: nordesk
 */ 

#include <event_groups.h>


#ifndef XEVENT_GROUPS_H_
#define XEVENT_GROUPS_H_

#define BIT_MEASURE_CO2	( 1 << 0 ) // BIT 0
#define BIT_MEASURE_HUM_TEMP ( 1 << 1 ) // BIT 1
#define BIT_MEASURE_LUX ( 1 << 2 ) // BIT 2
#define BIT_MEASURE_ALL (BIT_MEASURE_CO2 | BIT_MEASURE_HUM_TEMP | BIT_MEASURE_LUX)

#define BIT_READY_CO2 ( 1 << 0 ) // BIT 0
#define BIT_READY_HUM_TEMP ( 1 << 1 ) // BIT 1
#define BIT_READY_LUX ( 1 << 2 ) // BIT 2
#define BIT_READY_ALL (BIT_READY_CO2 | BIT_READY_HUM_TEMP | BIT_READY_LUX)

extern void initEventGroups();

extern EventGroupHandle_t getMeasureEventGroup();
extern EventGroupHandle_t getDataReadyEventGroup();
extern EventGroupHandle_t getNumbersEventGroup();



#endif /* XEVENT_GROUPS_H_ */