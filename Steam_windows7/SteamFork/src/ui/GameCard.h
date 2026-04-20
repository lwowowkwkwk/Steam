#pragma once
#include "pch.h"

class D2DRenderer;

class GameCard {
public:
    GameCard(const std::wstring& name, const std::wstring& iconPath);
    ~GameCard();

    void Render(D2DRenderer* renderer, const D2D1_RECT_F& rect);
    bool HitTest(float x, float y) const;
    void SetHovered(bool hovered);
    void SetPressed(bool pressed);
    void OnClick();

    std::wstring GetName() const { return m_name; }
    bool IsInstalled() const { return m_installed; }

private:
    std::wstring m_name;
    std::wstring m_iconPath;
    ID2D1Bitmap* m_bitmap = nullptr;
    D2D1_RECT_F m_rect = {};
    bool m_hovered = false;
    bool m_pressed = false;
    bool m_installed = true;
};