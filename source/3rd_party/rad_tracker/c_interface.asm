%include "player_nasm.asm"

global radInitPlayer_
radInitPlayer_:
    shr ax, 4   ; ax contains offset
    add ax, dx  ; dx contains segment
    mov es, ax

    call InitPlayer
    jc .error    ; InitPlayer sets carry flag if error occured
    .success:
    mov ax, 01h  ; return true
    retf
    .error:
    mov ax, 00h  ; return false
    retf

global radPlayMusic_
radPlayMusic_:
    call PlayMusic
    retf




