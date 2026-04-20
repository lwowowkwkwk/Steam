#pragma once
#include "../framework.h"
#include <string>

class LoginDialog {
public:
    LoginDialog();
    ~LoginDialog();

    bool Show(HINSTANCE hInstance, HWND parent);
    std::string GetUsername() const { return m_username; }
    std::string GetPassword() const { return m_password; }
    bool GetRemember() const { return m_remember; }
    bool IsSuccess() const { return m_success; }

private:
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    INT_PTR HandleMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    void OnCommand(HWND hwndDlg, int id, HWND hwndCtl, UINT codeNotify);
    bool OnClose(HWND hwndDlg);
    bool OnInitDialog(HWND hwndDlg);

    std::string m_username;
    std::string m_password;
    bool m_remember = false;
    bool m_success = false;
};