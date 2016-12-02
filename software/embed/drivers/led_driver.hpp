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

#ifndef __LED_DRIVER__
#define __LED_DRIVER__

#include <stdint.h>
#include "../common.hpp"
#include "../colors.hpp"

class LEDDriverData
{
  BC_t _BCred;
  BC_t _BCgreen;
  BC_t _BCblue;
  union {
    ledpwm_t _PWMs[CHANNEL_PER_LED_DRIVER];
    RGB      _LEDColors[LED_PER_LED_DRIVER];
  };

  friend void LED_Write(const LEDDriverData& data);

public:
#define BC_ACCESSORS(__color)                   \
  inline BC_t getBC##__color() const            \
  {                                             \
    return _BC##__color;                        \
  }                                             \
  inline void setBC##__color(BC_t val)          \
  {                                             \
    _BC##__color = val > BC_MAX ? BC_MAX : val; \
  }

  BC_ACCESSORS(red);
  BC_ACCESSORS(green);
  BC_ACCESSORS(blue);
#undef BC_ACCESSORS

  inline ledpwm_t getPWM(uint8_t channel) const
  {
    return _PWMs[channel];
  }
  inline void setPWM(uint8_t channel, ledpwm_t val)
  {
    _PWMs[channel] = val > LEDPWM_MAX ? LEDPWM_MAX : val;
  }

  inline RGB getLEDColor(uint8_t LED) const
  {
    return _LEDColors[LED];
  }
  void setLEDColor(uint8_t LED, const RGB& LEDColor)
  {
    setPWM(3 * LED + 0, LEDColor.array[0]);
    setPWM(3 * LED + 1, LEDColor.array[1]);
    setPWM(3 * LED + 2, LEDColor.array[2]);
  }
};

void LED_Write(const LEDDriverData& data);

#endif//__LED_DRIVER__
