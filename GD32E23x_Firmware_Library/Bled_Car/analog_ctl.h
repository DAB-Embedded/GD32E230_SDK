/*!
    \file    analog_ctl.c
    \brief   Analog input control module header

    \description Header for analog input module
*/

#ifndef ANALOG_CTL__H
#define ANALOG_CTL__H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32e23x.h"
#include "main.h"
#include <stdio.h>

void     analog_ctl_init(void);
uint16_t analog_ctl_get_adc_battery(void);
uint16_t analog_ctl_get_adc_mic(void);

#ifdef __cplusplus
}
#endif

#endif /* ANALOG_CTL__H */
