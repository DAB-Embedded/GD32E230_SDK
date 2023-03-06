/**
  ******************************************************************************
  * @file      startup_gd32e23x.s
  * @author    DAB-Embedded
  * @brief     GD32E23x Devices vector table for GCC based toolchains.
  ******************************************************************************
  * @version   2022-03-09, V3.0.0, firmware for GD32E23x
  */

  .syntax unified
  .cpu cortex-m23
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .thumb_func
  .align 2
  .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack     /* set stack pointer */
  mov   sp, r0

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system initialization function.*/
  bl  SystemInit
/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  bx  lr
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None
 * @retval None
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors

  g_pfnVectors:
  .word  _estack
  .word  Reset_Handler

  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  DebugMon_Handler
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  /* External Interrupts */
  .word     WWDGT_IRQHandler           /* Window Watchdog Timer  */
  .word     LVD_IRQHandler             /* LVD through EXTI Line detect */
  .word     RTC_IRQHandler             /* RTC through EXTI Line */
  .word     FMC_IRQHandler             /* FMC                   */
  .word     RCU_IRQHandler             /* RCU                   */
  .word     EXTI0_1_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     EXTI2_3_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     EXTI4_15_IRQHandler        /* EXTI Line 0 and EXTI Line 1 */
  .word     0                          /* Skip */
  .word     DMA_Channel0_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     DMA_Channel1_2_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     DMA_Channel3_4_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     ADC_CMP_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER0_BRK_UP_TRG_COM_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER0_Channel_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     0                          /* Skip */
  .word     TIMER2_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER5_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     0                          /* Skip */
  .word     TIMER13_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER14_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER15_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     TIMER16_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     I2C0_EV_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     I2C1_EV_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     SPI0_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     SPI1_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     USART0_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     USART1_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     0                          /* Skip */
  .word     0                          /* Skip */
  .word     0                          /* Skip */
  .word     I2C0_ER_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */
  .word     0                          /* Skip */
  .word     I2C1_ER_IRQHandler         /* EXTI Line 0 and EXTI Line 1 */




/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler

   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler

   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler

   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      DebugMon_Handler
   .thumb_set DebugMon_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler

   .weak      WWDGT_IRQHandler
   .thumb_set WWDGT_IRQHandler,Default_Handler

   .weak      LVD_IRQHandler
   .thumb_set LVD_IRQHandler,Default_Handler

   .weak      RTC_IRQHandler
   .thumb_set RTC_IRQHandler,Default_Handler

   .weak      FMC_IRQHandler
   .thumb_set FMC_IRQHandler,Default_Handler

   .weak      RCU_IRQHandler
   .thumb_set RCU_IRQHandler,Default_Handler

   .weak      EXTI0_1_IRQHandler
   .thumb_set EXTI0_1_IRQHandler,Default_Handler

   .weak      EXTI2_3_IRQHandler
   .thumb_set EXTI2_3_IRQHandler,Default_Handler

   .weak      EXTI4_15_IRQHandler
   .thumb_set EXTI4_15_IRQHandler,Default_Handler

   .weak      DMA_Channel0_IRQHandler
   .thumb_set DMA_Channel0_IRQHandler,Default_Handler

   .weak      DMA_Channel1_2_IRQHandler
   .thumb_set DMA_Channel1_2_IRQHandler,Default_Handler

   .weak      DMA0_Channel0_IRQHandler
   .thumb_set DMA0_Channel0_IRQHandler,Default_Handler

   .weak      DMA_Channel3_4_IRQHandler
   .thumb_set DMA_Channel3_4_IRQHandler,Default_Handler

   .weak      ADC_CMP_IRQHandler
   .thumb_set ADC_CMP_IRQHandler,Default_Handler

   .weak      TIMER0_BRK_UP_TRG_COM_IRQHandler
   .thumb_set TIMER0_BRK_UP_TRG_COM_IRQHandler,Default_Handler

   .weak      TIMER0_Channel_IRQHandler
   .thumb_set TIMER0_Channel_IRQHandler,Default_Handler

   .weak      TIMER2_IRQHandler
   .thumb_set TIMER2_IRQHandler,Default_Handler

   .weak      TIMER5_IRQHandler
   .thumb_set TIMER5_IRQHandler,Default_Handler

   .weak      TIMER13_IRQHandler
   .thumb_set TIMER13_IRQHandler,Default_Handler

   .weak      TIMER14_IRQHandler
   .thumb_set TIMER14_IRQHandler,Default_Handler

   .weak      TIMER15_IRQHandler
   .thumb_set TIMER15_IRQHandler,Default_Handler

   .weak      TIMER16_IRQHandler
   .thumb_set TIMER16_IRQHandler,Default_Handler

   .weak      I2C0_EV_IRQHandler
   .thumb_set I2C0_EV_IRQHandler,Default_Handler

   .weak      I2C1_EV_IRQHandler
   .thumb_set I2C1_EV_IRQHandler,Default_Handler

   .weak      SPI0_IRQHandler
   .thumb_set SPI0_IRQHandler,Default_Handler

   .weak      SPI1_IRQHandler
   .thumb_set SPI1_IRQHandler,Default_Handler

   .weak      USART0_IRQHandler
   .thumb_set USART0_IRQHandler,Default_Handler

   .weak      USART1_IRQHandler
   .thumb_set USART1_IRQHandler,Default_Handler

   .weak      I2C0_ER_IRQHandler
   .thumb_set I2C0_ER_IRQHandler,Default_Handler

   .weak      I2C1_ER_IRQHandler
   .thumb_set I2C1_ER_IRQHandler,Default_Handler
