#include "rad_player.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern "C"
{
	extern uint16_t radInitPlayer(const uint8_t far* songData);
}

/**
 * Returns addresses that are aligned to 16byte
 */
static void* allignedMalloc(int size)
{
	char* ptr = (char*)malloc(size + 16);
	ptr += 16ul - ((uint32_t)ptr & 0xf);
	return (void*)ptr;
}

int radLoadModule(const char* filename)
{
	FILE* fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (!fp)
	{
        printf("Could not open file.");
		return 0;
	}

	uint8_t* songData = (uint8_t*)allignedMalloc(len);
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
		return 0;
	}

	return 1;
}
