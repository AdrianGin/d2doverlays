#include "stdafx.h"
#include "D2DOverlay.h"


D2DOverlay::D2DOverlay(float renderScale, HWND parent) :
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pWICFactory(NULL),
	m_pDWriteFactory(NULL),
	m_pRenderTarget(NULL),
	m_pTextFormat(NULL),
	m_pPathGeometry(NULL),
	m_pLinearGradientBrush(NULL),
	m_pBlackBrush(NULL),
	m_pGridPatternBitmapBrush(NULL),
	m_pBitmap(NULL),
	m_pAnotherBitmap(NULL),
	m_renderScale(renderScale),
	m_parent(parent)
{
	CoInitialize(NULL);
}

D2DOverlay::~D2DOverlay()
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pWICFactory);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pPathGeometry);
	SafeRelease(&m_pLinearGradientBrush);
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pGridPatternBitmapBrush);
	SafeRelease(&m_pBitmap);
	SafeRelease(&m_pAnotherBitmap);

	CoUninitialize();
}


//
// Creates the application window and initializes
// device-independent resources.
//
HRESULT D2DOverlay::Initialize(HWND* hwnd, bool isTransparent)
{
	HRESULT hr = S_OK;
	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	//HR(CreateDeviceIndependentResources());
	// Register the window class.
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = L"Direct2dWindow";
	RegisterClassEx(&wcex);

	// Create the application window.
	CreateHwnd(hwnd, isTransparent);
	*hwnd = m_hwnd;

	//CreateD3D11Device();
	//CreateDCompositionDevice();
	//CreateDCompositionRenderTarget();
	//CreateDResources();
	//CreateDCompositionVisualTree();
	//CreateTransforms();

	return hr;
}


void D2DOverlay::CreateHwnd(HWND* hwnd, bool isTransparent)
{
	int left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int h = GetSystemMetrics(SM_CYVIRTUALSCREEN);


	//WS_EX_TOOLWINDOW WS_EX_TOPMOST WS_EX_TOOLWINDOW // WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOOLWINDOW
	DWORD flags = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOOLWINDOW;
	if (isTransparent) {
		flags |= WS_EX_TRANSPARENT;
	}

	// Create the application window.
	m_hwnd = CreateWindowEx(
		flags,
		L"Direct2dWindow",
		L"Swiftpoint Overlay",
		WS_POPUP,
		left,
		top,
		w,
		h,
		NULL,				  // Parent window 
		NULL,						 // Menu
		HINST_THISCOMPONENT,  // Instance handle
		this						 // Additional application data
	);

	//Make no top border. WS_CHILD  WS_OVERLAPPED
	SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_CHILD);

	//ShowWindow(m_hwnd, SW_NORMAL);
}
