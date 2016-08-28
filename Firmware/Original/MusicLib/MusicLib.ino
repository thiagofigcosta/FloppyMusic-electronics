#include "MusicLibrary.h"
#include "Songs.h"

// Function		| Wire	| Floppy pin
// -------------|-------|-----------
// DriveSelect	| Red	| 14
// Step			| Green	| 20
// Dir			| Blue	| 18

#define DEBUG 0
#define DEBUG_SONG 100

int GetSongCount()
{
	return 8;
}
void PlaySongByID(int id)
{
	switch (id)
	{
		case 0:		PlaySong(Entertainer,		-12,	-12,	1.2,	.85);	break;
		case 1:		PlaySong(EverytimeWeTouch,	-24,	-24,	1,		.85);	break;
		case 2:		PlaySong(HesAPirate,		0,		-12,	1,		1);		break;
		case 3:		PlaySong(MorningSong,		-12,	-12,	1,		.85);	break;
		case 4:		PlaySong(NyanCat,			-24,	-24,	1.2,	.9);	break;
		case 5:		PlaySong(OdeToJoy,			-12,	-12,	1,		.85);	break;
		case 6:		PlaySong(Popcorn,			12,		-24,	1,		.8);	break;
		case 7:		PlaySong(Tetris,			0,		-12,	1,		.9);	break;
		case 100:	PlaySong(DriveTest,			-12,	-12,	1,		.85);	break;
	}
}

void setup()
{
	randomSeed(analogRead(0));
	random(100);
	AdjustHeads();

#if DEBUG
	PlaySongByID(DEBUG_SONG);
#else
	int *list = new int[GetSongCount()];
	for (int i = 0; i < GetSongCount(); i++) list[i] = -1;
	for (int i = 0; i < GetSongCount(); i++)
	{
		while (true)
		{
			int rand = random(GetSongCount());
			if (list[rand] == -1)
			{
				list[rand] = i;
				break;
			}
		}
	}

	for (int i = 0; ; i++)
	{
		PlaySongByID(list[i % GetSongCount()]);
	}
#endif
}

void loop()
{
}