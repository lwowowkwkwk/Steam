#include "../pch.h"
#include "LibraryPanel.h"
#include "D2DRenderer.h"

LibraryPanel::LibraryPanel(D2DRenderer* renderer) 
    : m_renderer(renderer) {
    // Добавляем тестовые игры
    AddGame(L"Counter-Strike 2", L"");
    AddGame(L"Half-Life 3", L"");
    AddGame(L"Portal 3", L"");
}

LibraryPanel::~LibraryPanel() = default;

void LibraryPanel::Render() {
    for (auto& game : m_games) {
        game->Render(m_renderer, m_bounds);
    }
}

void LibraryPanel::UpdateLayout(float x, float y, float width, float height) {
    m_bounds = D2D1::RectF(x, y, x + width, y + height);
    RecalculateLayout();
}

void LibraryPanel::AddGame(const std::wstring& name, const std::wstring& iconPath) {
    m_games.push_back(std::make_unique<GameCard>(name, iconPath));
    RecalculateLayout();
}

void LibraryPanel::RecalculateLayout() {
    const float cardWidth = 150.0f;
    const float cardHeight = 200.0f;
    const float padding = 10.0f;
    
    float x = m_bounds.left + padding;
    float y = m_bounds.top + padding;
    
    for (auto& game : m_games) {
        D2D1_RECT_F cardRect = D2D1::RectF(x, y, x + cardWidth, y + cardHeight);
        game->Render(m_renderer, cardRect);
        
        x += cardWidth + padding;
        if (x + cardWidth > m_bounds.right) {
            x = m_bounds.left + padding;
            y += cardHeight + padding;
        }
    }
}

void LibraryPanel::OnMouseMove(int x, int y) {
    for (auto& game : m_games) {
        game->SetHovered(game->HitTest((float)x, (float)y));
    }
}

void LibraryPanel::OnLButtonDown(int x, int y) {
    for (auto& game : m_games) {
        if (game->HitTest((float)x, (float)y)) {
            game->SetPressed(true);
        }
    }
}

void LibraryPanel::OnLButtonUp(int x, int y) {
    for (auto& game : m_games) {
        if (game->HitTest((float)x, (float)y)) {
            game->SetPressed(false);
            game->OnClick();
        }
    }
}