/*
 * message_buffers.h
 *
 * Created: 5/26/2022 1:12:57 AM
 *  Author: nordesk
 */ 


#ifndef MESSAGE_BUFFERS_H_
#define MESSAGE_BUFFERS_H_

#include <message_buffer.h>

extern MessageBufferHandle_t getUpLinkMessageBuffer();
extern MessageBufferHandle_t getDownLinkMessageBuffer();

#endif /* MESSAGE_BUFFERS_H_ */