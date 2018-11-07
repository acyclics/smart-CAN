/*
File: can.c
Header file: can.h
Function: CAN1
Details: https://github.com/impeccableaslan/smart-CAN
*/

/*
Setup:
1. Please refer to the guide located here: https://github.com/impeccableaslan/smart-CAN

How to initialize CAN for your project:
1. Include "can.h" in your file.
2. Create a Can variable (e.g. Can device;)
3. Initialize Can variable by calling Device_initialize(parameters).
Note that you would've to fill in the paramters yourself according to
your needs. For details refer to complete guide on GitHub.
4. In main, call CAN_Initialize somewhere below MX_CAN1_Init.

How to transmit message:
1. Use Can_Transmit(). Refer to function below for details.

How to receive message:
1. There are two methods to receive message. Refer to functions below for details.
*/

/* ========== includes ========== */
#include "can.h"
#include <stdio.h>
#include <stdlib.h>
/* =============================== 
"can.h" is the header file that you should #include if CAN is needed.
"stdio.h" and "stdlib.h" are header files to ease our use of pointers.
==================================*/

/* ========== can bus ========== */
CAN_HandleTypeDef hcan1;
CAN_RxHeaderTypeDef can1RxHeader;
/* ==============================
CAN_HandleTypeDef is a stm32's struct that initializes CAN.
We define it in "can.c", if needed elsewhere than either #include "can.c" in that file or use "extern CAN_HandleTypeDef hcan1;"
=================================*/

/* ========== filters ========== */
CAN_FilterTypeDef can1Filter;
/* ==============================
Filter configures the process of receiving message from CAN.
You can configure multiple filters if needed.
=================================*/

/* ========== counter ========== */
uint32_t can_count = 0;
/* ==============================
A counter that increments by 1 whenever CAN received a message.
=================================*/

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
/* ===============================================
Initializes CAN and filter
==================================================*/

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
/* ============================================
Receive CAN message. This is one of two methods.

How to setup:
1. In the file "stm32f4xx_it.c", place the line "HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&can1RxHeader,canRxMsg);" inside the function "void CAN1_RX0_IRQHandler(void)".
2. Place the function "CanReceiveMsgProcess(&can1RxHeader,canRxMsg);" under the line you placed in step 1.

How to use:
1. Each CAN message to be received should have an ID.
2. Add the ID to the switch statement, then add a function under that switch statement to do something with the received message.

The second method is "Device_Receive". That method is better if there are a lot of devices that uses CAN.
This method is better if there is less device as it is faster for less device.
===============================================*/

/* ========== smart CAN ========== */
Can** all_rx_devices;
unsigned int total_rx_device = 0;
/* ================================
Pointers to store the Can devices created.
Having memory of the devices makes it so that we can automate a lot of CAN related processes.
===================================*/

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
/* =====================================================
Device_Initialize is a function to ease the process of creating a Can device.
Device_ResizeRxBuffer is used to resize pointers according to user input.
========================================================*/

/* ========== smart CAN transmit functions ========== */
void Can_Transmit(Can* device,CAN_HandleTypeDef* hcan,uint8_t* canMsg)
{
	CAN_TxHeaderTypeDef msgHeader = device->canTxHeader;
	HAL_CAN_AddTxMessage(hcan,&msgHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
/* ===================================================
Transmit the message.

How to use:
@param "device": the Can device to send message (e.g. Can chassis;).
@param "hcan": the CAN to use (e.g. hcan1 for CAN 1).
@param "canMsg": the message the device should send via CAN (e.g. uint8_t msg[8];).
======================================================*/

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
void Can_Receive(CAN_HandleTypeDef* hcan, uint8_t* canRxMsg)
{
	can_count++;
	CAN_RxHeaderTypeDef canRxHeader;
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&canRxHeader,canRxMsg);
	Device_Receive(&canRxHeader, canRxMsg);
}
/* ==================================================
Warning: This method, although better, is still being developed. For now, it is recommended to use the other msg receive process.

Receive the message. This is one of two methods

How to setup:
1. In the file "stm32f4xx_it.c", place the function "Can_Receive(&hcan1,canRxMsg);" inside the function "void CAN1_RX0_IRQHandler(void)".

How to use:
This method automatically places the received message into each "Can device".
To read the received message for each Can device, simply access the "data" variable for each device.
So, there's no need to manually edit "CanReceiveMsgProcess" unlike the other method.

The second method is "CanReceiveMsgProcess". That method is better if there is less device as it is faster for less device.
This method is better if there are a lot of devices that uses CAN.
=====================================================*/
