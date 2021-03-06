﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using NAudio.Midi;

namespace MidiToCode
{
	public static class Program
	{
		public static void Main()
		{
			StreamWriter file = File.CreateText(Path.Combine(Application.StartupPath, "Output", "Songs.h"));
			file.WriteLine("// File generated by MidiToCode.exe");
			foreach (FileInfo midiFile in new DirectoryInfo(Path.Combine(Application.StartupPath, "Input")).GetFiles("*.mid"))
			{
				string name = Path.GetFileNameWithoutExtension(midiFile.Name);

				MidiFile midi = new MidiFile(midiFile.FullName);
				List<string>[] notes = new List<string>[6];

				for (int channel = 0; channel < 6; channel++)
				{
					notes[channel] = new List<string>();
					List<int[]> noteList = new List<int[]>();

					int noteStart = 0, noteEnd = 0, noteNumber = 0;

					foreach (MidiEvent note in midi.Events[0])
					{
						if (note.Channel - 1 == channel)
						{
							if (note is TempoEvent)
							{
								int bpm = (int)(note as TempoEvent).Tempo;
								notes[channel].Add
								(
									"\t" +
									"0xff, " +
									"0x" + ((bpm & 0xff00) >> 8).ToString("x2") + ", " +
									"0x" + (bpm & 0xff).ToString("x2") + ", " +
									"0x00, " +
									"0x00, " +
									"0x00, " +
									"0x00,"
								);
							}
							else if (note.CommandCode == MidiCommandCode.NoteOn)
							{
								noteStart = (int)note.AbsoluteTime;
								noteNumber = (note as NoteOnEvent).NoteNumber;
								if (noteStart > 0x7fffff) throw new Exception();
							}
							else if (note.CommandCode == MidiCommandCode.NoteOff)
							{
								noteEnd = (int)note.AbsoluteTime;
								if (noteEnd > 0x7fffff) throw new Exception();

								notes[channel].Add
								(
									"\t" +
									"0x" + ((noteStart & 0xff0000) >> 16).ToString("x2") + ", " +
									"0x" + ((noteStart & 0xff00) >> 8).ToString("x2") + ", " +
									"0x" + (noteStart & 0xff).ToString("x2") + ", " +
									"0x" + ((noteEnd & 0xff0000) >> 16).ToString("x2") + ", " +
									"0x" + ((noteEnd & 0xff00) >> 8).ToString("x2") + ", " +
									"0x" + (noteEnd & 0xff).ToString("x2") + ", " +
									"0x" + noteNumber.ToString("x2") + ","
								);

								if (noteList.Any(itm => itm[0] > noteStart && itm[0] < noteEnd || itm[1] > noteStart && itm[1] < noteEnd || itm[0] < noteStart && itm[1] > noteEnd))
								{
									Console.WriteLine("Note overlap in " + name + " at position " + noteStart + " on channel " + channel + ".");
									Console.WriteLine("Exiting.");
									Console.ReadLine();
									return;
								}
								else
								{
									noteList.Add(new[] { noteStart, noteEnd });
								}
							}
						}
					}
				}

				file.WriteLine("CreateSong(" + name + ")");
				file.WriteLine("{");
				file.WriteLine
				(
					"\t" +
					"0x" + (midi.DeltaTicksPerQuarterNote >> 8).ToString("x2") + ", " +
					"0x" + (midi.DeltaTicksPerQuarterNote & 0xff).ToString("x2") + ","
				);
				for (int i = 0, count = 26; i < 6; i++)
				{
					file.WriteLine
					(
						"\t" +
						"0x" + (count >> 8).ToString("x2") + ", " +
						"0x" + (count & 0xff).ToString("x2") + ", " +
						"0x" + (notes[i].Count >> 8).ToString("x2") + ", " +
						"0x" + (notes[i].Count & 0xff).ToString("x2") + ",");
					count += notes[i].Count * 7;
				}
				for (int i = 0; i < 6; i++)
				{
					for (int a = 0; a < notes[i].Count - 1; a++)
					{
						file.WriteLine(notes[i][a]);
					}
					if (notes[i].Count > 0)
					{
						string line = notes[i][notes[i].Count - 1];
						file.WriteLine(i == 5 ? line.Substring(0, line.Length - 1) : line);
					}
				}
				file.WriteLine("};");
			}
			file.Close();
		}
	}
}