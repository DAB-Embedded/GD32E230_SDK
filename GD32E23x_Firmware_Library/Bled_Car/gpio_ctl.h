/*!
    \file    gpio_ctl.c
    \brief   GPIO control module header

    \description Header for GPIO module
*/

#ifndef GPIO_CTL__H
#define GPIO_CTL__H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32e23x.h"
#include "main.h"


#define LEDS_TOTAL                  (6)
#define LEDS_RGB_TOTAL              (LEDS_TOTAL * 3)

#define MPU_WK0_PORT           		GPIOA
#define MPU_WK0_PIN			   		GPIO_PIN_0
#define MPU_WK0_RCU			   		RCU_GPIOA
#define CPU_BAT_PWR_EN_PORT    		GPIOB
#define CPU_BAT_PWR_EN_PIN     		GPIO_PIN_2
#define CPU_BAT_PWR_EN_RCU	   		RCU_GPIOB
#define MIC_EN_PORT			    	GPIOB
#define MIC_EN_PIN					GPIO_PIN_6
#define MIC_EN_RCU					RCU_GPIOB

typedef struct cl_lamp_led {
    uint32_t        port;
    uint32_t        pin;
    rcu_periph_enum rcu_gpio;
} cl_lamp_led_s;

typedef struct cl_lamp {
    cl_lamp_led_s   red;
    cl_lamp_led_s   green;
    cl_lamp_led_s   blue;
} cl_lamp_s;

typedef struct cl_lamp_flat {
    cl_lamp_led_s   lamps[LEDS_RGB_TOTAL];
} cl_lamp_flat_s;

extern const cl_lamp_led_s lamps_flat[];

void gpio_ctl_init(void);
void gpio_ctl_pre_standby(void);
void gpio_lamp_control(int num, int r, int g, int b);
void gpio_mic_en(int enable);
void gpio_booster_en(int enable);
int  gpio_get_button_status(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_CTL__H */
