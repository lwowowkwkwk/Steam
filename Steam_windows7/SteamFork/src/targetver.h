#pragma once

// Включаем SDKDDKVer.h для определения последней доступной платформы Windows
#include <SDKDDKVer.h>

// Для Windows 7 минимальная версия - 0x0601
#define _WIN32_WINNT 0x0601
#define WINVER 0x0601
#define NTDDI_VERSION 0x06010000