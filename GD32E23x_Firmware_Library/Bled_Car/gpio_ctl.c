/*!
    \file    gpio_ctl.c
    \brief   GPIO control module

    \description Module for GPIO operation handling
*/

#include "gpio_ctl.h"

#define ARRAY_SIZE(x)   			(sizeof(x)/sizeof(x[0]))

const cl_lamp_s lamps[LEDS_TOTAL] = {
  { /* LED2 */
     {GPIOF, GPIO_PIN_0,  RCU_GPIOF},
     {GPIOF, GPIO_PIN_1,  RCU_GPIOF},
     {GPIOA, GPIO_PIN_3,  RCU_GPIOA},
  },
  { /* LED1 */
     {GPIOA, GPIO_PIN_5,  RCU_GPIOA},
     {GPIOA, GPIO_PIN_4,  RCU_GPIOA},
     {GPIOA, GPIO_PIN_6,  RCU_GPIOA},
  },
  { /* LED3 */
     {GPIOB, GPIO_PIN_0,  RCU_GPIOB},
     {GPIOA, GPIO_PIN_7,  RCU_GPIOA},
     {GPIOB, GPIO_PIN_1,  RCU_GPIOB},
  },
  { /* LED6 */
     {GPIOA, GPIO_PIN_9,  RCU_GPIOA},
     {GPIOA, GPIO_PIN_8,  RCU_GPIOA},
     {GPIOA, GPIO_PIN_10, RCU_GPIOA},
  },
  { /* LED5 */
     {GPIOA, GPIO_PIN_12, RCU_GPIOA},
     {GPIOA, GPIO_PIN_11, RCU_GPIOA},
     {GPIOA, GPIO_PIN_15, RCU_GPIOA},
  },
  { /* LED4 */
     {GPIOB, GPIO_PIN_4,  RCU_GPIOB},
     {GPIOB, GPIO_PIN_3,  RCU_GPIOB},
     {GPIOB, GPIO_PIN_5,  RCU_GPIOB},
  },
};

const cl_lamp_led_s lamps_flat[LEDS_RGB_TOTAL] =
{
  /* LED2 */
  {GPIOF, GPIO_PIN_0},
  {GPIOF, GPIO_PIN_1},
  {GPIOA, GPIO_PIN_3},
  /* LED1 */
  {GPIOA, GPIO_PIN_5},
  {GPIOA, GPIO_PIN_4},
  {GPIOA, GPIO_PIN_6},
  /* LED3 */
  {GPIOB, GPIO_PIN_0},
  {GPIOA, GPIO_PIN_7},
  {GPIOB, GPIO_PIN_1},
  /* LED6 */
  {GPIOA, GPIO_PIN_9},
  {GPIOA, GPIO_PIN_8},
  {GPIOA, GPIO_PIN_10},
  /* LED5 */
  {GPIOA, GPIO_PIN_12},
  {GPIOA, GPIO_PIN_11},
  {GPIOA, GPIO_PIN_15},
  /* LED4 */
  {GPIOB, GPIO_PIN_4},
  {GPIOB, GPIO_PIN_3},
  {GPIOB, GPIO_PIN_5},
};

/*!
    \brief      Initialize GPIO for normal mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_ctl_init(void)
{
	int i;

	/* Initialize GPIO for lamps */
	for (i = 0; i < LEDS_TOTAL; i++)
	{
	    /* enable the led clock */
	    rcu_periph_clock_enable(lamps[i].red.rcu_gpio);
	    rcu_periph_clock_enable(lamps[i].green.rcu_gpio);
	    rcu_periph_clock_enable(lamps[i].blue.rcu_gpio);
	    /* configure led GPIO port */
	    gpio_bit_set(lamps[i].red.port,   lamps[i].red.pin);
	    gpio_bit_set(lamps[i].green.port, lamps[i].green.pin);
	    gpio_bit_set(lamps[i].blue.port,  lamps[i].blue.pin);
	    gpio_mode_set(lamps[i].red.port,
	    		GPIO_MODE_OUTPUT,
				GPIO_PUPD_NONE,
				lamps[i].red.pin);
	    gpio_output_options_set(lamps[i].red.port,
	    		GPIO_OTYPE_OD,
				GPIO_OSPEED_50MHZ,
				lamps[i].red.pin);
	    gpio_mode_set(lamps[i].green.port,
	    		GPIO_MODE_OUTPUT,
				GPIO_PUPD_NONE,
				lamps[i].green.pin);
	    gpio_output_options_set(lamps[i].green.port,
	    		GPIO_OTYPE_OD,
				GPIO_OSPEED_50MHZ,
				lamps[i].green.pin);
	    gpio_mode_set(lamps[i].blue.port,
	    		GPIO_MODE_OUTPUT,
				GPIO_PUPD_NONE,
				lamps[i].blue.pin);
	    gpio_output_options_set(lamps[i].blue.port,
	    		GPIO_OTYPE_OD,
				GPIO_OSPEED_50MHZ,
				lamps[i].blue.pin);
	}

	/* Initialize button input pin */
	rcu_periph_clock_enable(MPU_WK0_RCU);
	rcu_periph_clock_enable(RCU_CFGCMP);
    gpio_mode_set(MPU_WK0_PORT,
    		GPIO_MODE_INPUT,
			GPIO_PUPD_NONE,
			MPU_WK0_PIN);

    /* Clear all related wakeup flags */
	pmu_flag_clear(PMU_FLAG_RESET_WAKEUP);

    /* Disable wakeup pin WKUP0 */
    pmu_wakeup_pin_disable(PMU_WAKEUP_PIN0);

    /* enable and set key EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI0_1_IRQn, 2U);
    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN0);

    /* configure key EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(EXTI_0);
    exti_interrupt_enable(EXTI_0);

	/* Initialize CPU_BAT_PWR_EN output */
	rcu_periph_clock_enable(CPU_BAT_PWR_EN_RCU);
    gpio_bit_reset(CPU_BAT_PWR_EN_PORT, CPU_BAT_PWR_EN_PIN);
    gpio_mode_set(CPU_BAT_PWR_EN_PORT,
    		GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE,
			CPU_BAT_PWR_EN_PIN);
    gpio_output_options_set(CPU_BAT_PWR_EN_PORT,
    		GPIO_OTYPE_PP,
			GPIO_OSPEED_50MHZ,
			CPU_BAT_PWR_EN_PIN);

	/* Initialize MIC_EN output */
	rcu_periph_clock_enable(MIC_EN_RCU);
    gpio_bit_reset(MIC_EN_PORT, MIC_EN_PIN);
    gpio_mode_set(MIC_EN_PORT,
    		GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE,
			MIC_EN_PIN);
    gpio_output_options_set(MIC_EN_PORT,
    		GPIO_OTYPE_PP,
			GPIO_OSPEED_50MHZ,
			MIC_EN_PIN);

	/* Initialize CPU_A0 input */
	rcu_periph_clock_enable(RCU_GPIOA);
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
    gpio_mode_set(GPIOA,
    		GPIO_MODE_ANALOG,
			GPIO_PUPD_NONE,
			GPIO_PIN_1);

	/* Initialize ADC_MIC_IN input */
	rcu_periph_clock_enable(RCU_GPIOA);
    gpio_bit_reset(GPIOA, GPIO_PIN_2);
    gpio_mode_set(GPIOA,
    		GPIO_MODE_ANALOG,
			GPIO_PUPD_NONE,
			GPIO_PIN_2);
}

/*!
    \brief      Prepare GPIO for standby mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_ctl_pre_standby(void)
{
	int i;

	/* Initialize GPIO for lamps */
	for (i = 0; i < LEDS_TOTAL; i++)
	{
	    /* configure led GPIO port */
	    gpio_mode_set(lamps[i].red.port,
	    		GPIO_MODE_ANALOG,
				GPIO_PUPD_NONE,
				lamps[i].red.pin);
	    gpio_mode_set(lamps[i].green.port,
	    		GPIO_MODE_ANALOG,
				GPIO_PUPD_NONE,
				lamps[i].green.pin);
	    gpio_mode_set(lamps[i].blue.port,
	    		GPIO_MODE_ANALOG,
				GPIO_PUPD_NONE,
				lamps[i].blue.pin);
	}

    gpio_mode_set(MIC_EN_PORT,
    		GPIO_MODE_ANALOG,
			GPIO_PUPD_NONE,
			MIC_EN_PIN);

    exti_interrupt_disable(EXTI_0);
    exti_interrupt_flag_clear(EXTI_0);
    nvic_irq_disable(EXTI0_1_IRQn);
}

/*!
    \brief      Turn on/off RGB lamps
    \param[in]  num	 - RGB lamp number (0..5)
    \param[in]  r	 - red lamp: 0 - off, 1 - on
    \param[in]  g	 - green lamp: 0 - off, 1 - on
    \param[in]  b	 - blue lamp: 0 - off, 1 - on
    \param[out] none
    \retval     none
*/
void gpio_lamp_control(int num, int r, int g, int b)
{
    if (num >= LEDS_TOTAL) num = LEDS_TOTAL - 1;

    gpio_bit_write(lamps[num].red.port,
            lamps[num].red.pin, r ? RESET : SET);

    gpio_bit_write(lamps[num].green.port,
            lamps[num].green.pin, g ? RESET : SET);

    gpio_bit_write(lamps[num].blue.port,
            lamps[num].blue.pin, b ? RESET : SET);
}

/*!
    \brief      Microphone power control
    \param[in]  enable - 0 means off, 1 means on
    \param[out] none
    \retval     none
*/
void gpio_mic_en(int enable)
{
    gpio_bit_write(MIC_EN_PORT,
    		MIC_EN_PIN, enable ? SET : RESET);
}

/*!
    \brief      Power booster control
    \param[in]  enable - 0 means off, 1 means on
    \param[out] none
    \retval     none
*/
void gpio_booster_en(int enable)
{
    gpio_bit_write(CPU_BAT_PWR_EN_PORT,
    		CPU_BAT_PWR_EN_PIN, enable ? SET : RESET);
}

/*!
    \brief      Get button status
    \param[in]  none
    \param[out] none
    \retval     1 - button pressed, 0 - button released
*/
int gpio_get_button_status(void)
{
	return gpio_input_bit_get(MPU_WK0_PORT, MPU_WK0_PIN) == SET ? 1 : 0;
}
