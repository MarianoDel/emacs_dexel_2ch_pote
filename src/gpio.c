//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### GPIO.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "gpio.h"
#include "stm32f0xx.h"
#include "hard.h"

//--- Defines for Configuration ---------------------------------
#define GPIOA_ENABLE
#define GPIOB_ENABLE
// #define GPIOC_ENABLE
// #define GPIOF_ENABLE

// #define WITH_EXTI
//--- End of Defines for Configuration --------------------------

// Private Types Constants and Macros ------------------------------------------
#define GPIOA_CLK    (RCC->AHBENR & 0x00020000)
#define GPIOA_CLK_ON    (RCC->AHBENR |= 0x00020000)
#define GPIOA_CLK_OFF    (RCC->AHBENR &= ~0x00020000)

#define GPIOB_CLK    (RCC->AHBENR & 0x00040000)
#define GPIOB_CLK_ON    (RCC->AHBENR |= 0x00040000)
#define GPIOB_CLK_OFF    (RCC->AHBENR &= ~0x00040000)

#define GPIOC_CLK    (RCC->AHBENR & 0x00080000)
#define GPIOC_CLK_ON    (RCC->AHBENR |= 0x00080000)
#define GPIOC_CLK_OFF    (RCC->AHBENR &= ~0x00080000)

#define GPIOD_CLK    (RCC->AHBENR & 0x00100000)
#define GPIOD_CLK_ON    (RCC->AHBENR |= 0x00100000)
#define GPIOD_CLK_OFF    (RCC->AHBENR &= ~0x00100000)

#define GPIOF_CLK    (RCC->AHBENR & 0x00400000)
#define GPIOF_CLK_ON    (RCC->AHBENR |= 0x00400000)
#define GPIOF_CLK_OFF    (RCC->AHBENR &= ~0x00400000)

#define SYSCFG_CLK    (RCC->APB2ENR & 0x00000001)
#define SYSCFG_CLK_ON    (RCC->APB2ENR |= 0x00000001)
#define SYSCFG_CLK_OFF    (RCC->APB2ENR &= ~0x00000001)


// Module Functions ------------------------------------------------------------
void GPIO_Config (void)
{
    unsigned long temp;

    //--- MODER ---//
    //00: Input mode (reset state)
    //01: General purpose output mode
    //10: Alternate function mode
    //11: Analog mode

    //--- OTYPER ---//
    //These bits are written by software to configure the I/O output type.
    //0: Output push-pull (reset state)
    //1: Output open-drain

    //--- ORSPEEDR ---//
    //These bits are written by software to configure the I/O output speed.
    //x0: Low speed.
    //01: Medium speed.
    //11: High speed.
    //Note: Refer to the device datasheet for the frequency.

    //--- PUPDR ---//
    //These bits are written by software to configure the I/O pull-up or pull-down
    //00: No pull-up, pull-down
    //01: Pull-up
    //10: Pull-down
    //11: Reserved

#ifdef GPIOA_ENABLE
    //--- GPIO A ---//
    if (!GPIOA_CLK)
        GPIOA_CLK_ON;

#ifdef HARD_VER_2_2
    temp = GPIOA->MODER;    //2 bits por pin
    temp &= 0xFCFF3CFC;    // PA0 analog; PA4 analog; PA7 alternative
    temp |= 0x01008303;    // PA8 - PA9 starts default go to alternative; PA12 out
    GPIOA->MODER = temp;
#endif
#ifdef HARD_VER_2_1
    temp = GPIOA->MODER;    //2 bits por pin
    temp &= 0xFCFF3CFF;    // PA4 analog; PA7 alternative
    temp |= 0x01008300;    // PA8 - PA9 alternative; PA12 out
    GPIOA->MODER = temp;
#endif

    temp = GPIOA->OTYPER;    //1 bit por pin
    temp &= 0xFFFFFEFF;    //PA8 open drain
    temp |= 0x00000100;
    GPIOA->OTYPER = temp;
    
    temp = GPIOA->OSPEEDR;	//2 bits por pin
    temp &= 0xFCF03FFF;    
    temp |= 0x00000000;    //PA7 PA8 PA9 PA12 low speed
    GPIOA->OSPEEDR = temp;

    temp = GPIOA->PUPDR;	//2 bits por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOA->PUPDR = temp;
    
#endif

#ifdef GPIOB_ENABLE

    //--- GPIO B ---//
    if (!GPIOB_CLK)
        GPIOB_CLK_ON;

    temp = GPIOB->MODER;    //2 bits por pin
    temp &= 0xFFFFCF30;    // PB0 PB1 analog input; PB3 out; PB4 PB5 starts default go to alt; PB6 alternative
    temp |= 0x0000204F;
    GPIOB->MODER = temp;

    temp = GPIOB->OTYPER;	//1 bit por pin
    temp &= 0xFFFFFFEF;    // PB4 open drain
    temp |= 0x00000010;
    GPIOB->OTYPER = temp;

    temp = GPIOB->OSPEEDR;	//2 bits por pin
    temp &= 0xFFFFC03F;
    temp |= 0x00000000;    // PB3, PB4 - PB6 low speed
    GPIOB->OSPEEDR = temp;

    temp = GPIOB->PUPDR;	//2 bits por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOB->PUPDR = temp;

#endif

#ifdef GPIOF_ENABLE

    //--- GPIO F ---//
    if (!GPIOF_CLK)
        GPIOF_CLK_ON;

    temp = GPIOF->MODER;
    temp &= 0xFFFFFFF0;
    temp |= 0x00000005;
    GPIOF->MODER = temp;

    temp = GPIOF->OTYPER;
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOF->OTYPER = temp;

    temp = GPIOF->OSPEEDR;
    temp &= 0xFFFFFFF0;
    temp |= 0x00000000;
    GPIOF->OSPEEDR = temp;

    temp = GPIOF->PUPDR;
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOF->PUPDR = temp;

#endif

#ifdef WITH_EXTI
    //Interrupt on PA8
    if (!SYSCFG_CLK)
        SYSCFG_CLK_ON;

    SYSCFG->EXTICR[0] = 0x00000000; //Select Port A
    SYSCFG->EXTICR[1] = 0x00000000; //Select Port A
    EXTI->IMR |= 0x0100;    //Corresponding mask bit for interrupts PA8
    EXTI->EMR |= 0x0000;    //Corresponding mask bit for events
    EXTI->RTSR |= 0x0100;    //Pin Interrupt line on rising edge PA8
    EXTI->FTSR |= 0x0100;    //Pin Interrupt line on falling edge PA8

    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 3);
#endif
    
    
}

inline void EXTIOff (void)
{
	EXTI->IMR &= ~0x00000100;
}

inline void EXTIOn (void)
{
	EXTI->IMR |= 0x00000100;
}

//--- end of file ---//
