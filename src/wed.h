#pragma once
#define _WIN32_WINNT 0x0501
#include "CtrlFoo.h" // "WinFoo.h"
#include "SciFoo.h"
#include <stdio.h>
#include "SciLexer.h"
void sOutputDebugFormat(LPCTSTR szFormat, ...)
{
    TCHAR szBuffer[1024];
    va_list pArgList;
    va_start(pArgList, szFormat);
#ifdef UNICODE
    _vsnwprintf(szBuffer, sizeof (szBuffer) / sizeof (TCHAR), szFormat, pArgList);
#else
    _vsnprintf(szBuffer, sizeof (szBuffer) / sizeof (TCHAR), szFormat, pArgList);
#endif
    va_end(pArgList);
    OutputDebugString(szBuffer);
}
int CDECL sMessageBoxFormat(LPCTSTR szCaption, LPCTSTR szFormat, ...)
{
    TCHAR szBuffer[1024];
    va_list pArgList;
    va_start(pArgList, szFormat);
#ifdef UNICODE
    _vsnwprintf(szBuffer, sizeof (szBuffer) / sizeof (TCHAR), szFormat, pArgList);
#else
    _vsnprintf(szBuffer, sizeof (szBuffer) / sizeof (TCHAR), szFormat, pArgList);
#endif
    va_end(pArgList);
    return MessageBox((HWND)0, szBuffer, szCaption, MB_OK);
}
#define LOOKINT(var) sOutputDebugFormat(TEXT(#var) TEXT(" = %d\n"), (var))
#ifdef UNICODE
#define LOOKSTR(var) sOutputDebugFormat(TEXT(#var) TEXT(" = %ls\n"), (var))
#else
#define LOOKSTR(var) sOutputDebugFormat(TEXT(#var) TEXT(" = %s\n"), (var))
#endif