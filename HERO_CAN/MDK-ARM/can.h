/*
Please refer to can.c for documentation.
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

/* ========== CAN user/transmit functions ========== */
void CAN_SendMsg(CAN_HandleTypeDef* hcan,CAN_TxHeaderTypeDef *canTxHeader,uint8_t* canMsg);
void set_CM_speed(int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq);
/* ================================================= */

/* ========== CAN receive functions ========== */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg);
/* =========================================== */

#endif
