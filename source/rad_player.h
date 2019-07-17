#ifndef RAD_PLAYER_H_INCLUDED
#define RAD_PLAYER_H_INCLUDED

extern "C"
{
    extern void radPlayMusic();
    extern void radEndPlayer();
}

void* radLoadModule(const char* filename);


#endif