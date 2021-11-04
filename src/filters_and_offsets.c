//----------------------------------------------------
// #### DEXEL 2CH PROJECT - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### FILTERS_AND_OFFSETS.C ########################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "filters_and_offsets.h"
#include "dsp.h"
#include "pwm.h"
#include "tim.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma32_u16_data_obj_t st_ch1;
ma32_u16_data_obj_t st_ch2;


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

// Funcion original
typedef enum {
    FILTERS_FILTER_EACH_CHANNEL,
    FILTERS_PROCESS_CHANNELS,
    FILTERS_MAPPING,
    FILTERS_DUMMY1,
    FILTERS_DUMMY2
    
} filters_and_offsets_e;


// unsigned char ch1_enable_state = 0;
// unsigned char ch2_enable_state = 0;
unsigned short ch1_input_filtered = 0;
unsigned short ch2_input_filtered = 0;

filters_and_offsets_e filters_sm = FILTERS_FILTER_EACH_CHANNEL;
unsigned short pwm_output [2] = { 0 };
unsigned short ena_output [2] = { 0 };

void FiltersAndOffsets_Post_Mapping_SM (unsigned short new_ch1,
                                        unsigned short new_ch2)
{
    unsigned int calc = 0;
    unsigned short bright = 0;
    unsigned short temp0 = 0;
    unsigned short temp1 = 0;

    unsigned short ch1_pwm, ch2_pwm, ena1_pwm, ena2_pwm;
    
    switch (filters_sm)
    {
    case FILTERS_FILTER_EACH_CHANNEL:
        ch1_input_filtered = MA32_U16Circular (&st_ch1, new_ch1);
        ch2_input_filtered = MA32_U16Circular (&st_ch2, new_ch2);

        filters_sm++;
        break;

    case FILTERS_PROCESS_CHANNELS:
        // backup and bright temp calcs
        // ch1 the bright ch2 the temp
        bright = ch1_input_filtered;
        temp0 = 4095 - ch2_input_filtered;
        temp1 = 4095 - temp0;
        
        calc = temp0 * bright;
        calc >>= 12;    // to 4095
        pwm_output[0] = (unsigned short) calc;
        
        calc = temp1 * bright;
        calc >>= 12;    // to 4095
        pwm_output[1] = (unsigned short) calc;

        filters_sm++;
        break;

    case FILTERS_MAPPING:
        PWM_Map_Post_Filter (pwm_output[0],
                            &ena1_pwm,
                            &ch1_pwm);

        ena1_pwm >>= 4;    //255 max
        PWM_Update_ENA1(ena1_pwm);
        PWM_Update_CH1(ch1_pwm);

        PWM_Map_Post_Filter (pwm_output[1],
                            &ena2_pwm,
                            &ch2_pwm);

        ena2_pwm >>= 4;    //255 max        
        PWM_Update_ENA2(ena2_pwm);
        PWM_Update_CH2(ch2_pwm);
        
        filters_sm++;
        break;

    case FILTERS_DUMMY1:
        filters_sm++;
        break;

    case FILTERS_DUMMY2:
        filters_sm = FILTERS_FILTER_EACH_CHANNEL;
        break;
        
    default:
        filters_sm = FILTERS_FILTER_EACH_CHANNEL;
        break;
    }
}


void FiltersAndOffsets_Filters_Reset (void)
{
    MA32_U16Circular_Reset(&st_ch1);
    MA32_U16Circular_Reset(&st_ch2);
}



//--- end of file ---//
