#include "rad_player.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdexcept>


extern "C"
{
    extern void radPlayMusic();
    extern void radEndPlayer();
	extern uint16_t radInitPlayer(const uint8_t far* songData);
}

/**
 * Returns addresses that are aligned to 16byte
 */
static void* allignedMalloc(int size, void** origBlock = NULL)
{
	char* ptr = (char*)malloc(size + 16);
	if (origBlock) *origBlock = (void*)ptr;
	ptr += 16ul - ((uint32_t)ptr & 0xf);
	return (void*)ptr;
}

void* radLoadModule(const char* filename)
{
	FILE* fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (!fp)
	{
        printf("Could not open file.");
		return NULL;
	}

	void* origBlock;
	uint8_t* songData = (uint8_t*)allignedMalloc(len, &origBlock);
	uint8_t* buf = songData;

	while (!feof(fp))
	{
		int bytes = fread(buf, 1, 1000, fp);
		buf += bytes;
	}

	fclose(fp);

	if (!radInitPlayer(songData))
	{
		printf("Could not initialize RAD player. Maybe file is corrupted.\n");
		free(origBlock);
		return NULL;
	}

	return origBlock;
}

static void timerFunction()
{
	radPlayMusic();
}

RadPlayer::RadPlayer(const char* modulePath)
{
	m_songData = radLoadModule(modulePath);
	if (!m_songData)
	{
		throw std::runtime_error("Error loading rad module.");
	}
	m_timer = new DosTimer(timerFunction, 50);
}

RadPlayer::~RadPlayer()
{
	delete m_timer;
	radEndPlayer();
	free(m_songData);
}
