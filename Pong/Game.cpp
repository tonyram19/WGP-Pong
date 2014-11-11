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
	//TODO: Create Back Buffer

	// Initialize DirectX.
	HRESULT hr = CreateGraphics(hWnd);
	if (FAILED(hr))
	{
		return; // Return -1 to abort Window Creation.
	}

	//TODO: Load Bitmaps/Brushes/Pens/Fonts

	//TODO: Initialize Game Components

	ballpos = D2D1::RectF(20, 20, 60, 60);
	ballvec = D2D1::SizeF(3, 3);

	paddlepos = D2D1::RectF(pRT->GetSize().width-30, 300, pRT->GetSize().width-10, 400);
	paddlevec = D2D1::SizeF(0, 4);

	bgpos = D2D1::RectF(0, 0, 1120, 800);
	bgvec = D2D1::SizeF(-4, 0);

	fgpos = D2D1::RectF(0, 0, 1120, 800);
	fgvec = D2D1::SizeF(-2, 0);
}

void Game::Shutdown(void)
{
	//TODO: Unload Bitmaps/Brushes/Pens/Fonts

	//TODO: Destroy Back Buffer

	// Shutdown DirectX.
	DestroyGraphics();
}

void Game::Input(void)
{
	//TODO: Read User Input
	if (keys[VK_ESCAPE] & 0x80)
	{
		PostMessage(hWnd, WM_CLOSE, 0, 0);
	}

	// Check if the W Key is pressed:
	if (keys['W'] & 0x80)
	{
		// TODO: W Key down action.
		ballpos = D2D1::RectF(20, 20, 60, 60);
		ballvec = D2D1::SizeF(3, 3);
	}

	if (keys[VK_UP] & 0x80)
	{
		// TODO: Up Arrow Key down action.
		paddlepos.top -= paddlevec.height;
		paddlepos.bottom -= paddlevec.height;
	}

	if (keys[VK_DOWN] & 0x80)
	{
		// TODO: Up Arrow Key down action.
		paddlepos.top += paddlevec.height;
		paddlepos.bottom += paddlevec.height;
	}
}

void Game::Simulate(void)
{
	//TODO: Simulate AI

	//TODO: Simulate Physics

	ballpos.left += ballvec.width;
	ballpos.top += ballvec.height;
	ballpos.right += ballvec.width;
	ballpos.bottom += ballvec.height;

	//Backgrounds
	bgpos.left += bgvec.width;
	bgpos.right += bgvec.width;

	fgpos.left += fgvec.width;
	fgpos.right += fgvec.width;

	//TODO: Simulate Collision Detection & Response

	if (ballpos.bottom > pRT->GetSize().height)
	{
		ballvec.height = -ballvec.height;
	}

	// Rectangle to Rectangle Intersection:
	RECT rIntersect;
	RECT rBall = {(LONG)ballpos.left, (LONG)ballpos.top, (LONG)ballpos.right, (LONG)ballpos.bottom};
	RECT rPaddle = {(LONG)paddlepos.left, (LONG)paddlepos.top, (LONG)paddlepos.right, (LONG)paddlepos.bottom};
	if(IntersectRect(&rIntersect, &rBall, &rPaddle))
	{
		// TODO: Intersection Response.
		ballvec.width = -ballvec.width;
	}
}

void Game::Render(void)
{
	pRT->BeginDraw();

	//TODO: Clear Back Buffer

	pRT->Clear(D2DColor(DarkSlateGray));

	//TODO: Render Game Components

	pRT->DrawBitmap(background1, bgpos);
	if (bgpos.right < 816)
	{
		ID2D1Bitmap* tempbmp = background1;
		D2D1_RECT_F temp = D2D1::RectF(bgpos.right, 0, bgpos.right + 1120, 800);
		pRT->DrawBitmap(tempbmp, temp);
		if (temp.left <= 0)
		{
			bgpos.left = 0;
			bgpos.right = 1120;

		}
	}

	pRT->DrawBitmap(background2, fgpos);
	if (fgpos.right < 816)
	{
		ID2D1Bitmap* tempbmp = background2;
		D2D1_RECT_F temp = D2D1::RectF(fgpos.right, 0, fgpos.right + 1120, 800);
		pRT->DrawBitmap(tempbmp, temp);
		if (temp.left <= 0)
		{
			fgpos.left = 0;
			fgpos.right = 1120;

		}
	}
		

//	pRT->DrawBitmap(background1, D2D1::RectF(0, 0, pRT->GetSize().width, pRT->GetSize().height));
//	pRT->DrawBitmap(background2, D2D1::RectF(0, 0, pRT->GetSize().width, pRT->GetSize().height));

	D2D1_ELLIPSE circle;
	circle.point.x = (ballpos.left+ballpos.right)/2;
	circle.point.y = (ballpos.top+ballpos.bottom)/2;
	circle.radiusX = (ballpos.right-ballpos.left)/2;
	circle.radiusY = (ballpos.bottom-ballpos.top)/2;
	pBrush->SetColor(D2DColor(Chartreuse));
	pRT->FillEllipse(circle, pBrush);

	// Render the Right Paddle.
	pBrush->SetColor(D2DColor(Maroon));
	pRT->FillRectangle(paddlepos, pBrush);

	//TODO: Present Back Buffer

	HRESULT hr = pRT->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		//TODO: Destroy Direct2D.
		DestroyGraphics();
		//TODO: Recreate Direct2D.
		CreateGraphics(hWnd);
	}
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

	//Background
	hr = LoadBitmapFromFile(L"bg.png", &background1);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load background.jpg."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	//Foreground
	hr = LoadBitmapFromFile(L"fg.png", &background2);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to load link.png."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	return S_OK; // Success!
}

void Game::DestroyGraphics(void)
{
	SafeRelease(&background1);
	SafeRelease(&background2);


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