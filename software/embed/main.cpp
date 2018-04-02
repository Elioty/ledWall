/*-
 * Copyright (c) 2016, Clément Lansmarie
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * $ledWall$
 */

#include <stdint.h>
#include "common.hpp"
#include "drivers/drivers.hpp"
#include "drivers/led_driver.hpp"
#include "animations/wave.hpp"
#include "animations/chaser.hpp"

int main()
{
  /********************************** Inits **********************************/
  SPI_Init();
  UART0_Init();

  /******************************* Hello world *******************************/
  static constexpr char HELLO[] = "Hello World\n\r";
  for(uint8_t i = 0; i < ARRAY_LEN(HELLO); ++i)
    UART0_Transmit(HELLO[i]);

  /******************** Reading and displaying team color ********************/
  DDRA  &= ~(1 << PA0); // Set PA0 as input
  PORTA |=  (1 << PA0); // Enable pull-up on PA0

  RGB color;
  uint8_t c = PINA & (1 << PA0);

  static constexpr char COLOR[] = "color == ";
  for(uint8_t i = 0; i < ARRAY_LEN(COLOR); ++i)
    UART0_Transmit(COLOR[i]);

  if(c != 0) {
    color = GREEN;
    static constexpr char S_GREEN[] = "GREEN";
    for(uint8_t i = 0; i < ARRAY_LEN(S_GREEN); ++i)
      UART0_Transmit(S_GREEN[i]);
  }
  else {
    color = ORANGE;
    static constexpr char S_ORANGE[] = "ORANGE";
    for(uint8_t i = 0; i < ARRAY_LEN(S_ORANGE); ++i)
      UART0_Transmit(S_ORANGE[i]);
  }

  UART0_Transmit('\n');
  UART0_Transmit('\r');

  /************************ Initializing the animation ***********************/
  LEDDriverData LEDData[LED_DRIVERS];
  //Wave waveAnimation(LEDData, ARRAY_LEN(LEDData));
  Chaser chaserAnimation(LEDData, ARRAY_LEN(LEDData), color, 4);

  //Animation* currentAnimation = &waveAnimation;
  Animation* currentAnimation = &chaserAnimation;
  currentAnimation->start();

  DDRB  |= (1 << PB7); // Set PB7 as output
  PORTB |= (1 << PB7); // Set PB7 high

  /**************************** Run the animation ****************************/
  Timer1_Start();
  while(1) {
    currentAnimation->update();
    if(Timer1_Expired()) {
      // Wow, we were too slow!!
      // We should reset here
      PORTB |= (1 << PB7);
      static constexpr char TOO_SLOW[] = "We were too slow!\n\r";
      for(uint8_t i = 0; i < ARRAY_LEN(TOO_SLOW); ++i)
	UART0_Transmit(TOO_SLOW[i]);
      while(1);
    }
    // FIXME: should be only one wait, the animation has to manage its speed itself
    Timer1_Wait();
    Timer1_Wait();
    Timer1_Wait();
    PINB = (1 << PB7); // Toggle PB7
  }

#if defined(UART_CLIENT)
  UART_Init();
  leds.write();

  uint8_t s = 0;
  RGB color { 0, 0, 0 };
  while(1) {
    uint8_t c = UART_Receive();
    if(c != '\n' && c != '\r')
      UART_Transmit(c);

    if(c >= '0' && c <= '9') {
      color.array[s] *= 10;
      color.array[s] += c - '0';
    }
    else {
      if(++s == 3) {
        leds.setLED(3, color.r, color.g, color.b);
        leds.write();

        s = 0;
        color.r = color.g = color.b = 0;

        UART_Transmit('\n');
        UART_Transmit('\r');
        UART_Transmit('O');
        UART_Transmit('K');
        UART_Transmit('\n');
        UART_Transmit('\r');
      }
    }
  }
#endif

  return 0;
}
