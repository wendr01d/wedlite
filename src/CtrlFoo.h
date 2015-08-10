#pragma once

#include "WinFoo.h"
#include <commctrl.h>
#pragma comment (lib, "comctl32.lib")
static void sInitCommonControls()
{
    INITCOMMONCONTROLSEX icce = {0};
    icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icce.dwICC = ICC_BAR_CLASSES | ICC_COOL_CLASSES/* | ICC_USEREX_CLASSES*/;
    InitCommonControlsEx(&icce);
}
static DWORD sToolbarGetButtonSize(HWND hwndToolbar)
{
    return (DWORD)::SendMessage(hwndToolbar, TB_GETBUTTONSIZE, 0, 0);
}
static DWORD sToolbarGetPadding(HWND hwndToolbar)
{
    return (DWORD)::SendMessage(hwndToolbar, TB_GETPADDING, 0, 0);
}
static int sToolbarButtonCount(HWND hwndToolbar)
{
    return (int)::SendMessage(hwndToolbar, TB_BUTTONCOUNT, 0, 0);
}
static void sToolbarSetState(HWND hwndToolbar, int idButton, BYTE fState)
{
    if (::SendMessage(hwndToolbar, TB_SETSTATE, idButton, MAKELONG(fState, 0)) == FALSE) {
        ::MessageBox((HWND)0, TEXT("Toolbar Set State Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static UINT sRebarGetBarHeight(HWND hwndRebar)
{
    return (UINT)::SendMessage(hwndRebar, RB_GETBARHEIGHT, 0, 0);
}

void sComboexInsertString(HWND hwndComboex, int indexItem, LPCTSTR text)
{
    TCHAR strItem[MAX_PATH];
    sStrNCpy(strItem, text, MAX_PATH);
    COMBOBOXEXITEM cbexItem;
    cbexItem.iItem = 0;
    cbexItem.mask = CBEIF_TEXT;
    cbexItem.pszText = strItem;
    cbexItem.cchTextMax = sStrLen(strItem);
    if (::SendMessage(hwndComboex, CBEM_INSERTITEM, 0, (LPARAM)&cbexItem) == -1) {
        ::MessageBox((HWND)0, TEXT("Comboex Insert String Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sRebarInsertBand(HWND hwndRebar, UINT uIndex, LPREBARBANDINFO rbBandInfo)
{
    if (::SendMessage(hwndRebar, RB_INSERTBAND, uIndex, (LPARAM)rbBandInfo) == 0) {
        ::MessageBox((HWND)0, TEXT("Rebar Insert Band Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}