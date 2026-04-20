#include "../pch.h"
#include "GameCard.h"
#include "D2DRenderer.h"

GameCard::GameCard(const std::wstring& name, const std::wstring& iconPath)
    : m_name(name), m_iconPath(iconPath) {
}

GameCard::~GameCard() {
    if (m_bitmap) {
        m_bitmap->Release();
    }
}

void GameCard::Render(D2DRenderer* renderer, const D2D1_RECT_F& rect) {
    m_rect = rect;
    auto rt = renderer->GetRenderTarget();
    
    // Загружаем изображение при первой отрисовке
    if (!m_bitmap && !m_iconPath.empty()) {
        m_bitmap = renderer->LoadBitmapFromFile(m_iconPath);
    }
    
    // Фон карточки
    ID2D1SolidColorBrush* bgBrush = nullptr;
    D2D1::ColorF bgColor = m_hovered ? D2D1::ColorF(0.95f, 0.95f, 0.95f) : D2D1::ColorF::White;
    if (m_pressed) bgColor = D2D1::ColorF(0.85f, 0.85f, 0.85f);
    
    rt->CreateSolidColorBrush(bgColor, &bgBrush);
    rt->FillRectangle(rect, bgBrush);
    bgBrush->Release();
    
    // Рамка
    ID2D1SolidColorBrush* borderBrush = nullptr;
    rt->CreateSolidColorBrush(D2D1::ColorF(0.8f, 0.8f, 0.8f), &borderBrush);
    rt->DrawRectangle(rect, borderBrush, 1.0f);
    borderBrush->Release();
    
    // Изображение игры
    if (m_bitmap) {
        D2D1_RECT_F imageRect = rect;
        imageRect.bottom = rect.top + (rect.bottom - rect.top) * 0.7f;
        imageRect.left += 5;
        imageRect.right -= 5;
        imageRect.top += 5;
        rt->DrawBitmap(m_bitmap, imageRect);
    }
    
    // Название игры
    ID2D1SolidColorBrush* textBrush = nullptr;
    rt->CreateSolidColorBrush(D2D1::ColorF::Black, &textBrush);
    
    D2D1_RECT_F textRect = rect;
    textRect.top = rect.top + (rect.bottom - rect.top) * 0.7f + 5;
    textRect.left += 5;
    textRect.right -= 5;
    
    renderer->DrawText(m_name, textRect, textBrush, L"Segoe UI", 12.0f);
    textBrush->Release();
}

bool GameCard::HitTest(float x, float y) const {
    return x >= m_rect.left && x <= m_rect.right && 
           y >= m_rect.top && y <= m_rect.bottom;
}

void GameCard::SetHovered(bool hovered) {
    m_hovered = hovered;
}

void GameCard::SetPressed(bool pressed) {
    m_pressed = pressed;
}

void GameCard::OnClick() {
    // Здесь будет запуск игры
    OutputDebugStringW((L"Game clicked: " + m_name + L"\n").c_str());
}