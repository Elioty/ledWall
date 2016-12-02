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

#ifndef __COLORS__
#define __COLORS__

#include "common.hpp"

union RGB {
  ledpwm_t array[3];
  struct {
    ledpwm_t r;
    ledpwm_t g;
    ledpwm_t b;
  };
};

static constexpr RGB BLACK         __maybe_unused = {    0,    0,    0 };
static constexpr RGB WHITE         __maybe_unused = { 4096, 4096, 4096 };
static constexpr RGB COLOR_WHEEL[] __maybe_unused = {
    { 4096,    0,    0 },
    { 4096, 4096,    0 },
    {    0, 4096,    0 },
    {    0, 4096, 4096 },
    {    0,    0, 4096 },
    { 4096,    0, 4096 },
};
static constexpr RGB RED          __maybe_unused = COLOR_WHEEL[0];
static constexpr RGB YELLOW       __maybe_unused = COLOR_WHEEL[1];
static constexpr RGB GREEN        __maybe_unused = COLOR_WHEEL[2];
static constexpr RGB TURQUOISE    __maybe_unused = COLOR_WHEEL[3];
static constexpr RGB BLUE         __maybe_unused = COLOR_WHEEL[4];
static constexpr RGB MAGENTA      __maybe_unused = COLOR_WHEEL[5];

#endif//__COLORS__
