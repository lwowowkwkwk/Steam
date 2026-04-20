#include "../pch.h"
#include "GameLauncher.h"

bool GameLauncher::LaunchGame(const GameInfo& info) {
    if (m_hProcess) {
        return false; // Игра уже запущена
    }
    
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi = {};
    
    std::wstring cmdLine = L"\"" + info.exePath + L"\"";
    if (!info.arguments.empty()) {
        cmdLine += L" " + info.arguments;
    }
    
    std::wstring workingDir = info.workingDir.empty() ? 
        info.exePath.substr(0, info.exePath.find_last_of(L'\\')) : 
        info.workingDir;
    
    if (CreateProcessW(nullptr, cmdLine.data(), nullptr, nullptr, FALSE,
                       CREATE_NEW_CONSOLE, nullptr, workingDir.c_str(),
                       &si, &pi)) {
        m_hProcess = pi.hProcess;
        m_hThread = pi.hThread;
        return true;
    }
    
    return false;
}

void GameLauncher::TerminateGame() {
    if (m_hProcess) {
        TerminateProcess(m_hProcess, 0);
        CloseHandle(m_hProcess);
        CloseHandle(m_hThread);
        m_hProcess = nullptr;
        m_hThread = nullptr;
    }
    
    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
    }
}

DWORD GameLauncher::GetExitCode() const {
    if (m_hProcess) {
        DWORD exitCode;
        if (GetExitCodeProcess(m_hProcess, &exitCode)) {
            return exitCode;
        }
    }
    return STILL_ACTIVE;
}

void GameLauncher::MonitorGame(std::function<void()> onExit) {
    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
    }
    
    m_monitorThread = std::thread([this, onExit]() {
        if (m_hProcess) {
            WaitForSingleObject(m_hProcess, INFINITE);
            onExit();
        }
    });
}