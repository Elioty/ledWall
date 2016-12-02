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

#ifndef __DRIVERS__
#define __DRIVERS__

#include <stdint.h>
#include <avr/io.h>
#include "../common.hpp"

inline void SPI_Init() __maybe_unused;
inline void SPI_Init()
{
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  SPSR = (1 << SPI2X);
}

inline void SPI_Transmit(uint8_t byte) __maybe_unused;
inline void SPI_Transmit(uint8_t byte)
{
  SPDR = byte;
  asm volatile("nop");
  while(!(SPSR & (1 << SPIF)));
}

inline void UART0_Init() __maybe_unused;
inline void UART0_Init()
{
#define BAUD 9600
#include <util/setbaud.h>
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
#if USE_2X
  UCSR0A |= (1 << U2X0);
#else
  UCSR0A &= ~(1 << U2X0);
#endif
#undef BAUD
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

inline bool UART0_Pending() __maybe_unused;
inline bool UART0_Pending()
{
  return (UCSR0A & (1 << RXC0));
}

inline uint8_t UART0_Receive() __maybe_unused;
inline uint8_t UART0_Receive()
{
  while(!UART0_Pending());
  return UDR0;
}

inline bool UART0_Ready() __maybe_unused;
inline bool UART0_Ready()
{
  return (UCSR0A & (1 << UDRE0));
}

inline void UART0_Transmit(uint8_t byte) __maybe_unused;
inline void UART0_Transmit(uint8_t byte)
{
  while(!UART0_Ready());
  UDR0 = byte;
}

inline void Timer1_Start() __maybe_unused;
inline void Timer1_Start()
{
  // 16-bit timer, 16MHz-chip with /8 pre-scaler
  // => 32.768ms period, 30.52Hz
  TCCR1A = 0;
  TCCR1B = (1 << CS11);
}

inline void Timer1_Stop() __maybe_unused;
inline void Timer1_Stop()
{
  TCCR1B = 0;
  TCNT1 = 0;
}

inline bool Timer1_Expired() __maybe_unused;
inline bool Timer1_Expired()
{
  return (TIFR1 & (1 << TOV1));
}

inline void Timer1_ClearOverflowFlag() __maybe_unused;
inline void Timer1_ClearOverflowFlag()
{
  TIFR1 = (1 << TOV1);
}

inline void Timer1_Wait() __maybe_unused;
inline void Timer1_Wait()
{
  while(!Timer1_Expired());
  Timer1_ClearOverflowFlag();
}

//TIMSK1 = (1 << TOIE1); To turn on timer interrupt

#endif//__DRIVERS__
