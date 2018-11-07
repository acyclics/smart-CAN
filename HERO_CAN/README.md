# How to create a "Can device", transmit, and receive message

1. Include "can.h" in your file.
```c
// in your_file.c
#include "can.h"
```

2. Create a Can variable. The great thing about smart CAN is that you can treat the electronic device as a "Can object", it's basically plug and play so you can get CAN up and running hassle free. Therefore, from now on you can treat each "Can variable" as your electronic device.
```c
Can device;
```

3. Initialize Can variable by calling Device_initialize(parameters). The parameters are as follows:
```c
Device_Initialize(Can* device,const uint32_t StdId, const uint32_t IDE, const uint32_t RTR, const uint32_t DLC, const uint32_t rx_StdId, const int rx_buffer_size)

device: the Can variable passed as a pointer

StdId: the identifier (id) of the electronic device when using CAN

IDE: specifies whether you're using the standard or extended CAN protocol. Use the macro "CAN_ID_STD" to specify the standard CAN protocol and "CAN_ID_EXT" for the extended CAN protocol.

RTR: sets the type of frame to be transmitted. Use the macro "CAN_RTR_DATA" to transmit data.

DLC: the size of the data, in bytes (maximum 8 bytes), to be transmitted.

rx_StdId: the identifier (id) of the electronic device to receive CAN messages from

rx_buffer_size: size of the data, in bytes, to be received

// example initialization
Can device;
Device_Initialize(device,0x200, CAN_ID_STD, CAN_RTR_DATA, 8, 0x201, 8);
```

4. Edit the function void CAN1_RX0_IRQHandler(void) as shown. This function is located in the file "stm32f4xx_it.c".
```c
uint8_t canRxMsg[8];
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */
	HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&can1RxHeader,canRxMsg);
	CanReceiveMsgProcess(&can1RxHeader,canRxMsg);
  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}
```
Note that canRxMsg is used to store all messages received by the stm32 through CAN. So, the size for canRxMsg[x] should be the largest amount of bytes to be received by the stm32 from an electronic device. In this example case, it is 8 bytes.

5. In main, call CAN_Initialize somewhere below MX_CAN1_Init.
```c
// in main.c
MX_CAN1_Init;
CAN_Initialize;
```

6. Lastly, send messages to your Can device by calling Can_Transmit(parameters). The parameters are as follows:
```c
Can_Transmit(Can* device,CAN_HandleTypeDef* hcan,uint8_t* canMsg);

device: the Can variable passed as a pointer
hcan: the CAN bus to be used for sending and receiving messages. In most cases, use CAN1, which by default can be referred to with "&hcan1".
canMsg: the message to be transmitted to the electronic device. This is a pointer to an array containing the data to be sent.
```

# Example
```c
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  
  // MX_CAN1_Init() is the cubemx generated function to initialize CAN1
  MX_CAN1_Init();
  // initialize smart CAN
  CAN_Initialize();
  
  MX_TIM2_Init();
  MX_TIM12_Init();
  MX_TIM6_Init();
  MX_TIM3_Init();
  HAL_TIM_PWM_Init(&htim2);
  HAL_TIM_PWM_Init(&htim12);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim2);

  // create and initialize Can variable here. In this example, the electronic device is a motor
  Can motor;
  Device_Initialize(&motor,0x200, CAN_ID_STD, CAN_RTR_DATA, 8, 0x201, 8);
  
  // create message to be sent here. It doesn't matter if you don't understand it, just know that you would've to fill in the 	// array somehow before sending the message (which should be contained by the array) through CAN.
  canTxMsg0[0] = (uint8_t)(5000 >> 8);
  canTxMsg0[1] = (uint8_t)5000;
  canTxMsg0[2] = (uint8_t)(5000 >> 8);
  canTxMsg0[3] = (uint8_t)5000;
  canTxMsg0[4] = (uint8_t)(5000 >> 8);
  canTxMsg0[5] = (uint8_t)5000;
  canTxMsg0[6] = (uint8_t)(5000 >> 8);
  canTxMsg0[7] = (uint8_t)5000;

  while (1)
  {
  	HAL_Delay(50);
	// transmit message to the electronic device in a loop so we can keep updating the electronic device. In this case, it 	       // is a motor so we want to keep sending messages to it to tell it how fast it should spin 
	Can_Transmit(&motor, &hcan1, canTxMsg0);
  }
}
```

# first method of receiving CAN message

1. Go to "can.c" and locate the function called "CanReceiveMsgProcess".
2. This function is what is used to do something with the received CAN messages. Inside the function, there's a switch statement that goes through each ID of the received message. So, you would have to figure out what the ID your device sends messages with, then add the ID to the switch statement.
For example, if my device send messages with the ID '0x200', then in the function "CanReceiveMsgProcess", I would makes the following changes:
```c
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{      

	can_count++;
		switch(rxHeader->StdId)
		{
				case 0x200:
				{
					// I made a new statement for 0x200 here
					function_to_do_something_here();
				}break;
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
```

# Second method of receiving CAN message

*Development in progress*
