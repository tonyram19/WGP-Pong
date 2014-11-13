#include "Game.h"

Game::Game(void)
{
	bRunning = TRUE;
	hWnd = NULL;
	ZeroMemory(keys, sizeof(keys));

	pD2DFactory = NULL;
	pRT = NULL;
	pBrush = NULL;
	pDWFactory = NULL;
	pTF = NULL;

	// Zero out the frames per second variables:
	dwFrames = 0;
	dwCurrentTime = 0;
	dwLastUpdateTime = 0;
	dwElapsedTime = 0;
	szFPS = _T("");
}

Game::~Game(void)
{
}

void Game::Startup(void)
{

#if 1
	HRESULT hr = CreateGraphics(hWnd);
	if (FAILED(hr))
	{
		return;
	}
#endif 

	/***************Initialize Game Components*******************/

	//Ship 
	shipSpriteHeight = ship->GetSize().height;
	shipSpriteWidth = ship->GetSize().width;
	shipAnimationFrames = 0;
	shipFrameWidth = shipSpriteWidth;
	shipFrameHeight = shipSpriteHeight / 4;
	shipPos = D2D1::RectF(25, 40, 25 + shipFrameWidth, 40 + shipFrameHeight);
	shipVec = D2D1::SizeF(0, 6);
	dwTimeShip = GetTickCount() + 100;

	//Enemy 
	enemySpriteHeight = enemy->GetSize().height;
	enemySpriteWidth = enemy->GetSize().width;
	enemyAnimationFramesX = 0;
	enemyAnimationFramesY = 0;
	enemyFrameWidth = 64;
	enemyFrameHeight = 64;
	enemyPos = D2D1::RectF(720, 40, 720 + enemyFrameWidth, 40 + enemyFrameHeight);;
	enemyVec = D2D1::SizeF(0, 6);
	dwTimeEnemy = GetTickCount() + 100;

	//Ball
	ballSpriteHeight = 128;
	ballSpriteWidth = ball->GetSize().width;
	enemyAnimationFramesX = 0;
	enemyAnimationFramesY = 1;
	ballFrameWidth = 64;
	ballFrameHeight = 64;
	ballpos = D2D1::RectF(400, 20, 400 + ballFrameWidth, 20 + enemyFrameHeight);
	ballvec = D2D1::SizeF(4, 4);
	dwTimeBall = GetTickCount() + 100;

	//Background 1
	bg1pos = D2D1::RectF(0, 0, 1120, 800);
	bg1vec = D2D1::SizeF(-1, 0);

	//Background 2
	bg2pos = D2D1::RectF(0, 0, 1120, 800);
	bg2vec = D2D1::SizeF(-3, 0);

	//Background 3
	bg3pos = D2D1::RectF(0, 0, 1120, 800);
	bg3vec = D2D1::SizeF(-5, 0);
}


void Game::Input(void)
{
	
	if (keys[VK_ESCAPE] & 0x80)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	if (keys['W'] & 0x80)
	{
		
		if (shipPos.top < 0)
		{
			shipPos.bottom += 0;
			shipPos.top += 0;
		}
		else
		{
			shipPos.top -= shipVec.height;
			shipPos.bottom -= shipVec.height;
		}

	}

	if (keys['S'] & 0x80)
	{

		if (shipPos.bottom > pRT->GetSize().height)
		{
			shipPos.bottom += 0;
			shipPos.top += 0;
		}
		else
		{
			shipPos.top += shipVec.height;
			shipPos.bottom += shipVec.height;
		}

	}


}

void Game::Simulate(void)
{

	//Ball
	ballpos.left += ballvec.width;
	ballpos.top += ballvec.height;
	ballpos.right += ballvec.width;
	ballpos.bottom += ballvec.height;

	if (GetTickCount() > dwTimeBall)
	{
		ballAnimationFramesX++;
		dwTimeBall = GetTickCount() + 100;
	}

	if (ballAnimationFramesX == 5)
	{
		ballAnimationFramesX = 0;
		ballAnimationFramesY++;
	}

	if (ballAnimationFramesY == 2)
	{
		ballAnimationFramesY = 1;
	}

	//Backgrounds
	bg1pos.left += bg1vec.width;
	bg1pos.right += bg1vec.width;

	bg2pos.left += bg2vec.width;
	bg2pos.right += bg2vec.width;

	bg3pos.left += bg3vec.width;
	bg3pos.right += bg3vec.width;

	//Ship
	if (GetTickCount() > dwTimeShip)
	{
		shipAnimationFrames++;
		dwTimeShip = GetTickCount() + 100;
	}

	if (shipAnimationFrames == 4)
	{
		shipAnimationFrames = 0;
	}

	//Enemy
	if (GetTickCount() > dwTimeEnemy)
	{
		enemyAnimationFramesX++;
		dwTimeEnemy = GetTickCount() + 100;
	}

	if (enemyAnimationFramesX == 4)
	{
		enemyAnimationFramesX = 0;
	}


	//Ball - Borders collision
	if (ballpos.bottom > pRT->GetSize().height)
		ballvec.height = -ballvec.height;

	if (ballpos.top < 0)
		ballvec.height = -ballvec.height;

	if (ballpos.left < 0)
		ballvec.width = -ballvec.width;

	if (ballpos.right > pRT->GetSize().width)
		ballvec.width = -ballvec.width;

	//Ball - Enemy collision
	if ( 
	  (ballpos.right > enemyPos.left) 
	  && (ballpos.left < enemyPos.right) 
	  && (ballpos.top < enemyPos.bottom) 
	  && (ballpos.bottom > enemyPos.top)
	)
	{
		ballvec.height = -ballvec.height;
		ballvec.width = -ballvec.width;
	}

	//Ball - Ship collision
	if (
		(ballpos.right >= shipPos.left)
		&& (ballpos.left <= shipPos.right)
		&& (ballpos.top <= shipPos.bottom)
		&& (ballpos.bottom >= shipPos.top)
		)
	{
		ballvec.height = -ballvec.height;
		ballvec.width = -ballvec.width;

		shipPos.left += 0;
		shipPos.right += 0;
	}

}

void Game::Render(void)
{
	pRT->BeginDraw();
	

	//Background 1
	pRT->Clear(D2DColor(DarkSlateGray));
	pRT->DrawBitmap(bg1, bg1pos);
	if (bg1pos.right < 816)
	{
		ID2D1Bitmap* tempbmp = bg1;
		D2D1_RECT_F temp = D2D1::RectF(bg1pos.right, 0, bg1pos.right + 1120, 800);
		pRT->DrawBitmap(tempbmp, temp);
		if (temp.left <= 0)
		{
			bg1pos.left = 0;
			bg1pos.right = 1120;
		}
	}

	//Background 2
	pRT->DrawBitmap(bg2, bg2pos);
	if (bg2pos.right < 816)
	{
		ID2D1Bitmap* tempbmp = bg2;
		D2D1_RECT_F temp = D2D1::RectF(bg2pos.right, 0, bg2pos.right + 1120, 800);
		pRT->DrawBitmap(tempbmp, temp);
		if (temp.left <= 0)
		{
			bg2pos.left = 0;
			bg2pos.right = 1120;
		}
	}

	//Auxiliary temporary rects
	D2D1_RECT_F destRect;
	D2D1_RECT_F srcRect;
	pBrush->SetColor(D2DColor(Red));
	
	//Ship	
	destRect.left = shipPos.left;
	destRect.top = shipPos.top;
	destRect.right = destRect.left + shipFrameWidth;
	destRect.bottom = destRect.top + shipFrameHeight;

	srcRect.left = 0;
	srcRect.top = shipAnimationFrames * shipFrameHeight;
	srcRect.right = srcRect.left + shipSpriteWidth;
	srcRect.bottom = srcRect.top + shipFrameHeight;
	
	pRT->DrawBitmap(ship, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
	
#if 0
	destRect.left = shipPos.left;
	destRect.top = shipPos.top;
	destRect.right = shipPos.right;
	destRect.bottom = shipPos.bottom;
	pRT->DrawRectangle(destRect, pBrush, 1);
#endif // Debug rect


	//Enemy
	destRect.left = enemyPos.left;
	destRect.top = enemyPos.top;
	destRect.right = destRect.left + (enemyFrameWidth);
	destRect.bottom = destRect.top + (enemyFrameHeight);

	srcRect.left = enemyAnimationFramesX * enemyFrameWidth;
	srcRect.top = 0; 
	srcRect.right = srcRect.left + enemyFrameWidth;
	srcRect.bottom = srcRect.top + enemyFrameHeight;

	pRT->SetTransform(D2D1::Matrix3x2F::Scale(-2.0f, 2.0f, D2D1::Point2F(enemyPos.left + ((enemyPos.right - enemyPos.left) / 2), enemyPos.top + ((enemyPos.bottom - enemyPos.top) / 2))));
	pRT->DrawBitmap(enemy, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

	pRT->SetTransform(D2D1::Matrix3x2F::Identity());

#if 0
	destRect.left = enemyPos.left;
	destRect.top = enemyPos.top;
	destRect.right = enemyPos.right;
	destRect.bottom = enemyPos.bottom;
	pRT->DrawRectangle(destRect, pBrush, 1);
#endif // Debug rect


	//Ball
	destRect.left =  ballpos.left;
	destRect.top = ballpos.top;
	destRect.right = destRect.left + (ballFrameWidth);
	destRect.bottom = destRect.top + (ballFrameHeight);

	srcRect.left = ballAnimationFramesX * ballFrameWidth;
	srcRect.top = ballAnimationFramesY * ballFrameHeight;
	srcRect.right = srcRect.left + ballFrameWidth;
	srcRect.bottom = srcRect.top + ballFrameHeight;

	pRT->DrawBitmap(ball, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

#if 0
	destRect.left = ballpos.left;
	destRect.top = ballpos.top;
	destRect.right = ballpos.right;
	destRect.bottom = ballpos.bottom;
	pRT->DrawRectangle(destRect, pBrush, 1);
#endif // Debug rect


	//Background 3
	pRT->DrawBitmap(bg3, bg3pos, (FLOAT)0.8);
	if (bg3pos.right < 816)
	{
		ID2D1Bitmap* tempbmp = bg3;
		D2D1_RECT_F temp = D2D1::RectF(bg3pos.right, 0, bg3pos.right + 1120, 800);
		pRT->DrawBitmap(tempbmp, temp, (FLOAT)0.8);
		if (temp.left <= 0)
		{
			bg3pos.left = 0;
			bg3pos.right = 1120;

		}
	}

	
#if 1
	HRESULT hr = pRT->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		//TODO: Destroy Direct2D.
		DestroyGraphics();
		//TODO: Recreate Direct2D.
		CreateGraphics(hWnd);
	}
#endif

}


HRESULT Game::CreateGraphics(HWND hWnd)
{
	// Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Factory."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Get the dimensions of the client.
	RECT rc;
	GetClientRect(hWnd, &rc);

	// Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);


	// Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	pRT->SetDpi(96.0f, 96.0f);


	// Create the Direct2D Solid Color Brush.
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(0x0), &pBrush);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		(IUnknown**)&pDWFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	// Create the DirectWrite Text Format.
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		24, _T(""), &pTF);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}
	//pTF->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	//pTF->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	//pTF->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// Create the WIC Interface
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pWICFactory));
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the WIC Interface."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	//Spritesheets
	hr = LoadBitmapFromFile(L"ship.png", &ship);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load ship.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = LoadBitmapFromFile(L"ball.png", &ball);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load ball.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = LoadBitmapFromFile(L"enemy.png", &enemy);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load enemy.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	//Backgrounds
	hr = LoadBitmapFromFile(L"bg1.png", &bg1);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load bg1.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = LoadBitmapFromFile(L"bg2.png", &bg2);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load bg2.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	hr = LoadBitmapFromFile(L"bg3.png", &bg3);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load bg3.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	return S_OK; // Success!
}

void Game::DestroyGraphics(void)
{
	SafeRelease(&bg1);
	SafeRelease(&bg2);
	SafeRelease(&bg3);
	SafeRelease(&ship);
	SafeRelease(&enemy);
	SafeRelease(&ball);


	SafeRelease(&pTF);
	SafeRelease(&pDWFactory);
	SafeRelease(&pBrush);
	SafeRelease(&pRT);
	// Release the Direct2D Factory.
	SafeRelease(&pD2DFactory);
}

HRESULT Game::LoadBitmapFromFile(LPCTSTR strFileName, ID2D1Bitmap** ppBitmap)
{
	HRESULT hr;

	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICFormatConverter *pConverter = NULL;


	hr = pWICFactory->CreateDecoderFromFilename(
		strFileName,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);
	if (SUCCEEDED(hr))
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
			);
	}
	if (SUCCEEDED(hr))
	{
		// Create a Direct2D bitmap from the WIC bitmap.
		hr = pRT->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
			);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pConverter);

	return hr;
}

void Game::Shutdown(void)
{
	//TODO: Unload Bitmaps/Brushes/Pens/Fonts

	//TODO: Destroy Back Buffer

	// Shutdown DirectX.
	DestroyGraphics();
}