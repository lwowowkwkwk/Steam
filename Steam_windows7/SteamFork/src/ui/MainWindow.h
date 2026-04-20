#pragma once
#include "pch.h"
#include "D2DRenderer.h"
#include "LibraryPanel.h"

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    bool Create(HINSTANCE hInstance);
    HWND GetHwnd() const { return m_hWnd; }

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);

    bool InitD2D();
    void OnPaint();
    void OnResize(int width, int height);
    void OnDestroy();

    HINSTANCE m_hInstance = nullptr;
    HWND m_hWnd = nullptr;
    std::unique_ptr<D2DRenderer> m_renderer;
    std::unique_ptr<LibraryPanel> m_libraryPanel;
};