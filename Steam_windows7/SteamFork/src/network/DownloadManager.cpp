#include "../pch.h"
#include "DownloadManager.h"
#include "../core/Utils.h"

DownloadManager& DownloadManager::GetInstance() {
    static DownloadManager instance;
    return instance;
}

DownloadManager::DownloadManager() {
    m_http.Initialize(L"SteamFork/1.0");
    
    // Запускаем worker threads
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) threadCount = 4;
    
    for (unsigned int i = 0; i < threadCount; ++i) {
        m_workers.emplace_back(&DownloadManager::WorkerThread, this);
    }
}

DownloadManager::~DownloadManager() {
    m_running = false;
    m_cv.notify_all();
    
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void DownloadManager::AddTask(std::shared_ptr<DownloadTask> task) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_queue.push(task);
    }
    m_cv.notify_one();
}

void DownloadManager::WorkerThread() {
    while (m_running) {
        std::shared_ptr<DownloadTask> task;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [this] { return !m_queue.empty() || !m_running; });
            
            if (!m_running) break;
            
            task = m_queue.front();
            m_queue.pop();
        }
        
        ProcessTask(task);
    }
}

void DownloadManager::ProcessTask(std::shared_ptr<DownloadTask> task) {
    // Создаем директорию для файла
    std::wstring dir = task->localPath.substr(0, task->localPath.find_last_of(L'\\'));
    CreateDirectoryW(dir.c_str(), nullptr);
    
    // Открываем файл для записи
    HANDLE hFile = CreateFileW(task->localPath.c_str(), GENERIC_WRITE, 0, nullptr,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        task->failed = true;
        task->errorMessage = L"Failed to create file";
        if (task->onComplete) task->onComplete(*task);
        return;
    }
    
    // Выполняем загрузку
    // ... (реализация загрузки с прогрессом)
    
    CloseHandle(hFile);
    
    task->completed = true;
    if (task->onComplete) task->onComplete(*task);
}