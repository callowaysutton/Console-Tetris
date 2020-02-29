// Console Tetris.cpp : Defines the entry point for the application.
//

#include "Console Tetris.h"

using namespace std;

wstring shapes[7];

int Setup() {
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
}

int main()
{
	Setup();
	cout << "Hello CMake." << endl;
	return 0;
}
