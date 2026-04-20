#pragma once
#include "pch.h"

class GameLauncher {
public:
    struct GameInfo {
        std::wstring name;
        std::wstring exePath;
        std::wstring arguments;
        std::wstring workingDir;
    };

    bool LaunchGame(const GameInfo& info);
    bool IsGameRunning() const { return m_hProcess != nullptr; }
    void TerminateGame();
    DWORD GetExitCode() const;
    void MonitorGame(std::function<void()> onExit);

private:
    HANDLE m_hProcess = nullptr;
    HANDLE m_hThread = nullptr;
    std::thread m_monitorThread;
};