#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "leds.hpp"

#define __maybe_unused  __attribute__((unused))

//#define COLOR_WHEEL
//#define UART_CLIENT
#define WHITE_BREATHING
//#define CHAIN

void SPI_Init()
{
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  SPSR = (1 << SPI2X);
}

void SPI_Transmit(uint8_t byte)
{
  SPDR = byte;

  while(!(SPSR & (1 << SPIF)));
}

void UART_Init()
{
  UBRR0 = 103; // 9600bps at 16MHz MCU
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

uint8_t UART_Receive()
{
  while(!(UCSR0A & (1 << RXC0)));
  return UDR0;
}

void UART_Transmit(uint8_t byte)
{
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = byte;
}

static constexpr uint8_t NumLedDriver = 3;
static constexpr Leds<NumLedDriver>::BC_t BCRed = 127;
static constexpr Leds<NumLedDriver>::BC_t BCGreen = 127;
static constexpr Leds<NumLedDriver>::BC_t BCBlue = 127;
using LedDriver = Leds<NumLedDriver>;

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

union RGB {
  LedDriver::pwm_t array[3];
  struct {
    LedDriver::pwm_t r;
    LedDriver::pwm_t g;
    LedDriver::pwm_t b;
  };
};

static constexpr RGB Black        __maybe_unused = {    0,    0,    0 };
static constexpr RGB White        __maybe_unused = { 4096,    0, 4096 };
static constexpr RGB ColorWheel[] __maybe_unused = {
    { 4096,    0,    0 },
    { 4096, 4096,    0 },
    {    0, 4096,    0 },
    {    0, 4096, 4096 },
    {    0,    0, 4096 },
    { 4096,    0, 4096 },
};
static constexpr RGB Red          __maybe_unused = ColorWheel[0];
static constexpr RGB Yellow       __maybe_unused = ColorWheel[1];
static constexpr RGB Green        __maybe_unused = ColorWheel[2];
static constexpr RGB Turquoise    __maybe_unused = ColorWheel[3];
static constexpr RGB Blue         __maybe_unused = ColorWheel[4];
static constexpr RGB Magenta      __maybe_unused = ColorWheel[5];

int main()
{
  LedDriver leds(BCRed, BCGreen, BCBlue);

  SPI_Init();
#if defined(COLOR_WHEEL)
  /*
  static constexpr RGB Colors[] = {
      { 4095, 4095, 4095 }, // White
      { 4095, 0 ,0 }, // Red
      { 4095, 0, 4095 }, // Magenta
      { 0, 0, 4095 }, // Blue
      { 0, 4095, 4095 }, // Turquoise
      { 0, 4095, 0 }, // Green
      { 4095, 4095, 0 }, // Yellow
  };
  */

  uint8_t movingColor = 1;
  bool goingUp = true;
  RGB color { 4095, 0, 0 };
  while(1) {
    leds.write();

    if(goingUp) {
      color.array[movingColor] += 512;
      if(color.array[movingColor] == 4096) {
        color.array[movingColor] = 4095;
        goingUp = false;
        --movingColor;
        if(movingColor == 0xFF)
          movingColor = 2;
      }
    }
    else {
      if(color.array[movingColor] == 4095)
        color.array[movingColor] = 4096 - 512;
      else {
        color.array[movingColor] -= 512;
        if(color.array[movingColor] == 0) {
          goingUp = true;
          movingColor += 2;
          if(movingColor > 2)
            movingColor -= 3;
        }
      }
    }

    leds.setLED(3, color.r, color.g, color.b);
    _delay_ms(75);
  }
#elif defined(UART_CLIENT)
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
#elif defined(WHITE_BREATHING)
  static constexpr LedDriver::pwm_t sinus4096[] = {
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
  static constexpr uint8_t len = ARRAY_LEN(sinus4096);

  uint8_t i = 0;
  while(1) {
    leds.write();

    ++i;
    if(i == len)
      i = 0;

    leds.setLED(2,
                sinus4096[i],
                sinus4096[i],
                sinus4096[i]);
    leds.setLED(3,
                sinus4096[(i + len / 16) % len],
                sinus4096[(i + len / 16) % len],
                sinus4096[(i + len / 16) % len]);
    leds.setLED(3 + 4,
                sinus4096[(i + len / 8) % len],
                sinus4096[(i + len / 8) % len],
                sinus4096[(i + len / 8) % len]);
    leds.setLED(3 + 8,
                sinus4096[(i + len / 4) % len],
                sinus4096[(i + len / 4) % len],
                sinus4096[(i + len / 4) % len]);
    _delay_ms(23);
  }
#else
  while(1);
#endif

  return 0;
}
