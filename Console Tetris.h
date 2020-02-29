// Console Tetris.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
#include <stdio.h>

int Rotate(int sx, int sy, int r) {
	switch (r % 4) {
	case 0: return sy * 4 + sx;
	case 1: return 12 + sy - (sx * 4);
	case 2: return 15 - (sy * 4) - sx;
	case 3: return 3 - sy + (sx * 4);
	}
}

bool DoesPieceFit(int Shape, int Rotation, int PosX, int PosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, Rotation);

			// Get index into field
			int fi = (PosY + py) * FieldWidth + (PosX + px);


			if (PosX + px >= 0 && PosX + px < FieldWidth)
			{
				if (PosY + py >= 0 && PosY + py < FieldHeight)
				{
					// In Bounds so do collision check
					if (shapes[Shape][pi] != L'.' && Field[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}