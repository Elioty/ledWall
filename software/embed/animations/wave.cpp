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

#include "wave.hpp"
#include "../common.hpp"
#include "../colors.hpp"
#include "../drivers/led_driver.hpp"

static constexpr ledpwm_t WAVE_FORM[] = {
    0,
    9,
    39,
    88,
    155,
    241,
    345,
    464,
    599,
    748,
    910,
    1082,
    1264,
    1453,
    1648,
    1847,
    2048,
    2248,
    2447,
    2642,
    2831,
    3013,
    3185,
    3347,
    3496,
    3631,
    3750,
    3854,
    3940,
    4007,
    4056,
    4086,
    4095,
    4086,
    4056,
    4007,
    3940,
    3854,
    3750,
    3631,
    3496,
    3347,
    3185,
    3013,
    2831,
    2642,
    2447,
    2248,
    2048,
    1847,
    1648,
    1453,
    1264,
    1082,
    910,
    748,
    599,
    464,
    345,
    241,
    155,
    88,
    39,
    9,
};
static constexpr uint8_t WAVE_LEN = ARRAY_LEN(WAVE_FORM);

Wave::Wave(LEDDriverData* LEDData, uint8_t nb) :
    Animation(LEDData, nb), _color(WHITE), _step(0), _ledSpace(4)
{
}

Wave::~Wave()
{
}

void Wave::setup(uint8_t buf, uint16_t len)
{
  (void) buf;
  (void) len;
}

void Wave::start()
{
  _step = 0;

  for(uint8_t i = 0; i < _nbLEDData; ++i) {
    _LEDData[i].setBCred(BC_RED);
    _LEDData[i].setBCgreen(BC_GREEN);
    _LEDData[i].setBCblue(BC_BLUE);
    for(uint8_t j = 0; j < LED_PER_LED_DRIVER; ++j)
      _LEDData[i].setLEDColor(j, BLACK);
  }
}

void Wave::stop()
{
}

void Wave::update()
{
  LED_Write(_LEDData, _nbLEDData);

  if(++_step >= WAVE_LEN)
    _step = 0;

  for(uint8_t i = 0; i < _nbLEDData; ++i)
    for(uint8_t j = 0; j < LED_PER_LED_DRIVER; ++j) {
      ledpwm_t val =
          WAVE_FORM[(_step + _ledSpace * (LED_PER_LED_DRIVER * i + j))
                    % WAVE_LEN];
      _LEDData[i].setLEDColor(j, {
          _color.r == 0 ? 0 : val,
          _color.g == 0 ? 0 : val,
          _color.b == 0 ? 0 : val
      });
    }
}
