#pragma once
#include "../framework.h"
#include <memory>
#include <string>

class D2DRenderer {
public:
    D2DRenderer();
    ~D2DRenderer();

    bool Initialize(HWND hwnd);
    void Shutdown();
    
    bool BeginDraw();
    bool EndDraw();
    void Clear(D2D1::ColorF color);
    
    ID2D1HwndRenderTarget* GetRenderTarget() { return m_pRenderTarget; }
    ID2D1Factory* GetFactory() { return m_pFactory; }
    IDWriteFactory* GetWriteFactory() { return m_pDWriteFactory; }
    IWICImagingFactory* GetWICFactory() { return m_pWICFactory; }
    
    ID2D1Bitmap* LoadBitmapFromFile(const std::wstring& filePath);
    ID2D1Bitmap* LoadBitmapFromResource(int resourceId);
    
    void DrawText(const std::wstring& text, const D2D1_RECT_F& rect, 
                  ID2D1SolidColorBrush* brush, const std::wstring& fontName = L"Segoe UI", 
                  float fontSize = 12.0f);
    
    ID2D1SolidColorBrush* CreateBrush(D2D1::ColorF color);

private:
    ID2D1Factory* m_pFactory = nullptr;
    ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
    IDWriteFactory* m_pDWriteFactory = nullptr;
    IWICImagingFactory* m_pWICFactory = nullptr;
    
    HWND m_hwnd = nullptr;
};