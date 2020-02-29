// Console Tetris.cpp : Defines the entry point for the application.
//

#include "Console Tetris.h"

using namespace std;

wstring shapes[7];

//Setup Vars
int ScreenWidth;
int ScreenHeight;
int FieldWidth = 12;
int FieldHeight = 18;
unsigned char* Field = nullptr;
HANDLE Console;
DWORD BytesWritten;
wchar_t* screen;

//Logistics Vars
bool Key[4];
int CurrentPiece = 0;
int CurrentRotation = 0;
int CurrentX = FieldWidth / 2;
int CurrentY = 0;
int Speed = 20;
int SpeedCount = 0;
bool ForceDown = false;
bool RotateHold = true;
int PieceCount = 0;
int Score = 0;
vector<int> Lines;
bool GameOver = false;



void Setup() {
	//Create the shapes 
	//Stick
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");
	shapes[0].append(L"..X.");

	//Right leaning spiral
	shapes[1].append(L"..X.");
	shapes[1].append(L".XX.");
	shapes[1].append(L".X..");
	shapes[1].append(L"....");

	//Left leeaning spiral
	shapes[2].append(L".X..");
	shapes[2].append(L".XX.");
	shapes[2].append(L"..X.");
	shapes[2].append(L"....");

	//Square
	shapes[3].append(L"....");
	shapes[3].append(L".XX.");
	shapes[3].append(L".XX.");
	shapes[3].append(L"....");

	//Platform?
	shapes[4].append(L"..X.");
	shapes[4].append(L".XX.");
	shapes[4].append(L"..X.");
	shapes[4].append(L"....");

	//L facing left
	shapes[5].append(L"....");
	shapes[5].append(L".XX.");
	shapes[5].append(L"..X.");
	shapes[5].append(L"..X.");

	//L facing right
	shapes[6].append(L"....");
	shapes[6].append(L".XX.");
	shapes[6].append(L".X..");
	shapes[6].append(L".X..");

	Field = new unsigned char[FieldWidth * FieldHeight];
	for (int x = 0; x < FieldWidth; x++)
		for (int y = 0; y < FieldHeight; y++)
			Field[y * FieldWidth + x] = (x == 0 || x == FieldWidth - 1 || y == FieldHeight - 1) ? 9 : 0;
}

void Input() {
	for (int k = 0; k < 4; k++)								// R   L   D Z
		Key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
}

void Logic() {
	CurrentX += (Key[0] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX + 1, CurrentY)) ? 1 : 0;
	CurrentX -= (Key[1] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX - 1, CurrentY)) ? 1 : 0;
	CurrentY += (Key[2] && DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1)) ? 1 : 0;

	// Rotate, but latch to stop wild spinning
	if (Key[3])
	{
		CurrentRotation += (RotateHold && DoesPieceFit(CurrentPiece, CurrentRotation + 1, CurrentX, CurrentY)) ? 1 : 0;
		RotateHold = false;
	}
	else
		RotateHold = true;

	// Force the piece down the playfield if it's time
	if (ForceDown)
	{
		// Update difficulty every 50 pieces
		SpeedCount = 0;
		PieceCount++;
		if (PieceCount % 50 == 0)
			if (Speed >= 10) Speed--;

		// Test if piece can be moved down
		if (DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY + 1))
			CurrentY++; // It can, so do it!
		else
		{
			// It can't! Lock the piece in place
			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
					if (shapes[CurrentPiece][Rotate(px, py, CurrentRotation)] != L'.')
						Field[(CurrentY + py) * FieldWidth + (CurrentX + px)] = CurrentPiece + 1;

			// Check for lines
			for (int py = 0; py < 4; py++)
				if (CurrentY + py < FieldHeight - 1)
				{
					bool bLine = true;
					for (int px = 1; px < FieldWidth - 1; px++)
						bLine &= (Field[(CurrentY + py) * FieldWidth + px]) != 0;

					if (bLine)
					{
						// Remove Line, set to =
						for (int px = 1; px < FieldWidth - 1; px++)
							Field[(CurrentY + py) * FieldWidth + px] = 8;
						Lines.push_back(CurrentY + py);
					}
				}

			Score += 25;
			if (!Lines.empty())	Score += (1 << Lines.size()) * 100;

			// Pick New Piece
			CurrentX = FieldWidth / 2;
			CurrentY = 0;
			CurrentRotation = 0;
			CurrentPiece = rand() % 7;

			// If piece does not fit straight away, game over!
			GameOver = !DoesPieceFit(CurrentPiece, CurrentRotation, CurrentX, CurrentY);
		}
	}
}

void Draw() {
	for (int x = 0; x < FieldWidth; x++)
		for (int y = 0; y < FieldHeight; y++)
			screen[(y + 2) * ScreenWidth + (x + 2)] = L" ABCDEFG=#"[Field[y * FieldWidth + x]];

	// Draw Current Piece
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
			if (shapes[CurrentPiece][Rotate(px, py, CurrentRotation)] != L'.')
				screen[(CurrentY + py + 2) * ScreenWidth + (CurrentX + px + 2)] = CurrentPiece + 65;

	// Draw Score
	swprintf_s(&screen[2 * ScreenWidth + FieldWidth + 6], 16, L"SCORE: %8d", Score);

	// Animate Line Completion
	if (!Lines.empty())
	{
		// Display Frame (cheekily to draw lines)
		WriteConsoleOutputCharacter(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &BytesWritten);
		this_thread::sleep_for(400ms); // Delay a bit

		for (auto& v : Lines)
			for (int px = 1; px < FieldWidth - 1; px++)
			{
				for (int py = v; py > 0; py--)
					Field[py * FieldWidth + px] = Field[(py - 1) * FieldWidth + px];
				Field[px] = 0;
			}

		Lines.clear();
	}

	// Display Frame
	WriteConsoleOutputCharacter(Console, screen, ScreenWidth * ScreenHeight, { 0,0 }, &BytesWritten);

}

int main()
{
	screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenWidth * ScreenHeight; i++) screen[i] = L' ';
	Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	BytesWritten = 0;
	Setup();
	while (!GameOver) {
		Input();
		Logic();
		Draw();
		this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
		SpeedCount++;
		ForceDown = (SpeedCount == Speed);
	}
	CloseHandle(Console);
	cout << "Game Over!! Score:" << Score << endl;
	system("pause");
	return 0;
}
