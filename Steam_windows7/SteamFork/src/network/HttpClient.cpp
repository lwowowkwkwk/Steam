#include "../pch.h"
#include "HttpClient.h"

HttpClient::HttpClient() = default;

HttpClient::~HttpClient() {
    if (m_hConnect) WinHttpCloseHandle(m_hConnect);
    if (m_hSession) WinHttpCloseHandle(m_hSession);
}

bool HttpClient::Initialize(const std::wstring& userAgent) {
    m_userAgent = userAgent;
    
    m_hSession = WinHttpOpen(m_userAgent.c_str(),
                             WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                             WINHTTP_NO_PROXY_NAME,
                             WINHTTP_NO_PROXY_BYPASS, 0);
    
    return m_hSession != nullptr;
}

HttpClient::Response HttpClient::Get(const std::wstring& url) {
    Response response;
    
    std::wstring host = ExtractHost(url);
    std::wstring path = ExtractPath(url);
    bool secure = IsSecure(url);
    
    HINTERNET hConnect = WinHttpConnect(m_hSession, host.c_str(),
                                        secure ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) return response;
    
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path.c_str(),
                                            nullptr, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            secure ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        return response;
    }
    
    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                           WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        if (WinHttpReceiveResponse(hRequest, nullptr)) {
            // Получаем статус код
            DWORD statusCode = 0;
            DWORD statusCodeSize = sizeof(statusCode);
            WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                               WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX);
            response.statusCode = statusCode;
            
            // Читаем ответ
            DWORD bytesAvailable = 0;
            while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
                std::vector<char> buffer(bytesAvailable);
                DWORD bytesRead = 0;
                if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead)) {
                    response.body.append(buffer.data(), bytesRead);
                }
            }
        }
    }
    
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    return response;
}

HttpClient::Response HttpClient::Post(const std::wstring& url, const std::string& data, const std::string& contentType) {
    Response response;
    
    std::wstring host = ExtractHost(url);
    std::wstring path = ExtractPath(url);
    bool secure = IsSecure(url);
    
    HINTERNET hConnect = WinHttpConnect(m_hSession, host.c_str(),
                                        secure ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0);
    if (!hConnect) return response;
    
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", path.c_str(),
                                            nullptr, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            secure ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        return response;
    }
    
    // Добавляем заголовок Content-Type
    std::wstring wContentType = L"Content-Type: " + Utils::ToWideString(contentType) + L"\r\n";
    WinHttpAddRequestHeaders(hRequest, wContentType.c_str(), (DWORD)-1, WINHTTP_ADDREQ_FLAG_ADD);
    
    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                           (LPVOID)data.c_str(), (DWORD)data.size(), (DWORD)data.size(), 0)) {
        if (WinHttpReceiveResponse(hRequest, nullptr)) {
            DWORD statusCode = 0;
            DWORD statusCodeSize = sizeof(statusCode);
            WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                               WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, WINHTTP_NO_HEADER_INDEX);
            response.statusCode = statusCode;
            
            DWORD bytesAvailable = 0;
            while (WinHttpQueryDataAvailable(hRequest, &bytesAvailable) && bytesAvailable > 0) {
                std::vector<char> buffer(bytesAvailable);
                DWORD bytesRead = 0;
                if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead)) {
                    response.body.append(buffer.data(), bytesRead);
                }
            }
        }
    }
    
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    return response;
}

std::wstring HttpClient::ExtractHost(const std::wstring& url) {
    size_t start = url.find(L"://");
    if (start == std::wstring::npos) start = 0;
    else start += 3;
    
    size_t end = url.find(L'/', start);
    if (end == std::wstring::npos) end = url.length();
    
    return url.substr(start, end - start);
}

std::wstring HttpClient::ExtractPath(const std::wstring& url) {
    size_t start = url.find(L"://");
    if (start == std::wstring::npos) start = 0;
    else start += 3;
    
    start = url.find(L'/', start);
    if (start == std::wstring::npos) return L"/";
    
    return url.substr(start);
}

bool HttpClient::IsSecure(const std::wstring& url) {
    return url.find(L"https://") == 0;
}