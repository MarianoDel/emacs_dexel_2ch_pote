//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TIM.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "hard.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_TIM1_CLK    (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLK_ON    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLK_OFF    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM3_CLK    (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLK_ON    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLK_OFF    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLK_ON    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLK_OFF    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM14_CLK    (RCC->APB1ENR & 0x00000100)
#define RCC_TIM14_CLK_ON    (RCC->APB1ENR |= 0x00000100)
#define RCC_TIM14_CLK_OFF    (RCC->APB1ENR &= ~0x00000100)

#define RCC_TIM15_CLK    (RCC->APB2ENR & 0x00010000)
#define RCC_TIM15_CLK_ON    (RCC->APB2ENR |= 0x00010000)
#define RCC_TIM15_CLK_OFF    (RCC->APB2ENR &= ~0x00010000)

#define RCC_TIM16_CLK    (RCC->APB2ENR & 0x00020000)
#define RCC_TIM16_CLK_ON    (RCC->APB2ENR |= 0x00020000)
#define RCC_TIM16_CLK_OFF    (RCC->APB2ENR &= ~0x00020000)

#define RCC_TIM17_CLK    (RCC->APB2ENR & 0x00040000)
#define RCC_TIM17_CLK_ON    (RCC->APB2ENR |= 0x00040000)
#define RCC_TIM17_CLK_OFF    (RCC->APB2ENR &= ~0x00040000)


// Externals -------------------------------------------------------------------
extern volatile unsigned short wait_ms_var;


// Globals ---------------------------------------------------------------------
volatile unsigned char f_channel_2_int = 0;
volatile unsigned char f_channel_4_int = 0;


// Module Functions ------------------------------------------------------------
void Update_TIM3_CH1 (unsigned short a)
{
    TIM3->CCR1 = a;
}


void Update_TIM3_CH2 (unsigned short a)
{
    TIM3->CCR2 = a;
}


void Update_TIM3_CH3 (unsigned short a)
{
    TIM3->CCR3 = a;
}


void Update_TIM3_CH4 (unsigned short a)
{
    TIM3->CCR4 = a;
}


void Update_TIM1_CH1 (unsigned short a)
{
    TIM1->CCR1 = a;
}


void Update_TIM1_CH2 (unsigned short a)
{
    TIM1->CCR2 = a;
}


void Update_TIM1_CH4 (unsigned short a)
{
    TIM1->CCR4 = a;
}


void Update_TIM14_CH1 (unsigned short a) 
{
    TIM14->CCR1 = a;
}


void Update_TIM16_CH1N (unsigned short a)
{
    TIM16->CCR1 = a;
}


void Wait_ms (unsigned short wait)
{
    wait_ms_var = wait;

    while (wait_ms_var);
}


#define VALUE_FOR_LEAST_FREQ    1200    //40KHz
#define VALUE_FOR_CONSTANT_OFF    173    //3.6us tick 20.83ns

void TIM_1_Init (void)
{
    unsigned long temp;

    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLK_ON;

#ifdef USE_F_CHNLS_FOR_FREQ_DETECT
    //Configuracion del timer.
    TIM1->CR1 = 0x00;		//clk int / 1; upcounting
    // TIM1->CR2 |= TIM_CR2_MMS_1;		//UEV -> TRG0

    TIM1->SMCR = 0x0000;
    // TIM1->CCMR1 = 0x2100;    //CH2 input filtered N=4 map IC2->TI2
    // TIM1->CCMR2 = 0x2100;    //CH4 input filtered N=4 map IC4->TI4
    TIM1->CCMR1 = 0x2d00;    //CH2 input filtered N=4 map IC2->TI2, pres / 8
    TIM1->CCMR2 = 0x2d00;    //CH4 input filtered N=4 map IC4->TI4, pres / 8
    
    TIM1->CCER |= TIM_CCER_CC4E | TIM_CCER_CC2E;    //CH4 & CH2 capture enable rising edge    
    TIM1->ARR = 0xFFFF;
    TIM1->CNT = 0;

    TIM1->PSC = 47;
#ifdef USE_OVERCURRENT_PROT
    // Enable timer interrupt see UDIS
    TIM1->DIER |= TIM_DIER_CC4IE | TIM_DIER_CC2IE;    // int on CC4 CC2 & overflow    
    // NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    // NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    NVIC_SetPriority(TIM1_CC_IRQn, 8);
#endif    
#endif

#ifdef USE_F_CHNLS_FOR_ENABLE
    //Configuracion del timer.
    TIM1->CR1 = 0x00;		//clk int / 1; upcounting
    // TIM1->CR2 |= TIM_CR2_MMS_1;		//UEV -> TRG0

    TIM1->SMCR = 0x0000;
    TIM1->CCMR1 = 0x6000;    //CH2 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
    TIM1->CCMR2 = 0x6000;    //CH4 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
    
    TIM1->CCER |= TIM_CCER_CC4E | TIM_CCER_CC2E;    //CH4 & CH2 enable on pin direct polarity

    TIM1->BDTR |= TIM_BDTR_MOE;
    
    TIM1->ARR = 4095;
    TIM1->CNT = 0;

    TIM1->PSC = 2;
#endif
    
    //Alternate Fuction Pin Configurations
    temp = GPIOA->AFR[1];
    temp &= 0xFFFF0F0F;    
    temp |= 0x00002020;    //PA11 -> AF2; PA9 -> AF2
    GPIOA->AFR[1] = temp;
    
    TIM1->CR1 |= TIM_CR1_CEN;
}


void TIM_1_Init_pwm_CH1_trig_CH2 (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLK_ON;

    //Configuracion del timer.
    TIM1->CR1 = 0x00;        //clk int / 1;
    TIM1->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
    
    //Reset mode, trigger with TI2
    TIM1->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;

    //CH1 output PWM mode 2 (channel active TIM1->CNT > TIM1->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM1->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;
    // TIM1->CCMR1 = 0x0070;
    
    TIM1->CCMR2 = 0x0000;
    TIM1->CCER |= TIM_CCER_CC1E;    // CH1 enable
    
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;    //cada tick 20.83ns

    TIM1->CNT = 0;
    TIM1->PSC = 0;

    TIM1->BDTR |= TIM_BDTR_MOE;    

    unsigned int temp = 0;    
    temp = GPIOA->AFR[1];
    temp &= 0xFFFFFF00;
    temp |= 0x00000022;    //PA9 -> AF2; PA8 -> AF2
    GPIOA->AFR[1] = temp;
    
    // Enable timer ver UDIS
    TIM1->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM1->CR1 |= TIM_CR1_CEN;

}


void TIM_3_Init_pwm_CH1_trig_CH2 (void)
{
    if (!RCC_TIM3_CLK)
        RCC_TIM3_CLK_ON;

    //Configuracion del timer.
    TIM3->CR1 = 0x00;        //clk int / 1;
    TIM3->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
    
    //Reset mode, trigger with TI2
    TIM3->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;

    //CH1 output PWM mode 2 (channel active TIM3->CNT > TIM3->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM3->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;
    // TIM3->CCMR1 = 0x0070;
    
    TIM3->CCMR2 = 0x0000;
    TIM3->CCER |= TIM_CCER_CC1E;    // CH1 enable
    
    TIM3->ARR = VALUE_FOR_LEAST_FREQ;    //cada tick 20.83ns

    TIM3->CNT = 0;
    TIM3->PSC = 0;

    unsigned int temp = 0;    
    temp = GPIOB->AFR[0];
    temp &= 0xFF00FFFF;
    temp |= 0x00110000;    //PB5 -> AF1; PB4 -> AF1
    GPIOB->AFR[0] = temp;
    
    // Enable timer ver UDIS
    TIM3->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM3->CR1 |= TIM_CR1_CEN;

}


void TIM3_IRQHandler (void)	//1 ms
{
	/*
	Usart_Time_1ms ();

	if (timer_1seg)
	{
		if (timer_1000)
			timer_1000--;
		else
		{
			timer_1seg--;
			timer_1000 = 1000;
		}
	}

	if (timer_led_comm)
		timer_led_comm--;

	if (timer_standby)
		timer_standby--;
	*/
	//bajar flag
	if (TIM3->SR & 0x01)	//bajo el flag
		TIM3->SR = 0x00;
}


void TIM14_IRQHandler (void)	//20us
{
    if (TIM14->SR & 0x01)
        TIM14->SR = 0x00;    //bajar flag

}


void TIM_14_Init (void)
{
    if (!RCC_TIM14_CLK)
        RCC_TIM14_CLK_ON;

    TIM14->CR1 = 0x00;		//clk int / 1; upcounting; uev

    TIM14->CCMR1 = 0x0060;            //CH1 output PWM mode 1
    TIM14->CCER |= TIM_CCER_CC1E;    //CH1 enable on pin active high

    TIM14->PSC = 2;
    TIM14->ARR = 4096;    // 4KHz 4096pts

    // Pins alternative config.
    unsigned int temp;
    temp = GPIOA->AFR[0];
    temp &= 0x0FFFFFFF;
    temp |= 0x50000000;    //PA7 -> AF5
    GPIOA->AFR[0] = temp;

    TIM14->CR1 |= TIM_CR1_CEN;
}


void TIM16_IRQHandler (void)	//tick on 1uS
{
    if (TIM16->SR & 0x01)
        TIM16->SR = 0x00;
}


void TIM_16_Init (void)
{
    if (!RCC_TIM16_CLK)
        RCC_TIM16_CLK_ON;

    //Configuracion del timer.
    TIM16->CR1 = 0x00;		//clk int / 1; upcounting; uev

    TIM16->CCMR1 = 0x0060;            //CH1 output PWM mode 1
    // TIM16->CCER |= TIM_CCER_CC1NE | TIM_CCER_CC1NP;    // CH1N enabled polarity reversal
    TIM16->CCER |= TIM_CCER_CC1NE;    // CH1N enabled
    TIM16->ARR = 4096;    // 4KHz 4096 pts    
    TIM16->CNT = 0;
    TIM16->PSC = 2;

    TIM16->BDTR |= TIM_BDTR_MOE;

    // Pins alternative config.
    unsigned int temp;
    temp = GPIOB->AFR[0];
    temp &= 0xF0FFFFFF;
    temp |= 0x02000000;	//PB6 -> AF2
    GPIOB->AFR[0] = temp;

    TIM16->CR1 |= TIM_CR1_CEN;

}

//--- end of file ---//
