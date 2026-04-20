#pragma once
#include "pch.h"

class HttpClient {
public:
    struct Response {
        DWORD statusCode = 0;
        std::string body;
        std::map<std::string, std::string> headers;
    };

    HttpClient();
    ~HttpClient();

    bool Initialize(const std::wstring& userAgent = L"SteamFork/1.0");
    Response Get(const std::wstring& url);
    Response Post(const std::wstring& url, const std::string& data, const std::string& contentType = "application/json");
    bool DownloadFile(const std::wstring& url, const std::wstring& localPath, std::function<void(size_t, size_t)> progressCallback = nullptr);

private:
    HINTERNET m_hSession = nullptr;
    HINTERNET m_hConnect = nullptr;
    std::wstring m_userAgent;

    static std::wstring ExtractHost(const std::wstring& url);
    static std::wstring ExtractPath(const std::wstring& url);
    static bool IsSecure(const std::wstring& url);
};