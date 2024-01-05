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
HRESULT D2DOverlay::Initialize(bool isTransparent)
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
	CreateHwnd(&m_hwnd, isTransparent);
	//*hwnd = m_hwnd;

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


	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int windowWidth = 400;
	int windowHeight = 300;

	// Calculate window position
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

	//WS_EX_TOOLWINDOW WS_EX_TOPMOST WS_EX_TOOLWINDOW // WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOOLWINDOW
	DWORD flags = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOREDIRECTIONBITMAP;
	if (isTransparent) {
		flags |= WS_EX_TRANSPARENT;
	}

	// Create the application window.
	m_hwnd = CreateWindowEx(
		flags,
		L"Direct2dWindow",
		L"Overlay",
		WS_OVERLAPPED,
		x,
		y,
		windowWidth,
		windowHeight,
		NULL,				  // Parent window 
		NULL,						 // Menu
		HINST_THISCOMPONENT,  // Instance handle
		this						 // Additional application data
	);

	//Make no top border. WS_CHILD  WS_OVERLAPPED
	//SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPED);

	ShowWindow(m_hwnd, SW_NORMAL);
}



//
// The window message handler.
//
LRESULT CALLBACK D2DOverlay::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		D2DOverlay* pDirect2dWindow = (D2DOverlay*)pcs->lpCreateParams;

		::SetWindowLongPtr(
			hwnd,
			GWLP_USERDATA,
			(LONG_PTR)(pDirect2dWindow)
		);

		return 0;
	}
	else
	{
		D2DOverlay* pDirect2dWindow = reinterpret_cast<D2DOverlay*>(static_cast<LONG_PTR>(
			::GetWindowLongPtr(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDirect2dWindow)
		{
			switch (message)
			{


			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);


				//if (pDirect2dWindow->m_pSwapChain)
				{
					//pDirect2dWindow->ResizeWindow();
					//pDirect2dWindow->Destroy();
					//pDirect2dWindow->ReCreate();
					//pDirect2dWindow->ResizeWindow();
					//pDirect2dWindow->CreateDCompositionVisualTree();
					//pDirect2dWindow->OnResize(width, height);
				}

			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				//pDirect2dWindow->OnRenderComposite();
				EndPaint(hwnd, &ps);

				//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
			}
			result = 0;
			wasHandled = true;
			break;


			case WM_DESTROY:
			{
				//pDirect2dWindow->Destroy();
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_USER:
			{
				if (wParam == WM_KEYDOWN) {
					KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
					//LOG_INFO("WM_USER %d\n", kb->vkCode);
					//pDirect2dWindow->m_isActive = !pDirect2dWindow->m_isActive;
					InvalidateRgn(hwnd, 0, 0);
					UpdateWindow(hwnd);
				}
				break;
			}

			case WM_KEYDOWN:
				//LOG_INFO("KEY %lc\n", wParam);
				break;

			case WM_CHAR:
				//LOG_INFO("WM_CHAR %lc\n", wParam);
				//pDirect2dWindow->m_output->HandleKeyboardPress(wParam);
				break;

			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
				if (wParam & MK_LBUTTON) {
					//pDirect2dWindow->m_output->ProcessNewMouseClick(IMouseProcessor::eLeftButton, 1);
				}
				else
				{
					//pDirect2dWindow->m_output->ProcessNewMouseClick(IMouseProcessor::eLeftButton, 0);
				}
				break;

			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
				if (wParam & MK_RBUTTON) {
					//pDirect2dWindow->m_output->ProcessNewMouseClick(IMouseProcessor::eRightButton, 1);
				}
				else
				{
					//pDirect2dWindow->m_output->ProcessNewMouseClick(IMouseProcessor::eRightButton, 0);
				}
				break;


			case WM_MOUSEMOVE:
			{

			}
			break;

			case WM_DPICHANGED:
			{
				WORD g_dpi = HIWORD(wParam);
				//LOG_INFO("WM_DPICHANGED %d\n", g_dpi);
				break;
			}

			case WM_DISPLAYCHANGE:
			{
				//LOG_INFO("WM_DISPLAYCHANGE %d x %d\n", HIWORD(lParam), LOWORD(lParam));
				//pDirect2dWindow->ResizeWindow();
				break;
			}



			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}


//
// The main window message loop.
//
void D2DOverlay::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, m_hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_DESTROY) {
			break;
		}
	}
}
