/*
File: can.h
Function: CAN1
Details: https://github.com/impeccableaslan/smart-CAN
*/

#ifndef __CAN_H
#define __CAN_H

/* ========== includes ========== */
#include "main.h"
#include "stm32f4xx_hal.h"
/* ============================== */

/* ========== headers ========== */
extern CAN_HandleTypeDef hcan1;
/* ============================= */

/* ========== ids ========== */
#define CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID           0x201
#define CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID           0x202 
#define CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID           0x203
#define CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID           0x204
#define CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID           0x205
#define CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID           0x206
#define CAN_BUS2_MOTOR7_FEEDBACK_MSG_ID 					0x207
#define CAN_BUS2_MOTOR8_FEEDBACK_MSG_ID 					0x208
/* ========================= */

/* ========== error handling ========== */
extern void _Error_Handler(char *, int);
/* ==================================== */

/* ========== CAN initialize functions ========== */
void CAN_Initialize(void);
/* ============================================== */

/* ========== CAN receive functions ========== */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg);
/* =========================================== */

/* ========== smart CAN ========== */
typedef struct Can
{
	/* ========== headers ========== */
	CAN_TxHeaderTypeDef canTxHeader;
	
	/* ========== settings ========== */
	uint32_t StdId;
	uint32_t IDE;
	uint32_t RTR;
	uint32_t DLC;
	uint32_t rx_StdId;
	uint8_t* data;
	int rx_buffer_size;
} Can;
/* =============================== */

/* ========== smart CAN functions ========== */
void Device_Initialize(Can* device,const uint32_t StdId, const uint32_t IDE, const uint32_t RTR, const uint32_t DLC, const uint32_t rx_StdId, const int rx_buffer_size);
void Can_Transmit(Can* device,CAN_HandleTypeDef* hcan,uint8_t* canMsg);
void Can_Receive(CAN_HandleTypeDef* hcan, uint8_t* canRxMsg);	// place this in can1/2_rx_isr
void Device_Activate_Rx(Can* device);
void test_smart_can(Can* device, CAN_HandleTypeDef* hcan,int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq);
void Device_ResizeRxBuffer(Can* device, int rx_buffer_size);

#endif
