/*!
    \file    video_ctl.c
    \brief   Video control module header

    \description Header for video module
*/

#ifndef VIDEO_CTL__H
#define VIDEO_CTL__H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32e23x.h"
#include "main.h"
#include <stdio.h>

typedef struct cmd_lamp {
    uint8_t   cmd;
    uint32_t  value;
} cmd_lamp_s;

typedef struct video_list {
    const cmd_lamp_s *video;
    const size_t video_sz;
} video_list_s;

void check_button(void);
void video_ctl_timer_event(void);
void bs_entry(void);

#ifdef __cplusplus
}
#endif

#endif /* VIDEO_CTL__H */
