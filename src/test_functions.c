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
#include "gpio.h"
#include "uart.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"
#include "temperatures.h"

#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];

extern volatile unsigned char f_channel_2_int;
extern volatile unsigned char f_channel_4_int;
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
    TIM_3_Init_pwm_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


void TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput (void)
{
    TIM_1_Init_pwm_CH1_trig_CH2 ();

    while (1)
    {
        Wait_ms(1);
    }
}


// void TF_TIM1_CH2_PWM (void)
// {
//     TIM_1_Init();
//     Update_TIM1_CH2(240);    //duty 50%

//     while (1)
//     {
//         // if (PIN8)
//         //     PIN8_OFF;
//         // else
//         //     PIN8_ON;

//         Wait_ms(1);
//     }
// }


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
        // Update_TIM16_CH1N (V_Sense);
    }
    
}

// void TF_ENA_CH1_ENA_CH2_SW_SEL (void)
// {
//     // while (1)
//     // {
//     //     if (SW_SEL)
//     //     {
//     //         ENA_CH1_ON;
//     //         ENA_CH2_OFF;
//     //     }
//     //     else
//     //     {
//     //         ENA_CH1_OFF;
//     //         ENA_CH2_ON;
//     //     }
//     // }
//     ENA_CH1_ON;
//     ENA_CH2_ON;

//     while (1);
// }


// void TF_SW_UP (void)
// {
//     while (1)
//     {
//         if (SW_UP)
//             LED_ON;
//         else
//             LED_OFF;
//     }    
// }


// void TF_SW_SEL (void)
// {
//     while (1)
//     {
//         if (SW_SEL)
//             LED_ON;
//         else
//             LED_OFF;
//     }    
// }


// void TF_SW_DWN (void)
// {
//     while (1)
//     {
//         if (SW_DWN)
//             LED_ON;
//         else
//             LED_OFF;
//     }    
// }


// void TF_lcdE (void)
// {
//     while (1)
//     {
//         if (LCD_E)
//             LCD_E_OFF;
//         else
//             LCD_E_ON;

//         Wait_ms(10);
//     }
// }


// void TF_lcdRS (void)
// {
//     while (1)
//     {
//         if (LCD_RS)
//             LCD_RS_OFF;
//         else
//             LCD_RS_ON;

//         Wait_ms(10);
//     }
// }


// void TF_lcdBklight (void)
// {
//     while (1)
//     {
//         if (CTRL_BKL)
//             CTRL_BKL_OFF;
//         else
//             CTRL_BKL_ON;

//         Wait_ms(1000);
//     }
// }


// void TF_lcdData (void)
// {
//     while (1)
//     {
//         //pa0 a pa3
//         LED_ON;
//         GPIOA->BSRR = 0x0000000F;
//         Wait_ms(2000);
        
//         LED_OFF;
//         GPIOA->BSRR = 0x000F0000;
//         Wait_ms(2000);

//         LED_ON;
//         GPIOA->BSRR = 0x00000005;
//         Wait_ms(2000);

//         LED_OFF;
//         GPIOA->BSRR = 0x00050000;
//         Wait_ms(2000);

//         LED_ON;
//         GPIOA->BSRR = 0x0000000A;
//         Wait_ms(2000);

//         LED_OFF;
//         GPIOA->BSRR = 0x000A0000;
//         Wait_ms(2000);
//     }
// }


// void TF_lcdBlink (void)
// {
//     LCD_UtilsInit();
//     CTRL_BKL_ON;

//     while (1)
//     {
//         while (LCD_ShowBlink("Kirno Technology",
//                              "  Smart Driver  ",
//                              2,
//                              BLINK_DIRECT) != resp_finish);

//         LCD_ClearScreen();
//         Wait_ms(1000);
//     }
// }


// void TF_lcdScroll (void)
// {
//     resp_t resp = resp_continue;

//     LCD_UtilsInit();
//     CTRL_BKL_ON;
    
//     while (1)
//     {
//         // LCD_ClearScreen();
//         // Wait_ms(2000);
//         do {
//             resp = LCD_Scroll1 ("Dexel Lighting DMX 2 channels 8 amps controller.");
//         } while (resp != resp_finish);

//         Wait_ms(2000);
//     }
// }


// void TF_Dmx_Packet (void)
// {
//     Usart1Config();
//     TIM_14_Init();
//     DMX_channel_selected = 1;
//     DMX_channel_quantity = 2;
//     DMX_EnableRx();

//     while (1)
//     {
//         if (Packet_Detected_Flag)
//         {
//             Packet_Detected_Flag = 0;
//             LED_ON;
//             Wait_ms(2);
//             LED_OFF;
//         }
//     }
// }


// void TF_Dmx_Packet_Data (void)
// {
//     // Init LCD
//     LCD_UtilsInit();
//     CTRL_BKL_ON;
//     LCD_ClearScreen();
//     Wait_ms(1000);

//     // Init DMX
//     Usart1Config();
//     TIM_14_Init();
//     DMX_channel_selected = 1;
//     DMX_channel_quantity = 2;
//     DMX_EnableRx();

//     unsigned char dmx_data1 = 0;
//     unsigned char dmx_data2 = 0;    

//     while (1)
//     {
//         if (Packet_Detected_Flag)
//         {
//             Packet_Detected_Flag = 0;
//             LED_ON;

//             if (dmx_buff_data[0] == 0)
//             {
//                 char s_lcd [20] = { 0 };

//                 if (dmx_data1 != dmx_buff_data[1])
//                 {
//                     sprintf(s_lcd, "ch1: %03d", dmx_buff_data[1]);
//                     LCD_Writel1(s_lcd);
//                     dmx_data1 = dmx_buff_data[1];
//                 }

//                 if (dmx_data2 != dmx_buff_data[2])
//                 {
//                     sprintf(s_lcd, "ch2: %03d", dmx_buff_data[2]);
//                     LCD_Writel2(s_lcd);
//                     dmx_data2 = dmx_buff_data[2];
//                 }
//             }
            
//             LED_OFF;
//         }
//     }
// }


// void TF_Pwm_Channels (void)
// {
//     TIM_3_Init();

//     PWM_Update_CH1(DUTY_20_PERCENT);
//     PWM_Update_CH2(DUTY_50_PERCENT);
                   
//     while (1);
    
// }


// void TF_F_Channels_As_Pwm (void)
// {
// #ifdef USE_F_CHNLS_FOR_ENABLE
//     TIM_1_Init();

//     Update_TIM1_CH2(5);
//     Update_TIM1_CH4(500);
    
// #endif    
// }

// void TF_F_Channels_As_ICapture_2 (void)
// {
// #ifdef USE_F_CHNLS_FOR_FREQ_DETECT
//     TIM_1_Init();
//     ENA_CH1_ON;

//     while (1)
//     {
//         if (f_channel_2_int)
//         {
//             if (LED)
//                 LED_OFF;
//             else
//                 LED_ON;
            
//             f_channel_2_int = 0;
//         }
//     }
// #endif    
// }

// void TF_F_Channels_As_ICapture_4 (void)
// {
// #ifdef USE_F_CHNLS_FOR_FREQ_DETECT
//     TIM_1_Init();
//     ENA_CH2_ON;

//     while (1)
//     {
//         if (f_channel_4_int)
//         {
//             if (LED)
//                 LED_OFF;
//             else
//                 LED_ON;
            
//             f_channel_4_int = 0;
//         }
//     }
// #endif    
// }


// void TF_Temp_Channel (void)
// {
//     // Init LCD
//     LCD_UtilsInit();
//     CTRL_BKL_ON;
//     LCD_ClearScreen();
//     Wait_ms(1000);

//     // Init Tim
//     TIM_3_Init();

//     // Init ADC and DMA
//     AdcConfig();
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;
//     ADC1->CR |= ADC_CR_ADSTART;
    
//     char s_lcd [30] = { 0 };
//     unsigned char temp_degrees = 0;
//     while (1)
//     {
//         Wait_ms (500);
//         temp_degrees = Temp_TempToDegrees(Temp_Channel);
        
//         sprintf(s_lcd, "Ch: %04d T: %d",
//                 Temp_Channel,
//                 temp_degrees);

//         LCD_Writel1(s_lcd);

//         sprintf(s_lcd, "convert: %04d",
//                 Temp_DegreesToTemp(temp_degrees));

//         LCD_Writel2(s_lcd);
//     }
// }

//--- end of file ---//
