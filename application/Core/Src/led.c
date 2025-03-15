/*
 * led.c
 *
 *  Created on: Mar 15, 2025
 *      Author: MadaYaswanth
 */
#include"main.h"
void led_init(){
    // Enable clock for GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // Configure PC13 as output (push-pull, 2MHz)
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= (GPIO_CRH_MODE13); // Output mode, 2 MHz
}
void led_on(){
	 GPIOC->BSRR = GPIO_BSRR_BR13;
}
void led_off(){
	  GPIOC->BSRR = GPIO_BSRR_BS13;  // LED OFF (PC13 HIGH)
}
