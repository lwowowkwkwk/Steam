#pragma once
#include "pch.h"

class AuthManager {
public:
    static AuthManager& GetInstance();

    bool Login(const std::string& username, const std::string& password, bool remember);
    void Logout();
    bool IsLoggedIn() const { return !m_sessionToken.empty(); }

    std::string GetSessionToken() const { return m_sessionToken; }
    std::string GetUsername() const { return m_username; }

private:
    AuthManager();
    ~AuthManager();

    bool SaveCredentials(const std::string& username, const std::string& token);
    bool LoadCredentials(std::string& username, std::string& token);
    void ClearCredentials();

    // Использование DPAPI для шифрования учётных данных
    std::vector<BYTE> ProtectData(const std::string& data);
    std::string UnprotectData(const std::vector<BYTE>& encrypted);

    std::string m_username;
    std::string m_sessionToken;
};