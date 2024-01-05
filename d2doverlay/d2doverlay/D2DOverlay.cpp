#include "stdafx.h"
#include "D2DOverlay.h"
using namespace Microsoft::WRL;


void HR(HRESULT const result)
{
	if (S_OK != result)
	{
		
	}
}

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

	CreateD3D11Device();
	CreateDCompositionDevice();
	CreateDCompositionRenderTarget();
	CreateDResources();
	CreateDCompositionVisualTree();
	//CreateTransforms();

	InvalidateRgn(m_hwnd, 0, 0);

	return hr;
}


void D2DOverlay::CreateHwnd(HWND* hwnd, bool isTransparent)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int windowWidth = 400;
	int windowHeight = 300;

	// Calculate window position
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;

	//WS_EX_TOOLWINDOW WS_EX_TOPMOST WS_EX_TOOLWINDOW // WS_EX_NOREDIRECTIONBITMAP | WS_EX_TOOLWINDOW
	DWORD flags = WS_EX_OVERLAPPEDWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOREDIRECTIONBITMAP;
	if (isTransparent) {
		flags |= WS_EX_TRANSPARENT;
	}

	// Create the application window.
	m_hwnd = CreateWindowEx(
		flags,
		L"Direct2dWindow",
		L"Overlay",
		WS_POPUP,
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
	SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPED);

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
				pDirect2dWindow->OnRenderComposite();
				EndPaint(hwnd, &ps);

				//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
			}
			result = 0;
			wasHandled = true;
			break;


			case WM_DESTROY:
			{
				PostQuitMessage(0);
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

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_DESTROY) {
			break;
		}

		if (msg.message == WM_QUIT) {
			break;
		}
	}
}


HRESULT D2DOverlay::OnRenderComposite()
{
	HRESULT hr = ((m_pDevice == nullptr) || (m_hwnd == NULL)) ? E_UNEXPECTED : S_OK;
	if (hr != S_OK) {
		return S_OK;
	}

	bool redraw = renderer.Render(m_dc);
	if (redraw) {
		m_pSwapChain->Present(0, 0); // flags
		HR(m_pDevice->Commit());
	}

	return S_OK;
}




HRESULT D2DOverlay::CreateD3D11Device()
{
	HRESULT hr = S_OK;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
	};

	D3D_FEATURE_LEVEL featureLevelSupported;

	//for (int i = 0; i < sizeof(driverTypes) / sizeof(driverTypes[0]); ++i)
	{
		CComPtr<ID3D11DeviceContext> d3d11DeviceContext;

		HR(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			//D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,
			NULL,
			0,
			D3D11_SDK_VERSION,
			m_d3d11Device.ReleaseAndGetAddressOf(),
			nullptr,
			m_d3d11Context.ReleaseAndGetAddressOf()));


		ComPtr<IDXGIDevice> dxgiDevice;
		HR(m_d3d11Device->QueryInterface(dxgiDevice.GetAddressOf()));

		ComPtr<IDXGIFactory2> dxFactory;
#ifdef DEBUG
		HR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(dxFactory), (void**)dxFactory.GetAddressOf()));
#else
		HR(CreateDXGIFactory2(0, __uuidof(dxFactory), (void**)dxFactory.GetAddressOf()));
#endif
		DXGI_SWAP_CHAIN_DESC1 description = {};
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		description.BufferCount = 2;
		description.SampleDesc.Count = 1;
		description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
		description.Scaling = DXGI_SCALING_STRETCH;
		description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		RECT rect = {};
		GetWindowRect(m_hwnd, &rect);
		description.Width = rect.right - rect.left;
		description.Height = rect.bottom - rect.top;

		description.Width *= m_renderScale;
		description.Height *= m_renderScale;

		HR(dxFactory->CreateSwapChainForComposition(dxgiDevice.Get(),
			&description,
			nullptr, // Don’t restrict
			(IDXGISwapChain1**)m_pSwapChain.ReleaseAndGetAddressOf()));

		m_pSwapChain->SetSourceSize(
			description.Width,
			description.Height
		);

		DXGI_MATRIX_3X2_F inverseScale = { 0 };
		inverseScale._11 = 1.0f / m_renderScale;
		inverseScale._22 = 1.0f / m_renderScale;

		HR(m_pSwapChain->SetMatrixTransform(&inverseScale));
	}
	return hr;
}

HRESULT D2DOverlay::CreateDCompositionDevice()
{
	HRESULT hr = (m_d3d11Device == nullptr) ? E_UNEXPECTED : S_OK;
	ComPtr<IDXGIDevice> dxgiDevice;
	HR(m_d3d11Device->QueryInterface(dxgiDevice.GetAddressOf()));
	HR(DCompositionCreateDevice(dxgiDevice.Get(), __uuidof(IDCompositionDevice), reinterpret_cast<void**>(m_pDevice.ReleaseAndGetAddressOf())));
	return hr;
}

HRESULT D2DOverlay::CreateDCompositionRenderTarget()
{
	HRESULT hr = ((m_pDevice == nullptr) || (m_hwnd == NULL)) ? E_UNEXPECTED : S_OK;
	HR(m_pDevice->CreateTargetForHwnd(m_hwnd, TRUE, m_pHwndRenderTarget.ReleaseAndGetAddressOf()));

	return hr;
}


HRESULT D2DOverlay::CreateDCompositionVisualTree()
{

	OnRenderComposite();

	// Make the swap chain available to the composition engine
	m_pSwapChain->Present(1, 0); // flags

	HR(m_pDevice->CreateVisual(m_pRootVisual.ReleaseAndGetAddressOf()));
	HR(m_pDevice->CreateVisual(m_pSpotlight.ReleaseAndGetAddressOf()));
	HR(m_pDevice->CreateVisual(m_pBackground.ReleaseAndGetAddressOf()));
	HR(m_pSpotlight->SetContent(m_pSwapChain.Get()));

	HR(m_pHwndRenderTarget->SetRoot(m_pRootVisual.Get()));
	HR(m_pRootVisual->AddVisual(m_pSpotlight.Get(), TRUE, NULL));


	HR(m_pDevice->Commit());

	return S_OK;
}


HRESULT D2DOverlay::CreateDResources()
{
	HR(m_d3d11Device->QueryInterface(m_dxgiDevice.ReleaseAndGetAddressOf()));

	D2D1_FACTORY_OPTIONS const options = { D2D1_DEBUG_LEVEL_NONE };

	// Create a single-threaded Direct2D factory with debugging information
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, m_d2Factory.ReleaseAndGetAddressOf()));

	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_IDWriteFactory.ReleaseAndGetAddressOf())));

	// Create the Direct2D device that links back to the Direct3D device
	HR(m_d2Factory->CreateDevice(m_dxgiDevice.Get(), m_d2Device.ReleaseAndGetAddressOf()));

	// Create the Direct2D device context that is the actual render target
	// and exposes drawing commands
	HR(m_d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_dc.ReleaseAndGetAddressOf()));

	// Retrieve the swap chain's back buffer
	HR(m_pSwapChain->GetBuffer(0, __uuidof(m_surface), reinterpret_cast<void**>(m_surface.ReleaseAndGetAddressOf())));


	// Create a Direct2D bitmap that points to the swap chain surface
	D2D1_BITMAP_PROPERTIES1 properties = {};
	properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;


	HR(m_dc->CreateBitmapFromDxgiSurface(m_surface.Get(), properties, m_bitmap.ReleaseAndGetAddressOf()));

	// Point the device context to the bitmap for rendering
	m_dc->SetTarget(m_bitmap.Get());

	return S_OK;
}