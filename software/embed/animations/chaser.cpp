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

#include "chaser.hpp"
#include "../common.hpp"
#include "../colors.hpp"
#include "../drivers/led_driver.hpp"

Chaser::Chaser(LEDDriverData* LEDData, uint8_t nb, const RGB& color, uint8_t len) :
    Animation(LEDData, nb), _color(color), _len(len), _step(0)
{
}

Chaser::~Chaser()
{
}

void Chaser::setup(uint8_t buf, uint16_t len)
{
  (void) buf;
  (void) len;
}

void Chaser::start()
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

void Chaser::stop()
{
}

void Chaser::update()
{
  LED_Write(_LEDData, _nbLEDData);

  if(++_step >= (_nbLEDData * LED_PER_LED_DRIVER))
    _step = 0;

  uint8_t rem = _len;
  uint8_t idx;
  uint8_t i;
  uint8_t j;
  do {
    uint8_t dist = _len - rem;
    
    idx = _step - dist;
    if(dist > _step)
      idx += _nbLEDData * LED_PER_LED_DRIVER;
    
    i = idx / LED_PER_LED_DRIVER;
    j = idx % LED_PER_LED_DRIVER;

    _LEDData[i].setLEDColor(j, {
	_color.r / ((dist + 1) * 2),
	_color.g / ((dist + 1) * 2),
	_color.b / ((dist + 1) * 2)
    });
  } while(--rem != 0);

  --idx;
  if(idx >= (_nbLEDData * LED_PER_LED_DRIVER))
    idx = (_nbLEDData * LED_PER_LED_DRIVER) - 1;
  i = idx / LED_PER_LED_DRIVER;
  j = idx % LED_PER_LED_DRIVER;
  _LEDData[i].setLEDColor(j, BLACK);
}
