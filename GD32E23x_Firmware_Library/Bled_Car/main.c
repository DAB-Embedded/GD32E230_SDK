/*!
    \file    main.c
    \brief   Police car beacon firmware

    \version 2019-02-19, V1.0.0, firmware for GD32E23x
    \version 2020-12-12, V1.1.0, firmware for GD32E23x
*/

/*
    Made by DAB-Embedded, 2023
*/

#include <stdio.h>
#include "gd32e23x.h"
#include "systick.h"
#include "main.h"
#include "analog_ctl.h"
#include "gpio_ctl.h"
#include "video_ctl.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* configure systick */
    systick_config();
    /* enable clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* initilize the LEDs, ADC and key */
    gpio_ctl_init();
    gpio_booster_en(1);
    analog_ctl_init();

    while(1){
    	bs_entry();
    }
}
