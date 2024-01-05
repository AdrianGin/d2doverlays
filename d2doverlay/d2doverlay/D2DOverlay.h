#pragma once

#include "stdafx.h"

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows 7 or later.
#define WINVER 0x0700       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows 7 or later.
#define _WIN32_WINNT 0x0700 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <dcomp.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <wrl.h>
#include <dxgi1_3.h>
#include <d3d11.h>
#include <d3d11_2.h>
#include <atlcomcli.h>


/******************************************************************
*                                                                 *
*  Macros                                                         *
*                                                                 *
******************************************************************/
template<class Interface>
inline void
SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


class D2DOverlay
{
public:
	D2DOverlay(float renderScale, HWND parent = NULL);
	~D2DOverlay();

	HRESULT D2DOverlay::Initialize(HWND* hwnd, bool isTransparent);

private:
	HWND m_hwnd;
	HWND m_parent;
	ID2D1Factory* m_pD2DFactory;
	IWICImagingFactory* m_pWICFactory;
	IDWriteFactory* m_pDWriteFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1PathGeometry* m_pPathGeometry;
	ID2D1LinearGradientBrush* m_pLinearGradientBrush;
	ID2D1SolidColorBrush* m_pBlackBrush;

	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;
	ID2D1Bitmap* m_pBitmap;
	ID2D1Bitmap* m_pAnotherBitmap;

	float m_renderScale;
};

