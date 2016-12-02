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

#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common.hpp"
#include "drivers.hpp"
#include "led_driver.hpp"

void LED_Write(const LEDDriverData& data)
{
  uint32_t packetHeader;

  packetHeader = 0b100101; // Magic write command

  packetHeader <<= 5;
  packetHeader |= 0b10110;

  packetHeader <<= 7;
  packetHeader |= data._BCred;

  packetHeader <<= 7;
  packetHeader |= data._BCgreen;

  packetHeader <<= 7;
  packetHeader |= data._BCblue;

  uint8_t sreg = SREG;
  cli();

  SPI_Transmit(packetHeader >> 24);
  SPI_Transmit(packetHeader >> 16);
  SPI_Transmit(packetHeader >> 8);
  SPI_Transmit(packetHeader);

  uint8_t channel = ARRAY_LEN(data._PWMs);
  do {
    --channel;
    SPI_Transmit(data._PWMs[channel] >> 8);
    SPI_Transmit(data._PWMs[channel]);
  } while(channel != 0);

  _delay_us(2);
  SREG = sreg;
}
