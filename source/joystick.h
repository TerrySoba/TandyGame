#ifndef _INCLUDE_JOYSTICK_H
#define _INCLUDE_JOYSTICK_H

#include <stdint.h>


struct JoystickStatus
{
    uint8_t buttons;
    uint16_t x1, y1, x2, y2;
};

JoystickStatus readJoystick();

#endif
