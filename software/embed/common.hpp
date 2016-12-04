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

#ifndef __COMMON__
#define __COMMON__

#include <stdlib.h>
#include <stdint.h>

#define __maybe_unused  __attribute__((unused))
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

using ledpwm_t = uint16_t;
using BC_t = uint8_t;

static constexpr uint8_t CHANNEL_PER_LED_DRIVER __maybe_unused = 12;
static constexpr uint8_t LED_PER_LED_DRIVER     __maybe_unused =
    CHANNEL_PER_LED_DRIVER / 3;
static constexpr uint8_t LED_DRIVERS            __maybe_unused = 3;

static constexpr ledpwm_t LEDPWM_MASK __maybe_unused = 0x0FFF;
static constexpr ledpwm_t LEDPWM_MAX  __maybe_unused = LEDPWM_MASK;
static constexpr BC_t BC_MASK         __maybe_unused = 0x7F;
static constexpr BC_t BC_MAX          __maybe_unused = BC_MASK;

static constexpr BC_t BC_RED   __maybe_unused = 127;
static constexpr BC_t BC_GREEN __maybe_unused = 95;
static constexpr BC_t BC_BLUE  __maybe_unused = 85;

inline void* operator new(size_t s) __maybe_unused;
inline void* operator new(size_t s)
{
  return malloc(s);
}

inline void operator delete(void* p) __maybe_unused;
inline void operator delete(void* p)
{
  free(p);
}

inline void* operator new[](size_t s) __maybe_unused;
inline void* operator new[](size_t s)
{
  return malloc(s);
}

inline void operator delete[](void* p) __maybe_unused;
inline void operator delete[](void* p)
{
  free(p);
}

#endif//__COMMON__
