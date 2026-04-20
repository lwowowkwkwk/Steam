#pragma once
#include "pch.h"
#include "HttpClient.h"

struct DownloadTask {
    std::wstring url;
    std::wstring localPath;
    std::atomic<size_t> totalBytes = 0;
    std::atomic<size_t> downloadedBytes = 0;
    std::atomic<bool> completed = false;
    std::atomic<bool> failed = false;
    std::wstring errorMessage;
    std::function<void(const DownloadTask&)> onProgress;
    std::function<void(const DownloadTask&)> onComplete;
};

class DownloadManager {
public:
    static DownloadManager& GetInstance();

    void AddTask(std::shared_ptr<DownloadTask> task);
    void CancelAll();
    void WaitForAll();

private:
    DownloadManager();
    ~DownloadManager();

    void WorkerThread();
    void ProcessTask(std::shared_ptr<DownloadTask> task);
    bool VerifyFileIntegrity(const std::wstring& filePath, const std::string& expectedHash);

    std::vector<std::thread> m_workers;
    std::queue<std::shared_ptr<DownloadTask>> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_running = true;

    HttpClient m_http;
};