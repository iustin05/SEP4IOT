/*
 * program_config.h
 *
 * Created: 5/27/2022 4:53:20 PM
 *  Author: nordesk
 */ 


#ifndef PROGRAM_CONFIG_H_
#define PROGRAM_CONFIG_H_

//     DEBUG AREA     //

//#define DEBUG_NONETWORK
#define DEBUG_PRINTS
#define DEBUG_EXTRA_DATA

//   DEBUG AREA END   //


//     SETTINGS AREA     //

// TIMEOUTS, DELAYS & CYCLE DURATION (in ms)
#define SETTING_TIMEOUT_DOWNLINK 410000
#define SETTING_TIMEOUT_UPLINK 420000
#define SETTING_TIMEOUT_CO2 390000
#define SETTING_TIMEOUT_LUX 380000
#define SETTING_TIMEOUT_TH 370000
#define SETTING_CYCLE_SERVO 200000
#define SETTING_CYCLE_APP 60000 // Time between two uploads (readings)
#define SETTING_TIMEOUT_WAIT_READY_BITS 200
#define SETTING_TIMEOUT_WAIT_LED_BITS 7000


#define SETTING_LORA_APPEUI "2BBE8F09765BBF4B"
#define SETTING_LORA_APPKEY "5F83717BC67B4646E3F00DC5EC3417DC"
#define SETTING_LORA_MAXJOINTRIES 10
#define SETTING_LORA_RECEIVEDELAY 500

//  SETTINGS AREA END   //

#endif /* PROGRAM_CONFIG_H_ */