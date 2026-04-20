#include "../pch.h"
#include "LoginDialog.h"
#include "../Resource.h"

LoginDialog::LoginDialog() = default;
LoginDialog::~LoginDialog() = default;

bool LoginDialog::Show(HINSTANCE hInstance, HWND parent) {
    INT_PTR result = DialogBoxParamW(hInstance, 
                                     MAKEINTRESOURCEW(IDD_LOGIN_DIALOG), 
                                     parent, 
                                     DialogProc, 
                                     reinterpret_cast<LPARAM>(this));
    return result == IDOK;
}

INT_PTR CALLBACK LoginDialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LoginDialog* pThis = nullptr;
    
    if (uMsg == WM_INITDIALOG) {
        pThis = reinterpret_cast<LoginDialog*>(lParam);
        SetWindowLongPtrW(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<LoginDialog*>(GetWindowLongPtrW(hwndDlg, GWLP_USERDATA));
    }
    
    if (pThis) {
        return pThis->HandleMessage(hwndDlg, uMsg, wParam, lParam);
    }
    
    return FALSE;
}

INT_PTR LoginDialog::HandleMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG:
            return OnInitDialog(hwndDlg);
            
        case WM_COMMAND:
            OnCommand(hwndDlg, LOWORD(wParam), reinterpret_cast<HWND>(lParam), HIWORD(wParam));
            return TRUE;
            
        case WM_CLOSE:
            return OnClose(hwndDlg);
    }
    
    return FALSE;
}

bool LoginDialog::OnInitDialog(HWND hwndDlg) {
    // Центрируем диалог
    HWND parent = GetParent(hwndDlg);
    RECT rcParent, rcDlg;
    GetWindowRect(parent, &rcParent);
    GetWindowRect(hwndDlg, &rcDlg);
    
    int x = rcParent.left + ((rcParent.right - rcParent.left) - (rcDlg.right - rcDlg.left)) / 2;
    int y = rcParent.top + ((rcParent.bottom - rcParent.top) - (rcDlg.bottom - rcDlg.top)) / 2;
    SetWindowPos(hwndDlg, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
    return true;
}

void LoginDialog::OnCommand(HWND hwndDlg, int id, HWND hwndCtl, UINT codeNotify) {
    switch (id) {
        case IDC_LOGIN_BUTTON: {
            wchar_t username[256] = {0};
            wchar_t password[256] = {0};
            
            GetDlgItemTextW(hwndDlg, IDC_USERNAME, username, 255);
            GetDlgItemTextW(hwndDlg, IDC_PASSWORD, password, 255);
            
            int len = WideCharToMultiByte(CP_UTF8, 0, username, -1, nullptr, 0, nullptr, nullptr);
            m_username.resize(len - 1);
            WideCharToMultiByte(CP_UTF8, 0, username, -1, &m_username[0], len, nullptr, nullptr);
            
            len = WideCharToMultiByte(CP_UTF8, 0, password, -1, nullptr, 0, nullptr, nullptr);
            m_password.resize(len - 1);
            WideCharToMultiByte(CP_UTF8, 0, password, -1, &m_password[0], len, nullptr, nullptr);
            
            m_remember = (IsDlgButtonChecked(hwndDlg, IDC_REMEMBER) == BST_CHECKED);
            m_success = true;
            
            EndDialog(hwndDlg, IDOK);
            break;
        }
        
        case IDC_CANCEL_BUTTON:
            m_success = false;
            EndDialog(hwndDlg, IDCANCEL);
            break;
    }
}

bool LoginDialog::OnClose(HWND hwndDlg) {
    m_success = false;
    EndDialog(hwndDlg, IDCANCEL);
    return true;
}