/*
 * main.c
 *
 *  Created on: Jan 5, 2026
 *      Author: ekrem
 */

#include "main.h"

void errorHandler(void);
void TIMER2Init(void);
void UART2Init(void);
void systemClockConfigHSE(uint8_t clockFreq);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;

/*
 * f = 1/T , 500 = 1/T , T = 0.002s
 *
 *   0.001s    0.001s
 *   ________         _
 * _|        |_______|
 *
 * 25000000 1s
 * 25000    0.001s
 */
uint32_t pulse1_value = 25000; //to produce 500hz
uint32_t pulse2_value = 12500; //to produce 1000hz
uint32_t pulse3_value = 6250;  //to produce 2000hz
uint32_t pulse4_value = 3125;  //to produce 4000hz
uint32_t ccr_content;

int main(){

	HAL_Init();
	systemClockConfigHSE(SYS_CLOCK_FREQ_50_MHZ);
	UART2Init();
	TIMER2Init();

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		errorHandler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK){
		errorHandler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK){
		errorHandler();
	}

	if(HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK){
		errorHandler();
	}

	while(1){

	}

	return 0;
}

void TIMER2Init(void){
	TIM_OC_InitTypeDef tim2OC_init;

	htimer2.Instance = TIM2;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.Prescaler = 1;
	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK){
		errorHandler();
	}

	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCNPolarity = TIM_OCNPOLARITY_HIGH; //that means not inverted

	tim2OC_init.Pulse = pulse1_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_1) != HAL_OK){
		errorHandler();
	}

	tim2OC_init.Pulse = pulse2_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_2) != HAL_OK){
		errorHandler();
	}

	tim2OC_init.Pulse = pulse3_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_3) != HAL_OK){
		errorHandler();
	}

	tim2OC_init.Pulse = pulse4_value;
	if(HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_4) != HAL_OK){
		errorHandler();
	}
}

void UART2Init(void){
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK){
		errorHandler();
	}
}

void systemClockConfigHSE(uint8_t clock_freq){
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	uint8_t FLatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_ON;
	//osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; //no need calibration for hse
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq) {
	  case SYS_CLOCK_FREQ_50_MHZ:{
	    osc_init.PLL.PLLM = 8;
	    osc_init.PLL.PLLN = 100;
	    osc_init.PLL.PLLP = 2;
	    osc_init.PLL.PLLQ = 2; //default
	    //osc_init.PLL.PLLR = 2; //default

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	                         RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	    FLatency = FLASH_ACR_LATENCY_1WS; // rm0090 relation between CPU clock frequency and flash memory read time
	    break;
	  }
	  case SYS_CLOCK_FREQ_84_MHZ:{
	    osc_init.PLL.PLLM = 8;
	    osc_init.PLL.PLLN = 168;
	    osc_init.PLL.PLLP = 2;
	    osc_init.PLL.PLLQ  = 2;
	    //osc_init.PLL.PLLR=2;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	                         RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	    FLatency = FLASH_ACR_LATENCY_2WS;

	    break;
	  }
	  case SYS_CLOCK_FREQ_120_MHZ: {
	    osc_init.PLL.PLLM = 8;
	    osc_init.PLL.PLLN = 240;
	    osc_init.PLL.PLLP = 2;
	    osc_init.PLL.PLLQ  = 2;
	    //osc_init.PLL.PLLR=2;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	                         RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	    FLatency = FLASH_ACR_LATENCY_3WS;
	    break;
	  }
	  case SYS_CLOCK_FREQ_168_MHZ: {
		//enable the clock for the power controller
		__HAL_RCC_PWR_CLK_ENABLE();

		//set regulator voltage scale as 1
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		//turn on the over drive mode of voltage regulator for nucleo
		//__HAL_PWR_OVERDRIVE_ENABLE();

	    osc_init.PLL.PLLM = 4;
	    osc_init.PLL.PLLN = 168;
	    osc_init.PLL.PLLP = 2;
	    osc_init.PLL.PLLQ  = 2;
	    //osc_init.PLL.PLLR=2;

	    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
	                         RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
	    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	    FLatency = FLASH_ACR_LATENCY_7WS;
	    break;
	  }

	  default:
	    return;
	  }

	  if(HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		  errorHandler();
	  }

	  if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK){
		  errorHandler();
	  }

	  //systick conf
	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

//so whenever the count matches with the CCR1,So that is considered as delay elapsed and you will get this call back.
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content + pulse1_value);
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_content + pulse2_value);
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_content + pulse3_value);
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_content + pulse4_value);
	}
}

void errorHandler(void){
	while(1);
}
