#include "joystick.h"



JoystickStatus readJoystick()
{
    uint8_t buttons = 0;
    uint16_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    __asm   {
        push ax
        push bx
        push cx
        push dx
        
        // read buttons
        mov ah, 84h      // 
        mov dx, 0    // 0000h == read joystick switches

        int 15h
        jc error         // jump if carry -> error!

        mov buttons, al  // store buttons in variable

        // read axis data
        mov ah, 84h      // 
        mov dx, 1    // 0000h == read joystick axis data
        int 15h

        mov x1, ax
        mov y1, bx
        mov x2, cx
        mov y2, dx

        jc error         // jump if carry -> error!
        jmp end

        error:
        mov bh, 255
        mov buttons, bh

        end:
        pop dx
        pop cx
        pop bx
        pop ax
    };

    JoystickStatus status;
    status.buttons = buttons;
    status.x1 = x1;
    status.y1 = y1;
    status.x2 = x2;
    status.y2 = y2;

    return status;
}
