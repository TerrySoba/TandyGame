#include "timer.h"

#include <dos.h>
#include <stdint.h>
#include <conio.h>

#define TIMER_INTERRUPT 0x08

static void (*s_timerFunction)();


static void __interrupt __far customTimerInterrupt()
{
    s_timerFunction();

    // signal interrupt done
    __asm{
        "mov al,20H"
        "out 20H,al"
    }
}

DosTimer::DosTimer(void (*timerFunction)(), int frequency)
{
    s_timerFunction = timerFunction;

    // The clock we're dealing with here runs at 1.193182mhz, so we
    // just divide 1.193182 by the number of triggers we want per
    // second to get our divisor.
    uint32_t c = 1193181 / (uint32_t)frequency;

    m_oldInterrupt = _dos_getvect(TIMER_INTERRUPT);
    _dos_setvect(TIMER_INTERRUPT, customTimerInterrupt);

    __asm{ "cli" }

    // There's a ton of options encoded into this one byte I'm going
    // to send to the PIT here so...

    // 0x34 = 0011 0100 in binary.

    // 00  = Select counter 0 (counter divisor)
    // 11  = Command to read/write counter bits (low byte, then high
    //       byte, in sequence).
    // 010 = Mode 2 - rate generator.
    // 0   = Binary counter 16 bits (instead of BCD counter).

    outp(0x43, 0x34);

    // Set divisor low byte.
    outp(0x40, (uint8_t)(c & 0xff));

    // Set divisor high byte.
    outp(0x40, (uint8_t)((c >> 8) & 0xff));

    __asm { "sti" }
}

DosTimer::~DosTimer()
{
    uint32_t c = 1193181 / (uint32_t)18;

    __asm{ "cli" }

    outp(0x43, 0x34);

    // Set divisor low byte.
    outp(0x40, (uint8_t)(c & 0xff));

    // Set divisor high byte.
    outp(0x40, (uint8_t)((c >> 8) & 0xff));

    __asm { "sti" }

    _dos_setvect(TIMER_INTERRUPT, m_oldInterrupt);
}

