#pragma once

//Includes
#if 1
#include <Windows.h> // Include the Win32 API
#include <tchar.h> // Include support for UNICODE
#include <sstream> // Include wostringstream support
using std::wostringstream;

#include <d2d1.h> // Include the Direct2D API.
#pragma comment(lib, "d2d1.lib") // Connect to the Direct2D Import Library.

#include <dwrite.h> // Include the DirectWrite API.
#pragma comment(lib, "dwrite.lib") // Connect to the DirectWrite Import Library.

#include <mmsystem.h> // Include the Multimedia header file.
#pragma comment(lib, "winmm.lib") // Connect to the Multimedia import library.

// WIC
#include <wincodec.h>
#pragma comment (lib, "windowscodecs.lib")  
#endif 


class Game
{

//Stuff
#if 1
	// Window Handle:
	HWND hWnd;

	// DirectX Helper Functions

	// The SafeRelease Pattern
	template <class T> void SafeRelease(T **ppT)
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	// Programmer defined macro to make using the known colors easier.
#define D2DColor(clr) D2D1::ColorF(D2D1::ColorF::clr)

	HRESULT CreateGraphics(HWND hWnd);
	void DestroyGraphics(void);

	// DirectX Interfaces:
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush *pBrush;

	D2D1_COLOR_F custom;
	D2D1_POINT_2F ptmouse;

	// DirectWrite Interfaces:
	IDWriteFactory* pDWFactory;
	IDWriteTextFormat* pTF;
#endif 


	/****************** Game Components **********************/

	// WIC Interface
	IWICImagingFactory* pWICFactory;
	ID2D1Bitmap* bg1;
	ID2D1Bitmap* bg2;
	ID2D1Bitmap* bg3;
	ID2D1Bitmap* ball;
	ID2D1Bitmap* ship;
	ID2D1Bitmap* enemy;



	//Background 1
	D2D1_RECT_F bg1pos;
	D2D1_SIZE_F bg1vec;

	//Background 2
	D2D1_RECT_F bg2pos;
	D2D1_SIZE_F bg2vec;

	//Background 3
	D2D1_RECT_F bg3pos;
	D2D1_SIZE_F bg3vec;

	//Ball
	float ballSpriteHeight;
	float ballSpriteWidth;
	int ballAnimationFramesX;
	int ballAnimationFramesY;
	D2D1_RECT_F ballpos;
	D2D1_SIZE_F ballvec;
	float ballFrameWidth;
	float ballFrameHeight;
	DWORD dwTimeBall;


	// Ship 
	float shipSpriteHeight;
	float shipSpriteWidth;
	int shipAnimationFrames;
	D2D1_RECT_F shipPos;
	D2D1_SIZE_F shipVec;
	float shipFrameWidth;
	float shipFrameHeight;
	DWORD dwTimeShip;

	// Enemy 
	float enemySpriteHeight;
	float enemySpriteWidth;
	int enemyAnimationFramesX;
	int enemyAnimationFramesY;
	D2D1_RECT_F enemyPos;
	D2D1_SIZE_F enemyVec;
	float enemyFrameWidth;
	float enemyFrameHeight;
	DWORD dwTimeEnemy;

	//Other
	bool ballCollided = false;

	// Variables used to calculate frames per second:
	DWORD dwFrames;
	DWORD dwCurrentTime;
	DWORD dwLastUpdateTime;
	DWORD dwElapsedTime;
	std::wstring szFPS;

public:

	BOOL bRunning;		// Is the game running?
	HANDLE hGameThread; // Handle to the GameMain thread.
	BYTE keys[256];		// Current state of the keyboard.

	Game(void);
	virtual ~Game(void);

	void Startup(void);
	void Shutdown(void);
	void Input(void);
	void Simulate(void);
	void Render(void);

	void SetHwnd(HWND _hWnd) {hWnd = _hWnd;}
	HRESULT LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap);
};
