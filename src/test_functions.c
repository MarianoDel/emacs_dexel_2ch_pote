//------------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board #####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "tim.h"
// #include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "temperatures.h"

#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];

// extern volatile unsigned char f_channel_2_int;
// extern volatile unsigned char f_channel_4_int;
// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(1000);
    }
}


void TF_Control_Fan (void)
{
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(5000);
    }
}


void TF_TIM16_Pwm_CH1N (void)
{
    TIM_16_Init ();
    Update_TIM16_CH1N (2048);

    while (1);
    
}


void TF_TIM14_Pwm_CH1 (void)
{
    TIM_14_Init ();
    Update_TIM14_CH1 (2048);

    while (1);
    
}


void TF_TIM3_CH1_ConstantOff_TIM3_CH2_TriggerInput (void)
{
    TIM_16_Init ();
    Update_TIM16_CH1N (4095);
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


void TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput (void)
{
    TIM_14_Init ();
    Update_TIM14_CH1 (4095);
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


void TF_Adc_With_DMA_TIM16_Pwm_CH1N (void)
{
    TIM_16_Init ();
    
    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;

    while (1)
    {
        Wait_ms(5);
        // Update_TIM16_CH1N (Temp_Channel);
        // Update_TIM16_CH1N (Pote_Channel_1);
        Update_TIM16_CH1N (Pote_Channel_2);        
    }
    
}


void TF_Two_Complete_Channels_Hardware (void)
{
    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;

    // Start of Complete Pote Channel 1
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Pote Channel 2
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
    while (1)
    {
        Wait_ms(5);

        Update_TIM14_CH1 (Pote_Channel_1);
        Update_TIM16_CH1N (Pote_Channel_2);        
    }
    
}


void TF_Two_Complete_Channels_Hardware_With_Offset (void)
{
    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;

    // Start of Complete Pote Channel 1
    TIM_14_Init ();
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();
    
    // Start of Complete Pote Channel 2
    TIM_16_Init ();
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
    while (1)
    {
        Wait_ms(5);

        Update_TIM14_CH1 (Pote_Channel_1 + 125);
        Update_TIM16_CH1N (Pote_Channel_2 + 125);        
    }
    
}


void TF_TIM17_Interrupt (void)
{
    TIM_17_Init ();

    TIM17Enable();
    while (1);
    
}


void TF_TIM17_Interrupt_Soft_Pwm (void)
{
    TIM_17_Init ();

    // Start of Complete Pote Channel 1    
    TIM_14_Init ();
    Update_TIM14_CH1 (125);    
    TIM_1_Init_pwm_neg_CH1_trig_CH2 ();

    // Start of Complete Pote Channel 2
    TIM_16_Init ();
    Update_TIM16_CH1N (125);
    TIM_3_Init_pwm_neg_CH1_trig_CH2 ();    
    
    PWM_Soft_Set_Channels (1, 127);
    PWM_Soft_Set_Channels (2, 0);
    
    TIM17Enable();
    while (1);
    
}

//--- end of file ---//
