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





/* Stuff below this line is the new experimental way to use CAN */






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

/* Below is to test the PID */
#define SPEED_OUTPUT_ATTENUATION (1.0f)
#define RATE_BUF_SIZE 6
typedef struct{
	int32_t raw_value;   									//???????????
	int32_t last_raw_value;								//??????????
	int32_t ecd_value;                       //????????????
	int32_t diff;													//??????????
	int32_t temp_count;                   //???
	uint8_t buf_count;								//????buf?
	int32_t ecd_bias;											//??????	
	int32_t ecd_raw_rate;									//???????????????
	float rate_buf[RATE_BUF_SIZE];	//buf,for filter
	int32_t round_cnt;										//??
	int32_t filter_rate;											//??
	float ecd_angle;				//??
	float last_ecd_angle;
	float ecd_speed;
}Encoder;

typedef struct PID_Regulator_t
{
	float ref;
	float fdb;
	float err[2];
	float output;
	float last_output;
	float kp;
	float ki;
	float kd;
	float KpComponent;
	float KiComponent;
	float KdComponent;
	float output_limit;
	float windup_limit;
	float max_step;
	void (*Calc)(struct PID_Regulator_t*);
	void (*Reset)(struct PID_Regulator_t*);
	float Ki_Limit;
}PID_Regulator_t;
void PID_Calc(PID_Regulator_t *pid); 
void PID_Reset(PID_Regulator_t *pid);
#define CHASSIS_MOTOR_SPEED_PID_DEFAULT \
{\
	0,\
	0,\
	{0,0},\
	0,\
	0,\
	80.0f,\
	3.0f,\
	5.0f,\
	0,\
	0,\
	0,\
	5000,\
	0,\
	800,\
	&PID_Calc,\
	&PID_Reset,\
}\

void Chassis_ReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg);
#endif
