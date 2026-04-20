#pragma once
#include "pch.h"
#include "GameCard.h"

class D2DRenderer;

class LibraryPanel {
public:
    explicit LibraryPanel(D2DRenderer* renderer);
    ~LibraryPanel();

    void Render();
    void UpdateLayout(float x, float y, float width, float height);
    void AddGame(const std::wstring& name, const std::wstring& iconPath);
    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);
    void OnLButtonUp(int x, int y);

private:
    D2DRenderer* m_renderer = nullptr;
    std::vector<std::unique_ptr<GameCard>> m_games;
    D2D1_RECT_F m_bounds = {};

    // Вспомогательные методы для расчёта расположения карточек
    void RecalculateLayout();
};