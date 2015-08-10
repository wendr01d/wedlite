#pragma once

#include <Windows.h>
const COLORREF eColorBlack = RGB(0x00, 0x00, 0x00);
const COLORREF eColorDarkRed = RGB(0x8B, 0x00, 0x00);
const COLORREF eColorRed = RGB(0xFF, 0x00, 0x00);
const COLORREF eColorDarkGreen = RGB(0x00, 0x64, 0x00);
const COLORREF eColorGreen = RGB(0x00, 0x80, 0x00);
const COLORREF eColorGray = RGB(0x80, 0x80, 0x80);
const COLORREF eColorYellow = RGB(0xFF, 0xFF, 0x00);
const COLORREF eColorDarkBlue = RGB(0x00, 0x00, 0x8B);
const COLORREF eColorDarkMagenta = RGB(0x8B, 0x00, 0x8B);
const COLORREF eColorDarkCyan = RGB(0x00, 0x8B, 0x8B);
const COLORREF eColorLightGreen = RGB(0x90, 0xEE, 0x90);
const COLORREF eColorDarkGray = RGB(0xA9, 0xA9, 0xA9);
const COLORREF eColorLightGray = RGB(0xD3, 0xD3, 0xD3);
const COLORREF eColorLightYellow = RGB(0xFF, 0xFF, 0xE0);
const COLORREF eColorLightBlue = RGB(0xAD, 0xD8, 0xE6);
const COLORREF eColorBlue = RGB(0x00, 0x00, 0xFF);
const COLORREF eColorMagenta = RGB(0xFF, 0x00, 0xFF);
const COLORREF eColorCyan = RGB(0x00, 0xFF, 0xFF);
const COLORREF eColorLightCyan = RGB(0xE0, 0xFF, 0xFF);
const COLORREF eColorWhite = RGB(0xFF, 0xFF, 0xFF);
void dbgLastError()
{
    DWORD lastError = ::GetLastError();
    DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
    HLOCAL hlocal = (HLOCAL)0;
    BOOL fOk = ::FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
            FORMAT_MESSAGE_ALLOCATE_BUFFER,
            (LPCVOID)0, lastError, systemLocale,
            (LPTSTR)&hlocal, (DWORD)0, (va_list*)0);
    if (fOk == FALSE) {
        HMODULE hDll = ::LoadLibraryEx(TEXT("netmsg.dll"), (HANDLE)0, DONT_RESOLVE_DLL_REFERENCES);
        if (hDll != (HMODULE)0) {
            fOk = ::FormatMessage(
                    FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
                    FORMAT_MESSAGE_ALLOCATE_BUFFER,
                    (LPCVOID)hDll, lastError, systemLocale,
                    (LPTSTR)&hlocal, (DWORD)0, (va_list*)0);
            FreeLibrary(hDll);
        }
    }
    if (fOk && (hlocal != (HLOCAL)0)) {
        ::MessageBox((HWND)0, (LPCTSTR)::LocalLock(hlocal), (LPCTSTR)0, MB_ICONERROR);
        ::LocalFree(hlocal);
    } else {
        ::MessageBox((HWND)0, TEXT("Nothing!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
LPTSTR sStrNCat(LPTSTR dst, LPCTSTR src, size_t n)
{
    TCHAR* temp;
    for (temp = dst; *temp != TEXT('\0'); ++temp);
    for (; 0 < n && *src != TEXT('\0'); --n) {
        *temp++ = *src++;
    }
    *temp = TEXT('\0');
    return dst;
}
int sStrNCmp(LPCTSTR dst, LPCTSTR src, size_t n)
{
    for (; 0 < n; ++dst, ++src, --n) {
        if (*dst != *src) {
            return ((*dst < *src) ? -1 : +1);
        } else if (*dst == TEXT('\0')) {
            return 0;
        }
    }
    return 0;
}
LPTSTR sStrNCpy(LPTSTR dst, LPCTSTR src, size_t n)
{
    TCHAR* temp;
    for (temp = dst; 0 < n && *src != TEXT('\0'); --n) {
        *temp++ = *src++;
    }
    for (; 0 < n; --n) {
        *temp = TEXT('\0');
    }
    return dst;
}
LPTSTR sStrCpy(LPTSTR dst, LPCTSTR src)
{
    TCHAR* temp;
    for (temp = dst; (*temp++ = *src++) != TEXT('\0'););
    return dst;
}
size_t sStrLen(LPCTSTR text)
{
    const TCHAR* temp;
    for (temp = text; *temp != TEXT('\0'); ++temp);
    return temp - text;
}
void sGetFileNameFromPath(LPCTSTR szPath, LPTSTR szFileName)
{
    int i = 0;
    while (szPath[i] != TEXT('\0')) ++i;
    while (szPath[i] != TEXT('\\')) --i;
    ++i; 
    LPCTSTR sz = szPath + i;
    while ((*szFileName++ = *sz++) != TEXT('\0'));
}
static int sComboGetDroppedWidth(HWND hwndCombo)
{
    return (int)::SendMessage(hwndCombo, CB_GETDROPPEDWIDTH, 0, 0);
}
static void sComboAddString(HWND hwndCombo, LPCTSTR text)
{
    switch (::SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)text)) {
    case CB_ERR: case CB_ERRSPACE:
        ::MessageBox((HWND)0, TEXT("Combo Add String Fail!"), (LPCTSTR)0, MB_ICONERROR);
        break;
    }
}
static int sComboGetCount(HWND hwndCombo)
{
    return (int)::SendMessage(hwndCombo, CB_GETCOUNT, 0, 0);
}
static int sComboGetCurSel(HWND hwndCombo)
{
    return (int)::SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
}
static void sComboSetCurSel(HWND hwndCombo, int indexItem)
{
    if (::SendMessage(hwndCombo, CB_SETCURSEL, indexItem, 0) == CB_ERR) {
        ::MessageBox((HWND)0, TEXT("Combo Set Cur Sel Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sComboGetLbText(HWND hwndCombo, int indexItem, LPTSTR textLb)
{
    if (::SendMessage(hwndCombo, CB_GETLBTEXT, (WPARAM)indexItem, (LPARAM)textLb) == CB_ERR) {
        ::MessageBox((HWND)0, TEXT("Combo Get Lb Text Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}






