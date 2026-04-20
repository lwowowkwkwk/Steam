#include "pch.h"
#include "MainWindow.h"
#include "resource.h"

MainWindow::MainWindow() = default;

MainWindow::~MainWindow() {
    if (m_hWnd) DestroyWindow(m_hWnd);
}

bool MainWindow::Create(HINSTANCE hInstance) {
    m_hInstance = hInstance;

    WNDCLASSEXW wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_STEAMFORK));
    wcex.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"SteamForkMainWindow";
    wcex.hIconSm       = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(IDI_SMALL));

    RegisterClassExW(&wcex);

    m_hWnd = CreateWindowExW(
        0,
        L"SteamForkMainWindow",
        L"Steam Fork",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1024, 768,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    return m_hWnd != nullptr;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;
    if (message == WM_NCCREATE) {
        auto pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<MainWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->m_hWnd = hWnd;
    } else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->HandleMessage(message, wParam, lParam);
    }
    return DefWindowProcW(hWnd, message, wParam, lParam);
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            if (!InitD2D()) return -1;
            m_libraryPanel = std::make_unique<LibraryPanel>(m_renderer.get());
            return 0;
        case WM_SIZE:
            OnResize(LOWORD(lParam), HIWORD(lParam));
            return 0;
        case WM_PAINT:
            OnPaint();
            return 0;
        case WM_DESTROY:
            OnDestroy();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(m_hWnd, message, wParam, lParam);
}

bool MainWindow::InitD2D() {
    m_renderer = std::make_unique<D2DRenderer>();
    return m_renderer->Initialize(m_hWnd);
}

void MainWindow::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    m_renderer->BeginDraw();
    m_renderer->Clear(D2D1::ColorF(D2D1::ColorF::LightGray));
    m_libraryPanel->Render();
    m_renderer->EndDraw();
    EndPaint(m_hWnd, &ps);
}