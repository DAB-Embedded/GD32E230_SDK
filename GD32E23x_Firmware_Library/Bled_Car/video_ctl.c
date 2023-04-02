/*!
    \file    video_ctl.c
    \brief   Video control module

    \description Module for video operation handling
*/

#include "analog_ctl.h"
#include "gpio_ctl.h"
#include "video_ctl.h"
#include "systick.h"

#define ARRAY_SIZE(x)               (sizeof(x)/sizeof(x[0]))
#define TIM2_PRESCALER              (300)
#define TIM2_PERIOD                 (550)

#define DEV_TIMEOUT_TOT             (60000)

#define DEMO_MODE                   (0)
#define DEMO_SW_TIME                (3000)

#define BTN_TIME_MENU               (7000)
#define BTN_TIME_SLEEP              (1500)
#define BTN_TIME_CHANGE_VIDEO       (1000)

#define VS_CMD_LED_END              (0x00)
#define VS_CMD_LED_SET              (0x01)
#define VS_CMD_LED_CLR              (0x02)
#define VS_CMD_LED_CNG              (0x03)

#define AUDIO_BUFFER_SZ             (1024)

const cmd_lamp_s police_light[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000100100100001001001},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},

  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000001001001100100100},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},

};
const size_t police_light_sz = ARRAY_SIZE(police_light);

const cmd_lamp_s road_srv_light[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000011011011},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000011011011000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000000},
};
const size_t road_srv_light_sz = ARRAY_SIZE(road_srv_light);

const cmd_lamp_s old_police_light[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000001000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000001000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000001000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000001000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000001000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000001000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000001000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000001000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000001000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000001000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000001000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000001000000001000000},
};
const size_t old_police_light_sz = ARRAY_SIZE(old_police_light);

const cmd_lamp_s old_police_light2[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000100000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000100000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000100000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000000100000000001},
  {VS_CMD_LED_CNG, 0b00000000000000000100000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000100000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000100000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000000100000000001000},
  {VS_CMD_LED_CNG, 0b00000000000000100000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000100000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000100000000001000000},
  {VS_CMD_LED_CNG, 0b00000000000000100000000001000000},
};
const size_t old_police_light2_sz = ARRAY_SIZE(old_police_light2);

const cmd_lamp_s green_peace_light[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000010000000},
};
const size_t green_peace_light_sz = ARRAY_SIZE(green_peace_light);

const cmd_lamp_s snow_team_light[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000111000000},
};
const size_t snow_team_light_sz = ARRAY_SIZE(snow_team_light);

const cmd_lamp_s snow_team_light2[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000000000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000000000000},
};
const size_t snow_team_light2_sz = ARRAY_SIZE(snow_team_light2);

const cmd_lamp_s snow_team_light3[] = {
  /*                               L06L05L04L03L02L01          */
  /*                               bgrbgrbgrbgrbgrbgr          */
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000000010000000111},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000000010000000111000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000010000000111000000},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000000111000000010},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000000111000000010000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
  {VS_CMD_LED_CNG, 0b00000000000000111000000010000000},
};
const size_t snow_team_light3_sz = ARRAY_SIZE(snow_team_light3);

const video_list_s movies[] =
{
  {police_light,        police_light_sz},
  {road_srv_light,      road_srv_light_sz},
  {old_police_light,    old_police_light_sz},
  {old_police_light2,   old_police_light2_sz},
  {green_peace_light,   green_peace_light_sz},
  {snow_team_light,     snow_team_light_sz},
  {snow_team_light2,    snow_team_light2_sz},
  {snow_team_light3,    snow_team_light3_sz},
};
const size_t movies_sz = ARRAY_SIZE(movies);

static int    button_state      = 0;
static size_t array_index_ptr   = 0;
static cmd_lamp_s * g_pseq      = NULL;
static size_t g_pseq_sz         = 0;
static uint32_t button_time     = 0;
static uint32_t current_video   = 0;
static uint32_t dev_timeout_val = 0;

static uint16_t audio_buf[AUDIO_BUFFER_SZ];
static uint32_t audio_ptr       = 0;

void video_init(void)
{
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER2);

    /* deinit a TIMER */
    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = TIM2_PRESCALER;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = TIM2_PERIOD;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER2, &timer_initpara);
}

void video_setup(const cmd_lamp_s * pseq, size_t array_sz)
{
	nvic_irq_disable(TIMER2_IRQn);
    /* disable a TIMER */
    timer_disable(TIMER2);
    /* disable the TIMER interrupt */
    timer_interrupt_disable(TIMER2, TIMER_INT_UP);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);

    g_pseq          = (cmd_lamp_s *)pseq;
    g_pseq_sz       = array_sz;
    array_index_ptr = 0;

    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    /* enable a TIMER */
    timer_enable(TIMER2);
    /* enable a IRQ */
    nvic_irq_enable(TIMER2_IRQn, 0);
}

void put_dev_into_suspend(void)
{
    RTC_BKP0 = current_video;

    /* Clear all related wakeup flags */
    pmu_flag_clear(PMU_FLAG_RESET_WAKEUP);

    /* Enable wakeup pin WKUP0 */
    pmu_wakeup_pin_enable(PMU_WAKEUP_PIN0);

    /* Clear all related wakeup flags */
    pmu_flag_clear(PMU_FLAG_RESET_WAKEUP);

    /* Prepare all IOs for standby mode - power reduction */
    gpio_ctl_pre_standby();
    gpio_booster_en(0);
    gpio_mic_en(0);

    /* Enter the Standby mode */
    pmu_to_standbymode();

    while(1);
}

void clear_timeout_dev(void)
{
    dev_timeout_val = get_systick();
}

void check_timeout_dev(void)
{
#if DEMO_MODE==0
    if ((get_systick() - dev_timeout_val) > DEV_TIMEOUT_TOT)
    {
        put_dev_into_suspend();
    }
#else
    if ((get_systick() - dev_timeout_val) > DEMO_SW_TIME)
    {
        clear_timeout_dev();
        current_video++;
        if (current_video >= movies_sz)
        {
            current_video = 0;
        }
        video_setup(movies[current_video].video, movies[current_video].video_sz);
    }
#endif
}

void check_button(void)
{
    if ((gpio_get_button_status() != 0) && (button_state == 0))
    {
            if ((gpio_get_button_status() != 0) && (button_state == 0))
            {
                    button_state = 1;
                    button_time = get_systick();
            }
    }

    if ((gpio_get_button_status() == 0) && (button_state == 1))
    {
            if ((gpio_get_button_status() == 0) && (button_state == 1))
            {
                    button_state = 0;
                    button_time = get_systick() - button_time;

                    clear_timeout_dev();

                    if (button_time > BTN_TIME_MENU)
                    {
                            /* Menu mode */
                    } else
                    if (button_time > BTN_TIME_SLEEP)
                    {
                            put_dev_into_suspend();
                    } else
                    if (button_time < BTN_TIME_CHANGE_VIDEO)
                    {
                            current_video++;
                            if (current_video >= movies_sz)
                            {
                                    current_video = 0;
                            }
                            video_setup(movies[current_video].video, movies[current_video].video_sz);
                    }
            }
    }
}

void video_ctl_timer_event(void)
{
    size_t az = g_pseq_sz;
    cmd_lamp_s * pseq = (cmd_lamp_s *)g_pseq;
    size_t j = 0;

        if (pseq[array_index_ptr].cmd == VS_CMD_LED_CNG)
        {
                for (j = 0; j < LEDS_RGB_TOTAL; j++)
                {
                        if (pseq[array_index_ptr].value & (1 << j))
                                GPIO_BC(lamps_flat[j].port) = (uint32_t)lamps_flat[j].pin;
                        else
                                GPIO_BOP(lamps_flat[j].port) = (uint32_t)lamps_flat[j].pin;
                }
        }

        if (++array_index_ptr >= az)
        {
                array_index_ptr = 0;
        }

        check_timeout_dev();
}

void video_audio_handler(void)
{
    uint16_t max_audio = 0, current_sig, avg_avalue = 0, ampl = 0;
    uint32_t acc_val = 0;

    audio_buf[audio_ptr++] = current_sig = analog_ctl_get_adc_mic();
    acc_val += current_sig;
    if (max_audio < current_sig) max_audio = current_sig;

    if (audio_ptr >= AUDIO_BUFFER_SZ) {
            audio_ptr = 0;
            avg_avalue = acc_val / AUDIO_BUFFER_SZ;
            ampl = max_audio - avg_avalue;
            if (ampl > 15)
            {
                    gpio_lamp_control(0, 0, 1, 0);
                    gpio_lamp_control(1, 0, 1, 0);
                    gpio_lamp_control(2, 0, 1, 0);
            } else
                    if (ampl > 10)
                    {
                            gpio_lamp_control(0, 0, 1, 0);
                            gpio_lamp_control(1, 0, 1, 0);
                            gpio_lamp_control(2, 0, 0, 0);
                    } else
                            if (ampl > 5)
                            {
                                    gpio_lamp_control(0, 0, 1, 0);
                                    gpio_lamp_control(1, 0, 0, 0);
                                    gpio_lamp_control(2, 0, 0, 0);
                            } else {
                                    gpio_lamp_control(0, 0, 0, 0);
                                    gpio_lamp_control(1, 0, 0, 0);
                                    gpio_lamp_control(2, 0, 0, 0);
                            }

            max_audio = 0;
            acc_val = 0;
    }
}

void bs_entry(void)
{
    /* Allow bkp registers access */
    pmu_backup_write_enable();
    /* Start video timer */
    video_init();
    /* Enable microphone */
    //gpio_mic_en(1);

    button_state    = 0;
    array_index_ptr = 0;
    g_pseq          = NULL;
    g_pseq_sz       = 0;
    button_time     = 0;
    current_video   = RTC_BKP0;
    audio_ptr       = 0;

    clear_timeout_dev();

    video_setup(movies[current_video].video, movies[current_video].video_sz);

    while(1)
    {
        __WFI();
    }
}
