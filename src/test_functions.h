//------------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board #####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.H #########################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _TEST_FUNTIONS_H_
#define _TEST_FUNTIONS_H_

#include "hard.h"

// Exported Types Constants and Macros -----------------------------------------


// Exported Functions ----------------------------------------------------------
void TF_Led (void);
void TF_Control_Fan (void);
void TF_TIM16_Pwm_CH1N (void);
void TF_TIM14_Pwm_CH1 (void);

void TF_TIM3_CH1_ConstantOff_TIM3_CH2_TriggerInput (void);
void TF_TIM1_CH1_ConstantOff_TIM1_CH2_TriggerInput (void);

void TF_Temp_Channel (void);

#endif    /* _TEST_FUNTIONS_H_ */

//--- end of file ---//

