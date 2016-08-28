#include "MusicLibrary.h"
#include "Songs.h"

int GetSongCount()
{
	return 2;
}
void PlaySongByID(int id)
{
	switch (id)
	{

		case 0:		PlaySong(Simpsons,		-12,	-12,	1.2,	.85);	break;
		case 1:		PlaySong(ImperialMarch,	-24,	-24,	1,		.85);	break;
		case 2:		PlaySong(CenturyFox,		0,		-12,	1,		1);		break;
		case 3:		PlaySong(Brazil,		-12,	-12,	1,		.85);	break;
		case 4:		PlaySong(zerozeroseven,			-24,	-24,	1.2,	.9);	break;
		case 5:		PlaySong(Pokemon,			-12,	-12,	1,		.85);	break;
		case 6:		PlaySong(HarryPotter,			12,		-24,	1,		.8);	break;
		case 7:		PlaySong(IndianaJones,			0,		-12,	1,		.9);	break;
		case 8:		PlaySong(Matrix,			0,		-12,	1,		.9);	break;
		case 9:		PlaySong(MissionImpossible,			0,		-12,	1,		.9);	break;
		case 10:	PlaySong(Xfiles,			0,		-12,	1,		.9);	break;
		case 11:	PlaySong(MortalKombat,			0,		-12,	1,		.9);	break;
		case 12:	PlaySong(NyanCat,			-24,	-24,	1.2,	.9);	break;
		case 13:	PlaySong(Zelda,			0,		-12,	1,		.9);	break;
		case 14:	PlaySong(PiratasDoCaribe,			0,		-12,	1,		.9);	break;
		case 15:	PlaySong(Tetris,			0,		-12,	1,		.9);	break;
		case 16:	PlaySong(Godfather,			0,		-12,	1,		.9);	break;
		case 17:	PlaySong(OdeToJoy,			0,		-12,	1,		.9);	break;
		case 18:	PlaySong(MorningSong,			0,		-12,	1,		.9);	break;
		case 19:	PlaySong(DriveTest,			0,		-12,	1,		.9);	break;
	}
}

void setup()
{

	AdjustHeads();
PlaySongByID(12);PlaySongByID(12);

PlaySongByID(0);
PlaySongByID(1);
PlaySongByID(2);
PlaySongByID(3);
PlaySongByID(4);
PlaySongByID(5);
PlaySongByID(6);
PlaySongByID(7);
PlaySongByID(8);
PlaySongByID(9);
PlaySongByID(10);
PlaySongByID(11);
PlaySongByID(12);
PlaySongByID(13);
PlaySongByID(14);
PlaySongByID(15);
PlaySongByID(16);
PlaySongByID(17);
PlaySongByID(18);
PlaySongByID(19);

}

void loop()
{
}

//
//		case 0:		PlaySong(Entertainer,		-12,	-12,	1.2,	.85);	break;
//		//case 1:		PlaySong(EverytimeWeTouch,	-24,	-24,	1,		.85);	break;
//		//case 2:		PlaySong(HesAPirate,		0,		-12,	1,		1);		break;
//		case 3:		PlaySong(MorningSong,		-12,	-12,	1,		.85);	break;
//		case 4:		PlaySong(NyanCat,			-24,	-24,	1.2,	.9);	break;
//		case 5:		PlaySong(OdeToJoy,			-12,	-12,	1,		.85);	break;
//		//case 6:		PlaySong(Popcorn,			12,		-24,	1,		.8);	break;
//		case 7:		PlaySong(Tetris,			0,		-12,	1,		.9);	break;
//		case 100:	PlaySong(DriveTest,			-12,	-12,	1,		.85);	break;
