# Step 1 - Configure CAN and the stm32 internal clock with cubemx:

1. Copy the settings from HERO_CAN.ioc located [here](https://github.com/impeccableaslan/smart-CAN/tree/master/HERO_CAN).
2. Once you generated the code using cubemx, go to the file "main.c" and make the following changes:
- Remove the "hcan1" declaration
- Declare "uint8_t canRxMsg[8];" somewhere above int main()
- In void SystemClock_Config(void), copy and paste the below settings:
```c
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
```
- in MX_CAN1_Init, copy and paste the below settings:
```c
static void MX_CAN1_Init(void)
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

}
```
- That's all for "main.c" You can check the example below for an example of the whole main.c file.
- In the file "stm32f4xx_it.c", add the below code somewhere near the beginning of the file:
```c
#include "can.h"
extern uint8_t canRxMsg[8];
extern CAN_RxHeaderTypeDef can1RxHeader;
```
- In the file "stm32f4_hal_msp.c", add the below code somewhere in the file:
```c
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }

}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }

}
```

# Step 2 - How to create a "Can device", transmit, and receive message:

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

# Second method of receiving CAN message

*Development in progress*
