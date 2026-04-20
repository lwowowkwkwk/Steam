#pragma once
#include "../framework.h"
#include <string>
#include <vector>
#include <functional>

class Utils {
public:
    static std::wstring ToWideString(const std::string& str);
    static std::string ToNarrowString(const std::wstring& wstr);
    
    static std::string Base64Encode(const std::vector<BYTE>& data);
    static std::vector<BYTE> Base64Decode(const std::string& data);
    
    static std::string SHA1(const std::string& data);
    static std::string SHA1(const std::vector<BYTE>& data);
    
    static uint32_t CRC32(const std::vector<BYTE>& data);
    static uint32_t CRC32(const std::string& data);
    
    static bool FileExists(const std::wstring& path);
    static std::vector<BYTE> ReadFile(const std::wstring& path);
    static bool WriteFile(const std::wstring& path, const std::vector<BYTE>& data);
    
    static std::wstring GetAppDataPath();
    static std::wstring GetTempPath();
    
    static void RunOnUIThread(std::function<void()> func);
};