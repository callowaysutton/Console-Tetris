// Console Tetris.h : Include file for standard system include files,
// or project specific include files.

#pragma once

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

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	// All Field cells >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Check that test is in bounds. Note out of bounds does
			// not necessarily mean a fail, as the long vertical piece
			// can have cells that lie outside the boundary, so we'll
			// just ignore them
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					// In Bounds so do collision check
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}
		}

	return true;
}