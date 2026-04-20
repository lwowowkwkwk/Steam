#pragma once
#include "../framework.h"
#include <string>
#include <map>

class ConfigManager {
public:
    static ConfigManager& GetInstance();
    
    bool Load();
    bool Save();
    
    std::string GetString(const std::string& key, const std::string& defaultValue = "");
    int GetInt(const std::string& key, int defaultValue = 0);
    bool GetBool(const std::string& key, bool defaultValue = false);
    
    void SetString(const std::string& key, const std::string& value);
    void SetInt(const std::string& key, int value);
    void SetBool(const std::string& key, bool value);

private:
    ConfigManager();
    ~ConfigManager();
    
    std::map<std::string, std::string> m_config;
    std::wstring GetConfigPath();
};