#include "Renderer.h"

bool Renderer::Render(Microsoft::WRL::ComPtr<ID2D1DeviceContext> dc)
{
    auto pDeviceContext = dc;

    dc->BeginDraw();
    dc->Clear();
    dc->EndDraw();

    D2D1_SIZE_F rect = dc->GetSize();

    pDeviceContext->BeginDraw();

    pDeviceContext->Clear(D2D1::ColorF(0,0)); // Clear the background with white color

    // Create a red brush
    ID2D1SolidColorBrush* pBrush;
    pDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.75f), &pBrush);

    float cicleRadius = 2.0;

    float centerX = static_cast<float>((rect.width) / 2);
    float centerY = static_cast<float>((rect.height+ cicleRadius + 2) / 2);

    // Draw a red circle
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(centerX, centerY), cicleRadius, cicleRadius); // Circle at (200, 200) with radius 50
    pDeviceContext->FillEllipse(&ellipse, pBrush);

    pDeviceContext->EndDraw();


	return true;
}
