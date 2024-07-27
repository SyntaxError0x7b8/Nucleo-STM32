/*
  ******************************************************************************
  * @file           : main.c
  * @brief          : This program is created to use a Nucleo-STM32F446RE board
  * 				to output digits from 0 to 9 using a shift register chip
  * 				SN74HC595 on a 7-segment display 5161AS (Common Cathode).
  * 				For that, nucleo board connects PA5 to SRCLK (aka SHCP),
  * 				PA6 to RCLK (aka STCP) and PA7 to SER (aka DS).
  * 				~OE to GND and ~SRCLK to Vcc.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"

/* 7-segment display: {DP, A, B, C, D, E, F, G} */

int digits[10][8] = {
	{0, 1, 1, 1, 1, 1, 1, 0},		// digit 0
	{0, 0, 1, 1, 0, 0, 0, 0},		// digit 1
	{0, 1, 1, 0, 1, 1, 0, 1},		// digit 2
	{0, 1, 1, 1, 1, 0, 0, 1},		// digit 3
	{0, 0, 1, 1, 0, 0, 1, 1},		// digit 4
	{0, 1, 0, 1, 1, 0, 1, 1},		// digit 5
	{0, 1, 0, 1, 1, 1, 1, 1},		// digit 6
	{0, 1, 1, 1, 0, 0, 0, 0},		// digit 7
	{0, 1, 1, 1, 1, 1, 1, 1},		// digit 8
	{0, 1, 1, 1, 1, 0, 1, 1}		// digit 9
	};


void delayMs(int n);

void displayDigit(int Digit);

int main(void)
{
	RCC->AHB1ENR |= JTM_GPIOA_CLK_EN;

	GPIOA->MODER &= JTM_CLEAR_PA5_6_7;			// clear mode of PA5, PA6 and PA7
	GPIOA->MODER |= JTM_OUTPUT_PA5_6_7;			// set PA5, PA6 and PA7 to '01' (output mode)

	while(1){
		int num = 0;
		for(num = 0; num < 10; num++){
			displayDigit(num);
			delayMs(250);
		}

	}

}

/* 16MHz SYSCLK (always check with oscilloscope) */
void delayMs(int n)
{
	int i = 0;
	for(; n > 0; n--){
		for(i = 0; i < 3200; i++);
	}
}


/* send the data to the shift register SN74HC595 8-bit */
void displayDigit(int Digit)
{

	GPIOA->BSRR = JTM_BSRR_RCLK_PA6_LO;		// STCP Low
	// update all 8 bits from LSB to MSB
	for(int d = 7; d >= 0; d--){
		GPIOA->BSRR = JTM_BSRR_SRCLK_PA5_LO;  // SHCP Low
		if(digits[Digit][d] == 1) { GPIOA->BSRR = JTM_BSRR_SER_PA7_HI; } // DS High
		else { GPIOA->BSRR = JTM_BSRR_SER_PA7_LO; } // DS Low
		GPIOA->BSRR = JTM_BSRR_SRCLK_PA5_HI;  // SHCP High
	}
	GPIOA->BSRR = JTM_BSRR_RCLK_PA6_HI;  // STCP High --> send data to shift register
}
