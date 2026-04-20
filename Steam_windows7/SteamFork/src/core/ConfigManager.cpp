#include "../pch.h"
#include "ConfigManager.h"
#include <fstream>
#include <sstream>

ConfigManager& ConfigManager::GetInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() {
    Load();
}

ConfigManager::~ConfigManager() {
    Save();
}

std::wstring ConfigManager::GetConfigPath() {
    wchar_t appData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, appData))) {
        std::wstring path = appData;
        path += L"\\SteamFork";
        CreateDirectoryW(path.c_str(), nullptr);
        path += L"\\config.ini";
        return path;
    }
    return L"config.ini";
}

bool ConfigManager::Load() {
    std::ifstream file(GetConfigPath());
    if (!file.is_open()) return false;
    
    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq != std::string::npos) {
            std::string key = line.substr(0, eq);
            std::string value = line.substr(eq + 1);
            m_config[key] = value;
        }
    }
    
    return true;
}

bool ConfigManager::Save() {
    std::ofstream file(GetConfigPath());
    if (!file.is_open()) return false;
    
    for (const auto& [key, value] : m_config) {
        file << key << "=" << value << "\n";
    }
    
    return true;
}

std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) {
    auto it = m_config.find(key);
    return it != m_config.end() ? it->second : defaultValue;
}

int ConfigManager::GetInt(const std::string& key, int defaultValue) {
    auto it = m_config.find(key);
    return it != m_config.end() ? std::stoi(it->second) : defaultValue;
}

bool ConfigManager::GetBool(const std::string& key, bool defaultValue) {
    auto it = m_config.find(key);
    return it != m_config.end() ? (it->second == "true") : defaultValue;
}

void ConfigManager::SetString(const std::string& key, const std::string& value) {
    m_config[key] = value;
}

void ConfigManager::SetInt(const std::string& key, int value) {
    m_config[key] = std::to_string(value);
}

void ConfigManager::SetBool(const std::string& key, bool value) {
    m_config[key] = value ? "true" : "false";
}