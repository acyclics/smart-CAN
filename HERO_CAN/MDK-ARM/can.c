/*
Complete guide to use smart CAN:
Learn CAN here:

Quick guide:
1. Include "can.h" in your file.
2. Create a Can variable (e.g. Can device;)
3. Initialize Can variable by calling Device_initialize(parameters).
Note that you would've to fill in the paramters yourself according to
your needs. For details refer to complete guide.
4. Place the function Can_Receive(parameters) into void CAN1_RX0_IRQHandler(void)
, which is located in the file "stm32f4xx_it.c".
5. In main, call CAN_Initialize somewhere below MX_CAN1_Init.
6. Lastly, send messages to your Can device by calling Can_Transmit(parameters).
For details refer to complete guide.
*/

/* ========== includes ========== */
#include "can.h"
#include <stdio.h>
#include <stdlib.h>
/* ============================== */

/* ========== can bus ========== */
CAN_HandleTypeDef hcan1;
/* ============================= */

/* ========== headers ========== */
CAN_TxHeaderTypeDef can1TxHeader0;
CAN_TxHeaderTypeDef test;
CAN_TxHeaderTypeDef can1TxHeader1;
CAN_RxHeaderTypeDef can1RxHeader;
/* ============================= */

/* ========== filters ========== */
CAN_FilterTypeDef can1Filter;
/* ============================= */

/* ========== buffers ========== */
uint8_t canTxMsg0[8] = {0};
uint8_t canTxMsg1[8] = {0};
uint32_t can_count=0;
/* ============================= */

/* ========== CAN initialize functions ========== */
void CAN_Initialize(void)
{
	hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	can1TxHeader0.IDE = CAN_ID_STD;
	can1TxHeader0.StdId = 0x200;
	can1TxHeader0.DLC = 8;
	
	test.IDE = CAN_ID_STD;
	test.StdId = 0x200;
	test.DLC = 8;
	
	can1TxHeader1.IDE = CAN_ID_STD;
	can1TxHeader1.StdId = 0x1FF;
	can1TxHeader1.RTR = CAN_RTR_DATA;
	can1TxHeader1.DLC = 8;
	
	
	can1Filter.FilterActivation = ENABLE;
	can1Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can1Filter.FilterFIFOAssignment = CAN_FilterFIFO0;
	can1Filter.FilterIdHigh = 0x0000;
	can1Filter.FilterIdLow = 0x0000;
	can1Filter.FilterBank = 0;
	HAL_CAN_ConfigFilter(&hcan1,&can1Filter);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_Start(&hcan1);
}
/* ============================================== */
//
//
//
//
//
/* Stuff below this line is the old implementation of CAN */
//
//
//
//
//
/* ========== CAN user/transmit functions ========== */
void CAN_SendMsg(CAN_HandleTypeDef* hcan,CAN_TxHeaderTypeDef *canTxHeader,uint8_t* canMsg)
{
	HAL_CAN_AddTxMessage(hcan,canTxHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
// use set_CM_speed for chassis test. However, this function will soon be degraded >:)
void set_CM_speed(int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq)
{
    canTxMsg0[0] = (uint8_t)(cm1_iq >> 8);
    canTxMsg0[1] = (uint8_t)cm1_iq;
    canTxMsg0[2] = (uint8_t)(cm2_iq >> 8);
    canTxMsg0[3] = (uint8_t)cm2_iq;
    canTxMsg0[4] = (uint8_t)(cm3_iq >> 8);
    canTxMsg0[5] = (uint8_t)cm3_iq;
    canTxMsg0[6] = (uint8_t)(cm4_iq >> 8);
    canTxMsg0[7] = (uint8_t)cm4_iq;
    CAN_SendMsg(&hcan1,&can1TxHeader0,canTxMsg0);
}
/* ================================================= */

/* ========== CAN receive functions ========== */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{      

	can_count++;
		switch(rxHeader->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				{                    
				}break;
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID:
				{
				}break;
				
				case CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID:
				{	
				}break;		
				case CAN_BUS2_MOTOR7_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR8_FEEDBACK_MSG_ID:
				{
				}break;
				
		}

}
/* =========================================== */
//
//
//
//
//
//
/* Stuff below this line is the new experimental way to use CAN */
//
//
//
//
//
//
/* ========== smart CAN ========== */
Can** all_rx_devices;
unsigned int total_rx_device = 0;
/* =============================== */

/* ========== smart CAN initialize functions ========== */
void Device_Initialize(Can* device,const uint32_t StdId, const uint32_t IDE, const uint32_t RTR, const uint32_t DLC, const uint32_t rx_StdId, const int rx_buffer_size)
{
	device->StdId = StdId;
	device->IDE = IDE;
	device->RTR = RTR;
	device->DLC = DLC;
	device->rx_StdId = rx_StdId;
	device->canTxHeader.IDE = device->IDE;
	device->canTxHeader.StdId = device->StdId;
	device->canTxHeader.DLC = device->DLC;
	//device.canTxHeader.RTR = device.RTR;
	device->rx_buffer_size = rx_buffer_size;
	
	Device_ResizeRxBuffer(device, device->rx_buffer_size);
	Device_Activate_Rx(device);
}
void Device_ResizeRxBuffer(Can* device, int rx_buffer_size)
{
	uint8_t* new_rx_buffer = realloc(device->data, rx_buffer_size);
	if (new_rx_buffer == NULL)
	{
		// error check
	}
	else
	{
		device->data = new_rx_buffer;
	}
}
/* ==================================================== */

/* ========== smart CAN transmit functions ========== */
void Can_Transmit(Can* device,CAN_HandleTypeDef* hcan,uint8_t* canMsg)
{
	CAN_TxHeaderTypeDef msgHeader = device->canTxHeader;
	HAL_CAN_AddTxMessage(hcan,&msgHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
/* ================================================== */

/* ========== smart CAN receive functions ========== */
void Device_Activate_Rx(Can* device)
{
	Can** new_rx_device = realloc(all_rx_devices, ++total_rx_device);
	if (new_rx_device == NULL)
	{
		// error check
	}
	else
	{
		all_rx_devices = new_rx_device;
		all_rx_devices[total_rx_device-1] = device;
	}
}
void Device_Receive(CAN_RxHeaderTypeDef* canRxHeader,uint8_t* canRxMsg)
{
	for (int i = 0; i < total_rx_device; ++i)
	{
		if (canRxHeader->StdId == all_rx_devices[i]->rx_StdId)
		{
			for (int j = 0; j < all_rx_devices[i]->rx_buffer_size; ++j)
			{
				all_rx_devices[i]->data[j] = canRxMsg[j];
			}
		}
	}
}
void Can_Receive(CAN_HandleTypeDef* hcan, uint8_t* canRxMsg)	// place this in can1/2_rx_isr
{
	CAN_RxHeaderTypeDef canRxHeader;
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&canRxHeader,canRxMsg);
	Device_Receive(&canRxHeader, canRxMsg);
}
/* ================================================= */

/* ========== smart CAN test functions ========== */
void test_smart_can(Can* device, CAN_HandleTypeDef* hcan,int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq)
{
		uint8_t canTxMsg0[8];
    canTxMsg0[0] = (uint8_t)(cm1_iq >> 8);
    canTxMsg0[1] = (uint8_t)cm1_iq;
    canTxMsg0[2] = (uint8_t)(cm2_iq >> 8);
    canTxMsg0[3] = (uint8_t)cm2_iq;
    canTxMsg0[4] = (uint8_t)(cm3_iq >> 8);
    canTxMsg0[5] = (uint8_t)cm3_iq;
    canTxMsg0[6] = (uint8_t)(cm4_iq >> 8);
    canTxMsg0[7] = (uint8_t)cm4_iq;
    Can_Transmit(device, hcan, canTxMsg0);
}
/* ============================================== */
//
//
//
//
//
//
/* Stuff below this line is for testing PID */
//
//
//
//
//
//
/* ========== PID ========== */
volatile Encoder CM1Encoder;
volatile Encoder CM2Encoder;
volatile Encoder CM3Encoder;
volatile Encoder CM4Encoder;  
PID_Regulator_t CM1SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM2SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM3SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;
PID_Regulator_t CM4SpeedPID = CHASSIS_MOTOR_SPEED_PID_DEFAULT;

void PID_Calc(PID_Regulator_t *pid)
{
	pid->err[1] = pid->err[0];
	pid->err[0] = (pid->ref - pid->fdb);

	//calculate PID
	pid->KpComponent = pid->kp * pid->err[0];
	pid->KiComponent += pid->ki * pid->err[0];
	pid->KdComponent = pid->kd * (pid->err[0] - pid->err[1]);
	pid->output = pid->KpComponent + pid->KiComponent+ pid->KdComponent;
	
	//output value limit
	if((pid->output) > pid->output_limit)
		(pid->output>0) ? (pid->output=pid->output_limit) : (pid->output = -pid->output_limit);
}
void EncoderProcess(volatile Encoder *v, uint8_t* msg)
{
	int i=0;
	int32_t temp_sum = 0;    
	v->last_raw_value = v->raw_value;
	v->raw_value = (msg[0]<<8)|msg[1];
	v->diff = v->raw_value - v->last_raw_value;
	if(can_count < 50)
	{
		v->ecd_raw_rate = 0;
	}
	else
	{
		if(v->diff < -7000)    //?????????????,?????????
		{
			v->round_cnt++;
			v->ecd_raw_rate = v->diff + 8192;
		}
		else if(v->diff>7000)
		{
			v->round_cnt--;
			v->ecd_raw_rate = v->diff- 8192;
		}		
		else
		{
			v->ecd_raw_rate = v->diff;
		}
	}
	//?????????????
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//???????,???????
	//v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192 + v->round_cnt * 360;
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*360/8192.0f;
	v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
	if(v->buf_count == RATE_BUF_SIZE)
	{
		v->buf_count = 0;
	}
	//???????
	for(i = 0;i < RATE_BUF_SIZE; i++)
	{
		temp_sum += v->rate_buf[i];
	}
	v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);		

}
void GetEncoderBias(volatile Encoder *v,CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{

            v->ecd_bias = (msg[0]<<8)|msg[1];  //????????????  
            v->ecd_value = v->ecd_bias;
            v->last_raw_value = v->ecd_bias;
            v->temp_count++;
}
void PID_Reset(PID_Regulator_t *pid)
{
	pid->ref = 0;
	pid->fdb = 0;
	pid->output = 0;
}
void send_Chassis_Msg(Can* chassis, int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq)
{
		uint8_t canTxMsg0[8];
    canTxMsg0[0] = (uint8_t)(cm1_iq >> 8);
    canTxMsg0[1] = (uint8_t)cm1_iq;
    canTxMsg0[2] = (uint8_t)(cm2_iq >> 8);
    canTxMsg0[3] = (uint8_t)cm2_iq;
    canTxMsg0[4] = (uint8_t)(cm3_iq >> 8);
    canTxMsg0[5] = (uint8_t)cm3_iq;
    canTxMsg0[6] = (uint8_t)(cm4_iq >> 8);
    canTxMsg0[7] = (uint8_t)cm4_iq;
    Can_Transmit(chassis, &hcan1, canTxMsg0);
}
void Motor_Task(Can chassis)
{
	CM1SpeedPID.ref = 600;
	CM2SpeedPID.ref = 600;
	CM3SpeedPID.ref = 600;
	CM4SpeedPID.ref = 600;
  CM1SpeedPID.fdb = CM1Encoder.filter_rate;
	CM2SpeedPID.fdb = CM2Encoder.filter_rate;
	CM3SpeedPID.fdb = CM3Encoder.filter_rate;
	CM4SpeedPID.fdb = CM4Encoder.filter_rate;
  PID_Calc(&CM1SpeedPID);
	PID_Calc(&CM2SpeedPID);
	PID_Calc(&CM3SpeedPID);
	PID_Calc(&CM4SpeedPID);
	send_Chassis_Msg(&chassis, CM1SpeedPID.output*SPEED_OUTPUT_ATTENUATION,CM2SpeedPID.output*SPEED_OUTPUT_ATTENUATION,CM3SpeedPID.output*SPEED_OUTPUT_ATTENUATION,CM4SpeedPID.output*SPEED_OUTPUT_ATTENUATION);	// hold motor please :)
}
// test using old way
int chassis_count = 0;
void Chassis_ReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{      

	chassis_count++;
		switch(rxHeader->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				{
				  (chassis_count<=50) ? GetEncoderBias(&CM1Encoder ,rxHeader,msg):EncoderProcess(&CM1Encoder ,msg);
				}break;
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				{
					(chassis_count<=50) ? GetEncoderBias(&CM2Encoder ,rxHeader,msg):EncoderProcess(&CM2Encoder ,msg);
				}break;
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				{
					(chassis_count<=50) ? GetEncoderBias(&CM3Encoder ,rxHeader,msg):EncoderProcess(&CM3Encoder ,msg);
				}break;
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
					(chassis_count<=50) ? GetEncoderBias(&CM4Encoder ,rxHeader,msg):EncoderProcess(&CM4Encoder ,msg);
				}break;
				case CAN_BUS2_MOTOR5_FEEDBACK_MSG_ID:
				{
				}break;
				
				case CAN_BUS2_MOTOR6_FEEDBACK_MSG_ID:
				{	
				}break;		
				case CAN_BUS2_MOTOR7_FEEDBACK_MSG_ID:
				{
				}break;
				case CAN_BUS2_MOTOR8_FEEDBACK_MSG_ID:
				{
				}break;
				
		}

}
