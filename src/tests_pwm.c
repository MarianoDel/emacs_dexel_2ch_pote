//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "pwm.h"


//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short soft_saved_pwm_ch1;
extern volatile unsigned short soft_saved_pwm_ch2;

extern volatile unsigned char soft_pwm_output_ch1;
extern volatile unsigned char soft_pwm_output_ch2;

extern volatile unsigned short soft_pwm_cnt;
extern volatile unsigned short soft_pwm_next;

extern volatile unsigned char edges;
// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Pwm_Set (void);
void Test_Pwm_Handler (void);
void Test_Pwm_Handler_Low_Freq (void);
    

// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    Test_Pwm_Set ();
    // Test_Pwm_Handler ();
    Test_Pwm_Handler_Low_Freq ();
}


void Test_Pwm_Handler_Low_Freq (void)
{
    int error = 0;
    int posi = 0;
    int setting = 0;
    
    printf("Test soft pwm handler no output: ");
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    for (int i = 0; i < 512; i++)    //two turns
    {
        PWM_Soft_Handler_Low_Freq();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d\n", posi);
    }
    else
        PrintOK();


    printf("Test soft pwm handler output ch1: ");
    PWM_Soft_Set_Channels (2, 0);

    for (int j = 0; j <= 255; j++)
    {
        PWM_Soft_Set_Channels (1, j);

        for (int i = 0; i < 512; i++)    // two turns
        {
            PWM_Soft_Handler_Low_Freq();

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();


    printf("Test soft pwm handler output ch2: ");
    PWM_Soft_Set_Channels (1, 0);

    for (int j = 0; j <= 255; j++)
    {
        PWM_Soft_Set_Channels (2, j);

        for (int i = 0; i < 512; i++)    // two turns
        {
            PWM_Soft_Handler_Low_Freq();

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();
    
}


void Test_Pwm_Handler (void)
{
    int error = 0;
    int posi = 0;
    
    printf("Test soft pwm handler no output: ");
    PWM_Soft_Set_Channels (1, 0);
    PWM_Soft_Set_Channels (2, 0);

    for (int i = 0; i < 0xffff; i++)
    {
        PWM_Soft_Handler();
        if ((soft_pwm_output_ch1) ||
            (soft_pwm_output_ch2))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();


    int setting = 0;
    printf("Test soft pwm handler output ch1: ");
    PWM_Soft_Set_Channels (2, 0);

    for (int j = 0; j < 255; j++)
    {
        PWM_Soft_Set_Channels (1, j);

        soft_pwm_next = 0;
        edges = 0;
        for (int i = 0; i < 4095; i++)
        {
            soft_pwm_cnt = i;

            if (soft_pwm_cnt == soft_pwm_next)
                PWM_Soft_Handler();

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch1)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();


    printf("Test soft pwm handler output ch2: ");
    PWM_Soft_Set_Channels (1, 0);

    for (int j = 0; j < 255; j++)
    {
        PWM_Soft_Set_Channels (2, j);

        soft_pwm_next = 0;
        edges = 0;
        for (int i = 0; i < 4095; i++)
        {
            soft_pwm_cnt = i;

            if (soft_pwm_cnt == soft_pwm_next)
                PWM_Soft_Handler();

            if (soft_pwm_cnt < j)    // check one output
            {
                if (!soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;                    
                    break;
                }
            }
            else    //check zero output
            {
                if (soft_pwm_output_ch2)
                {
                    error = 1;
                    posi = i;
                    setting = j;
                    j = 255;
                    break;
                }
            }
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on counter: %d with setting: %d\n", posi, setting);
    }
    else
        PrintOK();
    
    
    
    
}


void Test_Pwm_Set (void)
{
    int error = 0;
    int posi = 0;

    printf("Test soft pwm settings on ch1: ");
    PWM_Soft_Set_Channels (2, 0);
    for (int i = 0; i < 65536; i++)
    {
        PWM_Soft_Set_Channels (1, i);
        if ((soft_saved_pwm_ch2 != 0) ||
            (soft_saved_pwm_ch1 != i))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();

    printf("Test soft pwm settings on ch2: ");
    PWM_Soft_Set_Channels (1, 0);
    for (int i = 0; i < 65536; i++)
    {
        PWM_Soft_Set_Channels (2, i);
        if ((soft_saved_pwm_ch1 != 0) ||
            (soft_saved_pwm_ch2 != i))
        {
            error = 1;
            posi = i;
            break;
        }
    }

    if (error)
    {
        PrintERR();
        printf(" error on value: %d\n", posi);            
    }
    else
        PrintOK();
    
}



// Module Mocked Functions
void Update_TIM3_CH1 (unsigned short a)
{
    printf("tim3 ch1 set to: %d\n", a);
}


void Update_TIM3_CH2 (unsigned short a)
{
    printf("tim3 ch2 set to: %d\n", a);
}



//--- end of file ---//


