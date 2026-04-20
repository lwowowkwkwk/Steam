#include "pch.h"
#include "ui/MainWindow.h"
#include "ui/LoginDialog.h"
#include "core/ConfigManager.h"
#include <crtdbg.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    // Включаем проверку утечек памяти в отладочной сборке
    #ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    // Инициализация COM для работы с WIC (изображения)
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to initialize COM library.", L"Fatal Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Инициализация общих элементов управления
    INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    // Загрузка конфигурации
    ConfigManager& config = ConfigManager::GetInstance();
    config.Load();

    // Попытка автоматического входа (если сохранён токен)
    // ...

    // Создание и отображение главного окна
    MainWindow mainWindow;
    if (!mainWindow.Create(hInstance)) {
        MessageBoxW(nullptr, L"Failed to create main window.", L"Fatal Error", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return 1;
    }

    // Если требуется вход, показываем диалог
    // ...

    // Главный цикл сообщений
    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    CoUninitialize();
    return static_cast<int>(msg.wParam);
}