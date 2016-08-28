#include "MusicLibrary.h"

int valueofsix=5;

void AdjustHeads()
{
          pinMode(ButtonPin,INPUT);
        digitalWrite(ButtonPin,HIGH);
	for (int i = 0; i < valueofsix; i++)
	{
		pinMode(Pinout[i].DriveSelect, OUTPUT);
		pinMode(Pinout[i].Step, OUTPUT);
		pinMode(Pinout[i].Dir, OUTPUT);
		digitalWrite(Pinout[i].DriveSelect, LOW);
		digitalWrite(Pinout[i].Dir, HIGH);
	}
	for (int i = 0; i < 80; i++)
	{
		for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].Step, LOW);
		delay(2);
		for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].Step, HIGH);
		delay(2);
	}
	for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].Dir, LOW);
	for (int i = 0; i < 30; i++)
	{
		for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].Step, HIGH);
		delay(2);
		for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].Step, LOW);
		delay(2);
	}
	for (int i = 0; i < valueofsix; i++) digitalWrite(Pinout[i].DriveSelect, HIGH);
}

void PlaySong(Song song, int noteOffsetLow, int noteOffsetHigh, float speedModifier, float noteCutoff)
{
	Channel *channels = new Channel[valueofsix];
	for (int i = 0; i < valueofsix; i++)
	{
		channels[i].CurrentPosition = channels[i].Start = GetSongElement(song, i * 4 + 2) << 8 | GetSongElement(song, i * 4 + 3);
		channels[i].NoteCount = GetSongElement(song, i * 4 + 4) << 8 | GetSongElement(song, i * 4 + 5);
		channels[i].IsPlaying = false;
		channels[i].IsPausing = false;
		channels[i].IsDone = false;
		channels[i].ActionEndTime = 0;
		channels[i].ActionCutOffTime = 0;
		channels[i].LastNoteEndTime = 0;
		channels[i].HeadPosition = 0;
		channels[i].NextHeadStepTime = 0;
	}

	int deltaPerQuater = GetSongElement(song, 0) << 8 | GetSongElement(song, 1);
	long speed = long(60000000 / 90 / deltaPerQuater / speedModifier);

	while (true)
	{
		if (channels[0].IsDone && channels[1].IsDone && channels[2].IsDone && channels[3].IsDone && channels[4].IsDone) break;
		
		long time = micros();
		for (int i = 0; i < valueofsix; i++)
		{
			Channel *channel = &channels[i];

			if (channel->IsPlaying)
			{
				if (time > channel->ActionCutOffTime)
				{
					digitalWrite(Pinout[i].DriveSelect, HIGH);
					if (time > channel->ActionEndTime)
					{
						channel->IsPlaying = false;
						channel->IsDone = channel->CurrentPosition >= channel->Start + channel->NoteCount * 7;
					}
				}
				else if (!channel->IsPausing && time > channel->NextHeadStepTime && time < channel->ActionCutOffTime)
				{
					channel->NextHeadStepTime = time + channel->Frequency;
					digitalWrite(Pinout[i].Dir, channel->HeadPosition & 2);
					digitalWrite(Pinout[i].Step, channel->HeadPosition & 1);
					channel->HeadPosition = (channel->HeadPosition + 1) & 3;
				}
			}
			if (!channel->IsPlaying && !channel->IsDone)
			{
				while (channel->CurrentPosition < channel->Start + channel->NoteCount * 7 && GetSongElement(song, channel->CurrentPosition) == 0xff)
				{
					int bpm = int(GetSongElement(song, channel->CurrentPosition + 1)) << 8 | int(GetSongElement(song, channel->CurrentPosition + 2));
					speed = long(60000000 / bpm / deltaPerQuater / speedModifier);
					channel->CurrentPosition += 7;
				}

				if (channel->IsPausing)
				{
					long start = long(GetSongElement(song, channel->CurrentPosition)) << 16 | long(GetSongElement(song, channel->CurrentPosition + 1)) << 8 | long(GetSongElement(song, channel->CurrentPosition + 2));
					long end = long(GetSongElement(song, channel->CurrentPosition + 3)) << 16 | long(GetSongElement(song, channel->CurrentPosition + 4)) << 8 | long(GetSongElement(song, channel->CurrentPosition + 5));
					int note = GetSongElement(song, channel->CurrentPosition + valueofsix);//deve ser 6
					channel->CurrentPosition += 6;

					channel->IsPlaying = true;
					channel->IsPausing = false;
					channel->ActionEndTime = time + (end - start) * speed;
					channel->ActionCutOffTime = time + long((end - start) * speed * noteCutoff);
					channel->LastNoteEndTime = end;
					channel->Frequency = long(1000000 / NoteFrequencies[note + (i < 3 ? noteOffsetLow : noteOffsetHigh)]);
					digitalWrite(Pinout[i].DriveSelect, LOW);
				}
				else
				{
					long start = long(GetSongElement(song, channel->CurrentPosition)) << 16 | long(GetSongElement(song, channel->CurrentPosition + 1)) << 8 | long(GetSongElement(song, channel->CurrentPosition + 2));

					channel->IsPlaying = true;
					channel->IsPausing = true;
					channel->ActionEndTime = time + (start - channel->LastNoteEndTime) * speed;
				}
			}
		}
	}

	delete[] channels;
}
