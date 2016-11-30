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

#ifndef __LEDS__
#define __LEDS__

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void SPI_Transmit(uint8_t byte);

template<uint8_t numLedDriver>
class Leds
{
public:
  using BC_t = uint8_t;
  using pwm_t = uint16_t;
  using channel_t = uint16_t;
  using led_t = uint16_t;
  enum class color_t : uint8_t { RED = 0, GREEN, BLUE };

  static constexpr channel_t channelPerDriver = 12;
  static constexpr BC_t  BCmax  = 0x7F;
  static constexpr led_t numLed = numLedDriver * channelPerDriver;
  static constexpr pwm_t PWMmax = 4096 - 1;

private:
  pwm_t _PWMs[numLed];

  BC_t _BCred;
  BC_t _BCgreen;
  BC_t _BCblue;

public:
  inline Leds() :
    _PWMs{0}, _BCred(0), _BCgreen(0), _BCblue(0)
  {
  }

  inline Leds(BC_t BCred, BC_t BCgreen, BC_t BCblue) :
    _PWMs{0}, _BCred(BCred), _BCgreen(BCgreen), _BCblue(BCblue)
  {
  }

  Leds(const Leds& o) = delete;
  Leds& operator=(const Leds& o) = delete;

#define BCaccessors(color)                                \
  inline BC_t BC##color() const                           \
  {                                                       \
    return _BC##color;                                    \
  }                                                       \
                                                          \
  inline void setBC##color(BC_t BC##color)                \
  {                                                       \
    _BC##color = (BC##color < BCmax) ? BC##color : BCmax; \
  }

  BCaccessors(red);
  BCaccessors(green);
  BCaccessors(blue);

#undef BCaccessors

  inline pwm_t channelPWM(channel_t channelId) const
  {
    return _PWMs[channelId];
  }

  inline void setChannelPWM(channel_t channelId, pwm_t PWMvalue)
  {
    _PWMs[channelId] = (PWMvalue < PWMmax) ? PWMvalue : PWMmax;
  }

  inline pwm_t LEDcolorPWM(led_t ledId, color_t color) const
  {
    return channelPWM(ledId * 3 + (channel_t) color);
  }

  inline void setLED(led_t ledId, pwm_t red, pwm_t green, pwm_t blue)
  {
    setChannelPWM(ledId * 3    , red);
    setChannelPWM(ledId * 3 + 1, green);
    setChannelPWM(ledId * 3 + 2, blue);
  }

  inline void write()
  {
    uint32_t packetHeader;

    packetHeader = 0b100101; // Magic write command

    packetHeader <<= 5;
    packetHeader |= 0b10110;

    packetHeader <<= 7;
    packetHeader |= _BCred;

    packetHeader <<= 7;
    packetHeader |= _BCgreen;

    packetHeader <<= 7;
    packetHeader |= _BCblue;

    cli();
    for(uint8_t ledDriver = 0; ledDriver < numLedDriver; ++ledDriver) {
      SPI_Transmit(packetHeader >> 24);
      SPI_Transmit(packetHeader >> 16);
      SPI_Transmit(packetHeader >> 8);
      SPI_Transmit(packetHeader);

      uint8_t channel = channelPerDriver;
      do {
        --channel;
        SPI_Transmit(_PWMs[channelPerDriver * ledDriver + channel] >> 8);
        SPI_Transmit(_PWMs[channelPerDriver * ledDriver + channel]);
      } while(channel != 0);
    }

    _delay_us(2);
    sei();
  }
};

#endif//__LEDS__
