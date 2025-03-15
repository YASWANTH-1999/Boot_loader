/*
 * application.h
 *
 *  Created on: Mar 15, 2025
 *      Author: MadaYaswanth
 */
#include"appliction.h"
#include<stdint.h>
extern void led_off();
void goto_application(){
//  void (*app_reset_handler)(void)=(void*)(*(volatile uint32_t*)(8004000+4));
//
//   ///__set_MSP((*(volatile uint32_t*)(8000000)));
//  led_off();
//  app_reset_handler();

  uint32_t appStack = *(volatile uint32_t*)APP_START_ADDR;
   uint32_t appResetHandler = *(volatile uint32_t*)(APP_START_ADDR + 4);

   //__set_MSP(appStack);  // Set Main Stack Pointer
   // Set MSP manually
   __asm volatile ("MSR MSP, %0" : : "r" (appStack) : "memory");
   void (*resetHandler)(void) = (void (*)(void))appResetHandler;
   resetHandler();  // Jump to application
}
