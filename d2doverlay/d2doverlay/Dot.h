#pragma once

#include <wrl.h>
#include <d2d1.h>
#include <d2d1_1.h>

class Dot
{
public:
	//Returns true if redraw is required
	bool Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> dc);
	
};

