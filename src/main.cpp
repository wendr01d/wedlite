#include "wed.h"

const TCHAR eAppName[] = TEXT("WEd");
const TCHAR eFrameClass[] = TEXT("WEdFrame");

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE gHinstMain;
HWND gHwndFrame;
HWND gHwndToolbar;
HWND gHwndStatusbar;
HWND gHwndSearchOpts;
HWND gHwndRebar;
HWND gHwndListFileType;
HWND gHwndEditorOpts;
HWND gHwndEditSearch;
HWND gHwndEditReplace;

struct Editor {
    HWND handle;
    int (*directFunctionToEditor)(void*, int, int, int);
    void* directPointerToEditor;
    TCHAR pathSource[MAX_PATH];
    int indexFileType;
    BOOL flagDirty; // sEditorGetModify
    TCHAR encode[MAX_PATH]; // sEditorSetCodePage
    int eolMode; // sEditorSetEolMode
    BOOL flagOvertype; // sEditorGetOvertype

    BOOL flagReadOnly; // sEditorGetReadOnly
    BOOL flagAutoIndent;
    BOOL flagLineWrap; // sEditorSetWrapMode
    BOOL flagViewWhiteSpace; // sEditorSetViewWs
    BOOL flagViewLineEnding; // sEditorSetViewEol
    BOOL flagUseTabs; // sEditorUseTabs
    BOOL flagLineNumber;
};

Editor sEditor;
#define eBufferSize 128 * 1024
#define gEditor (sEditor)
static sptr_t sEditorCall(unsigned int uMsg, uptr_t wParam, sptr_t lParam)
{
    return gEditor.directFunctionToEditor(gEditor.directPointerToEditor, uMsg, wParam, lParam);
}

enum eResource {
    ID_START = 1024,
    ID_REBAR,
    ID_TOOLBAR,
    ID_LIST_FILETYPE,
    ID_EDITOR_OPTS,
    ID_SEARCH_OPTS,
    ID_EDIT_SEARCH,
    ID_EDIT_REPLACE,
    ID_EDITOR,
    ID_STATUSBAR,
    IDM_FIRSTCHILD,
    IDM_NEW, IDM_OPEN, IDM_SAVE, IDM_SAVEAS, IDM_PROPERTIES, IDM_PRINTSETUP, IDM_PRINT, IDM_CLOSE, IDM_EXIT,
    IDM_UNDO, IDM_REDO, IDM_CUT, IDM_COPY, IDM_PASTE, IDM_CLEAR, IDM_SELECTALL,
    IDM_FIND, IDM_REPLACE,

    IDM_READONLY,
    IDM_USETABS,
    IDM_LINEWRAP,
    IDM_VIEWWHITESPACE,
    IDM_VIEWLINEENDING,
    IDM_OVERTYPE,
    IDM_LINENUMBER,

    IDM_OPTIONS,
    IDM_ABOUT
};
struct FileType
{
    LPCTSTR description;
    void (*pFunFiletype)();
};

static void sEditorToPlainText();
static void sEditorToCpp();
FileType gFileTypeTable[] = {
    {TEXT("Plain Text"), sEditorToPlainText},
    {TEXT("Assembler"), 0},
    {TEXT("Bash"), 0},
    {TEXT("Batch File"), 0},
    {TEXT("C / C++"), sEditorToCpp},
    {TEXT("CSS"), 0},
    {TEXT("HTML"), 0},
    {TEXT("Java"), 0},
    {TEXT("JavaScript"), 0},
    {TEXT("XML"), 0}
};
static int sGetHeight(HWND hwnd)
{
    RECT rcWindow;
    GetWindowRect(hwnd, &rcWindow);
    return rcWindow.bottom - rcWindow.top;
}
void sRebarAddToolbar(HWND hwndRebar, HWND hwndToolbar)
{
    REBARBANDINFO rbBandInfo = {0};
    rbBandInfo.cbSize = sizeof(REBARBANDINFO);
    rbBandInfo.fMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE;
    rbBandInfo.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS | RBBS_USECHEVRON;
    rbBandInfo.hwndChild = hwndToolbar;
    DWORD size = sToolbarGetButtonSize(hwndToolbar);
    DWORD padding = sToolbarGetPadding(hwndToolbar);
    int count = sToolbarButtonCount(hwndToolbar);
    rbBandInfo.cxMinChild = LOWORD(size) * count + LOWORD(padding);
    rbBandInfo.cyMinChild = HIWORD(size) + HIWORD(padding);
    rbBandInfo.cyChild = HIWORD(size) + HIWORD(padding);
    rbBandInfo.cyIntegral = 2;
    rbBandInfo.cxIdeal = 300;
    sRebarInsertBand(hwndRebar, -1, &rbBandInfo);
}
void sRebarAddCombo(HWND hwndRebar, HWND hwndCombo, LPCTSTR text)
{
    REBARBANDINFO rbBandInfo = {0};
    rbBandInfo.cbSize = sizeof(REBARBANDINFO);
    rbBandInfo.fMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE;
    if (text != (LPCTSTR)0) {
        rbBandInfo.fMask |= RBBIM_TEXT;
        TCHAR textToBand[MAX_PATH];
        sStrNCpy(textToBand, text, MAX_PATH);
        rbBandInfo.lpText = textToBand;
        rbBandInfo.cch = sStrLen(textToBand);
    }
    rbBandInfo.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
    rbBandInfo.hwndChild = hwndCombo;
    rbBandInfo.cxMinChild = sComboGetDroppedWidth(hwndCombo) + 100;
    rbBandInfo.cyMinChild = sGetHeight(gHwndListFileType);
    sRebarInsertBand(hwndRebar, -1, &rbBandInfo);
}
static void sInitMenu()
{
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup, MF_STRING, IDM_NEW, TEXT("&New"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_OPEN, TEXT("&Open..."));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_SAVE, TEXT("&Save"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_SAVEAS, TEXT("Save &As..."));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_PROPERTIES, TEXT("Properties"));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_PRINT, TEXT("&Print"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_PRINTSETUP, TEXT("Print Setup"));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_CLOSE, TEXT("&Close"));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_EXIT, TEXT("E&xit"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&File"));

    hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup, MF_STRING, IDM_UNDO, TEXT("&Undo"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_REDO, TEXT("&Redo"));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_CUT, TEXT("Cu&t"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_COPY,TEXT("&Copy"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_PASTE,TEXT("&Paste"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_CLEAR,TEXT("De&lete"));
    AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenuPopup, MF_STRING, IDM_SELECTALL, TEXT("Select &All"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Edit"));

    hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup, MF_STRING, IDM_FIND, TEXT("&Find"));
    AppendMenu(hMenuPopup, MF_STRING, IDM_REPLACE, TEXT("&Replace"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Search"));

    hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup, MF_STRING, IDM_OPTIONS, TEXT("&Options"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Tools"));

    hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup, MF_STRING, IDM_ABOUT, TEXT("&About"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&Help"));

    SetMenu(gHwndFrame, hMenu);
}
static void sRegisterFrameClass()
{
    WNDCLASSEX wndclass    = {0};
    wndclass.cbSize        = sizeof(WNDCLASSEX); // more
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = gHinstMain;
    wndclass.hIcon         = LoadIcon((HINSTANCE)0, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor((HINSTANCE)0, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = (LPCTSTR)0;
    wndclass.lpszClassName = eFrameClass;
    wndclass.hIconSm       = (HICON)0; // more
    if (RegisterClassEx(&wndclass) == 0) {
        MessageBox((HWND)0, TEXT("This program requires Windows NT!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
void sInitFrame(int ncmdshow)
{
    gHwndFrame = CreateWindowEx(
            WS_EX_APPWINDOW,
            eFrameClass, eAppName,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            (HWND)0, (HMENU)0, gHinstMain, (LPVOID)0);
    if (::IsWindow(gHwndFrame) == 0) {
        MessageBox((HWND)0, TEXT("Create Frame Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitScintillaLibrary()
{
    if (LoadLibrary(TEXT("SciLexer.DLL")) == (HMODULE)0) {
        MessageBox((HWND)0, TEXT("The Scintilla DLL could not be loaded."),
            TEXT("Error loading Scintilla"), MB_OK | MB_ICONERROR);
    }
}
static void sInitToolBar()
{
    gHwndToolbar = ::CreateWindowEx(
            (DWORD)0, TOOLBARCLASSNAME, (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPCHILDREN
            | CCS_NOPARENTALIGN | CCS_NORESIZE// | CCS_NODIVIDER
            | TBSTYLE_FLAT,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)ID_TOOLBAR, gHinstMain, (LPVOID)0);
    if (gHwndToolbar == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Toolbar Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    ::SendMessage(gHwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    TBADDBITMAP tbAddBitmap;
    tbAddBitmap.hInst = HINST_COMMCTRL;
    tbAddBitmap.nID = IDB_STD_SMALL_COLOR;
    if (::SendMessage(gHwndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbAddBitmap) == -1) {
        ::MessageBox((HWND)0, TEXT("Toolbar Add Bitmap Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
    TBBUTTON tbButtons[] = {
        {STD_FILENEW, IDM_NEW, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_FILESAVE, IDM_SAVE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_PROPERTIES, IDM_PROPERTIES, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_PRINT, IDM_PRINT, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_UNDO, IDM_UNDO, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_REDOW, IDM_REDO, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_CUT, IDM_CUT, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_COPY, IDM_COPY, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_PASTE, IDM_PASTE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_DELETE, IDM_CLEAR, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_HELP, IDM_ABOUT, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
    };
    if (::SendMessage(gHwndToolbar, TB_ADDBUTTONS, sizeof(tbButtons)/sizeof(TBBUTTON), (LPARAM)&tbButtons) == FALSE) {
        ::MessageBox((HWND)0, TEXT("Toolbar Add Buttons Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitListFileType()
{
    gHwndListFileType = ::CreateWindow(
            WC_COMBOBOX,
            (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
            | WS_VSCROLL
            | CBS_DISABLENOSCROLL
            | CBS_DROPDOWNLIST
            | CBS_HASSTRINGS,
            0, 0, 0, 400,
            gHwndFrame, (HMENU)ID_LIST_FILETYPE, gHinstMain, (LPVOID)0);
    if (gHwndListFileType == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create List FileType Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
    int numFileTypeSupported = sizeof(gFileTypeTable) / sizeof(FileType);
    for (int i = 0; i < numFileTypeSupported; ++i) {
        sComboAddString(gHwndListFileType, gFileTypeTable[i].description);
    }
    sComboSetCurSel(gHwndListFileType, 0);
}
static void sInitEditorOpts()
{
    gHwndEditorOpts = ::CreateWindowEx(
            TBSTYLE_EX_MIXEDBUTTONS,
            TOOLBARCLASSNAME,
            (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPCHILDREN
            | CCS_NOPARENTALIGN | CCS_NORESIZE// | CCS_NODIVIDER
            | TBSTYLE_LIST
            | TBSTYLE_EX_MIXEDBUTTONS
            | TBSTYLE_FLAT,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)0, gHinstMain, (LPVOID)0);
    if (gHwndEditorOpts == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Editor Opts Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    ::SendMessage(gHwndEditorOpts, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    if (::SendMessage(gHwndEditorOpts, TB_SETBITMAPSIZE, 1, 1) == FALSE) {
        ::MessageBox((HWND)0, TEXT("Toolbar Set Bitmap Size Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
    TBBUTTON tbButtons[] = {
        {I_IMAGENONE, IDM_READONLY, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("RO"))},
        {I_IMAGENONE, IDM_LINEWRAP, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("LW"))},
        {I_IMAGENONE, IDM_VIEWWHITESPACE, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("VW"))},
        {I_IMAGENONE, IDM_VIEWLINEENDING, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("VL"))},
        {I_IMAGENONE, IDM_USETABS, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("UT"))},
        {I_IMAGENONE, IDM_OVERTYPE, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("OT"))},
        {I_IMAGENONE, IDM_LINENUMBER, TBSTATE_ENABLED, TBSTYLE_CHECK, {(BYTE)0},
            (DWORD_PTR)0, (INT_PTR)::SendMessage(gHwndEditorOpts, TB_ADDSTRING, 0, (LPARAM)TEXT("LN"))},
    };
    if (::SendMessage(gHwndEditorOpts, TB_ADDBUTTONS, sizeof(tbButtons)/sizeof(TBBUTTON), (LPARAM)&tbButtons) == FALSE) {
        ::MessageBox((HWND)0, TEXT("Toolbar Add Buttons Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitSearchOpts()
{
    gHwndSearchOpts = ::CreateWindow(
            TOOLBARCLASSNAME,
            (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPCHILDREN
            | CCS_NOPARENTALIGN | CCS_NORESIZE// | CCS_NODIVIDER
            | TBSTYLE_FLAT,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)0, gHinstMain, (LPVOID)0);
    if (gHwndSearchOpts == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Searchbar Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    ::SendMessage(gHwndSearchOpts, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    TBADDBITMAP tbAddBitmap;
    tbAddBitmap.hInst = HINST_COMMCTRL;
    tbAddBitmap.nID = IDB_STD_SMALL_COLOR;
    if (::SendMessage(gHwndSearchOpts, TB_ADDBITMAP, 0, (LPARAM)&tbAddBitmap) == -1) {
        ::MessageBox((HWND)0, TEXT("Toolbar Add Bitmap Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
    TBBUTTON tbButtons[] = {
        {STD_PRINTPRE, IDM_PRINTSETUP, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {0, 0, TBSTATE_ENABLED, BTNS_SEP, 0, 0},
        {STD_FIND, IDM_FIND, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
        {STD_REPLACE, IDM_REPLACE, TBSTATE_ENABLED, BTNS_BUTTON, 0, 0},
    };
    if (::SendMessage(gHwndSearchOpts, TB_ADDBUTTONS, sizeof(tbButtons)/sizeof(TBBUTTON), (LPARAM)&tbButtons) == FALSE) {
        ::MessageBox((HWND)0, TEXT("Toolbar Add Buttons Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitEditSearch()
{
    gHwndEditSearch = ::CreateWindow(WC_COMBOBOX, (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE |
            CBS_SIMPLE,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)ID_EDIT_SEARCH, gHinstMain, (LPVOID)0);
    if (gHwndEditSearch == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Edit Search Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitEditReplace()
{
    gHwndEditReplace = ::CreateWindow(WC_COMBOBOX, (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE |
            CBS_SIMPLE,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)ID_EDIT_REPLACE, gHinstMain, (LPVOID)0);
    if (gHwndEditReplace == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Edit Replace Fail!"), (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sInitRebar()
{
    gHwndRebar = ::CreateWindowEx(
            WS_EX_TOOLWINDOW,
            REBARCLASSNAME, (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
            | CCS_NODIVIDER
            | RBS_AUTOSIZE
            ,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)ID_REBAR, gHinstMain, (LPVOID)0);
    if (gHwndRebar == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Rebar Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    REBARINFO rbInfo;
    rbInfo.cbSize = sizeof(REBARINFO);
    rbInfo.fMask = (UINT)0;
    if (::SendMessageA(gHwndRebar, RB_SETBARINFO, 0, (LPARAM)&rbInfo) == 0) {
        ::MessageBox((HWND)0, TEXT("Rebar Set Bar Info Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
}
static void sInitStatusBar()
{
    gHwndStatusbar = ::CreateWindowEx(
            (DWORD)0, STATUSCLASSNAME, (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
            0, 0, 0, 0,
            gHwndFrame, (HMENU)ID_STATUSBAR, gHinstMain, (LPVOID)0);
    if (gHwndStatusbar == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Status Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    int widthsPart[] = {100, 200, 300, 400, -1};
    ::SendMessage(gHwndStatusbar, SB_SETPARTS, sizeof(widthsPart)/sizeof(int), (LPARAM)widthsPart);
    ::SendMessage(gHwndStatusbar, SB_SETTEXT, 0, (LPARAM)TEXT("pos"));
    ::SendMessage(gHwndStatusbar, SB_SETTEXT, 1, (LPARAM)TEXT("encode"));
    ::SendMessage(gHwndStatusbar, SB_SETTEXT, 2, (LPARAM)TEXT("CR+LF"));
    ::SendMessage(gHwndStatusbar, SB_SETTEXT, 3, (LPARAM)TEXT("insert"));
    ::SendMessage(gHwndStatusbar, SB_SETTEXT, 4, (LPARAM)TEXT("Ready"));
}
static void sGetRectEditor(LPCRECT prcClient, LPRECT prcEditor)
{
    SetRect(prcEditor, prcClient->left, prcClient->top  + sRebarGetBarHeight(gHwndRebar),
            prcClient->right, prcClient->bottom - sGetHeight(gHwndStatusbar));
}
HWND sCreateScintilla(int idChild)
{
    RECT rc;
    ::GetClientRect(gHwndFrame, &rc);
    sGetRectEditor(&rc, &rc);
    HWND hwndScintilla = ::CreateWindowEx(
            (DWORD)0, TEXT("Scintilla"), (LPCTSTR)0,
            WS_CHILD | WS_VISIBLE,
            rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
            gHwndFrame, (HMENU)idChild, gHinstMain, (LPVOID)0);
    if (hwndScintilla == (HWND)0) {
        ::MessageBox((HWND)0, TEXT("Create Scintilla Fail!"), (LPCTSTR)0, MB_ICONERROR);
        return (HWND)0;
    }
    return hwndScintilla;
}
static void sEditorReadFromFile(LPCTSTR szFilePath)
{
    sEditorClearAll();
    sEditorEmptyUndoBuffer();
    sEditorSetSavePoint();
    sEditorKey(SCI_CANCEL);
    sEditorSetUndoCollection(FALSE);
    HANDLE hFile = CreateFile(
            szFilePath, GENERIC_READ, FILE_SHARE_READ,
            (LPSECURITY_ATTRIBUTES)0, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, (HANDLE)0);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox((HWND)0, TEXT("Create File Fail!"),
                (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    LARGE_INTEGER liFileSize;
    if (GetFileSizeEx(hFile, &liFileSize) == (BOOL)0) {
        MessageBox((HWND)0, TEXT("Get File Size Fail!"),
                (LPCTSTR)0, MB_ICONERROR);
    }
    BYTE buffer[eBufferSize];
    DWORD dwNumberOfBytesRead;
    LONGLONG liTotalRead = 0;
    while (TRUE) {
        if (ReadFile(hFile, buffer, eBufferSize, &dwNumberOfBytesRead,
                (LPOVERLAPPED)0) == 0) {
            MessageBox((HWND)0, TEXT("Read File Fail!"),
                    (LPCTSTR)0, MB_ICONERROR);
            break;
        }
        sEditorCall(SCI_ADDTEXT, dwNumberOfBytesRead, (LPARAM)buffer);
        liTotalRead += dwNumberOfBytesRead;
        if (liTotalRead == liFileSize.QuadPart) {
            break;
        }
    }
    CloseHandle(hFile);

    sEditorSetUndoCollection(TRUE);
    SetFocus(gEditor.handle);
    sEditorEmptyUndoBuffer();
    sEditorSetSavePoint();
    sEditorGotoPos(0);
}
static void sEditorWriteToFile(LPCTSTR szFilePath)
{
    HANDLE hFile = CreateFile(
            szFilePath, GENERIC_WRITE, (DWORD)0,
            (LPSECURITY_ATTRIBUTES)0, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, (HANDLE)0);
    if (hFile == INVALID_HANDLE_VALUE) {
        MessageBox((HWND)0, TEXT("Create File Fail!"),
                (LPCTSTR)0, MB_ICONERROR);
        return;
    }
    int iFileSize = sEditorCall(SCI_GETLENGTH);
    char buffer[eBufferSize + 1];
    DWORD dwNumberOfBytesWritten;
    int iTotalWritten = 0;
    while (TRUE) {
        int grabSize;
        if (iTotalWritten + eBufferSize > iFileSize) {
            grabSize = sEditorGetTextRange(iTotalWritten, iFileSize, (char*)buffer);
        } else {
            grabSize = sEditorGetTextRange(iTotalWritten, iTotalWritten + eBufferSize, (char*)buffer);
        }
        if (WriteFile(hFile, buffer, grabSize, &dwNumberOfBytesWritten,
                (LPOVERLAPPED)0) == 0) {
            MessageBox((HWND)0, TEXT("Write File Fail!"),
                    (LPCTSTR)0, MB_ICONERROR);
            break;
        }
        iTotalWritten += dwNumberOfBytesWritten;
        if (iTotalWritten == iFileSize) {
            break;
        }
    }
    CloseHandle(hFile);
    sEditorCall(SCI_SETSAVEPOINT);
}
static void sGetPathByOpenDialog(LPTSTR szPathFile)
{
    TCHAR szDir[MAX_PATH];
    GetCurrentDirectory(sizeof(szDir), szDir);
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize       = sizeof(ofn);
    ofn.hwndOwner         = gHwndFrame;
    ofn.hInstance         = gHinstMain;
    ofn.lpstrFilter       = TEXT("Text (*.dat;*.txt)\0*.dat;*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.lpstrCustomFilter = 0;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 2;
    ofn.lpstrFile         = szPathFile;
    ofn.lpstrFile[0]      = TEXT('\0');
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrFileTitle    = NULL;
    ofn.nMaxFileTitle     = 0;
    ofn.lpstrInitialDir   = szDir;
    ofn.lpstrTitle        = NULL;
    ofn.Flags             = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.nFileOffset;
    ofn.nFileExtension;
    ofn.lpstrDefExt;
    ofn.lCustData;
    ofn.lpfnHook;
    ofn.lpTemplateName;
    if (GetOpenFileName(&ofn) == (BOOL)0) {
        MessageBox((HWND)0, TEXT("Get Open File Name Fail!"), 
                (LPCTSTR)0, MB_ICONERROR);
    }
}
static void sGetPathBySaveDialog(LPTSTR szPathFile)
{
    OPENFILENAME ofn = {sizeof(ofn)};
    ofn.lStructSize       = sizeof(ofn);
    ofn.hwndOwner         = gHwndFrame;
    ofn.hInstance         = gHinstMain;
    ofn.lpstrFilter       = TEXT("Text (*.dat;*.txt)\0*.dat;*.txt\0All Files (*.*)\0*.*\0\0");
    ofn.nFilterIndex      = 2;
    ofn.lpstrFile         = szPathFile;
    ofn.lpstrFile[0]      = TEXT('\0');
    ofn.nMaxFile          = MAX_PATH;
    TCHAR szDir[MAX_PATH];
    GetCurrentDirectory(sizeof(szDir), szDir);
    ofn.lpstrInitialDir   = szDir;
    ofn.Flags             = OFN_HIDEREADONLY;
    ofn.lpstrDefExt;
    if (GetOpenFileName(&ofn) == (BOOL)0) {
        MessageBox((HWND)0, TEXT("Get Open File Name Fail!"), 
                (LPCTSTR)0, MB_ICONERROR);
    }
}


static void sEditorToPlainText()
{
    MessageBox(0, TEXT("Pai!"), 0, MB_OK);
}

static void sEditorToCpp()
{
    const char* keywordList =
        "asm auto bool break case catch char class const "
        "const_cast continue default delete do double "
        "dynamic_cast else enum explicit extern false finally "
        "float for friend goto if inline int long mutable "
        "namespace new operator private protected public "
        "register reinterpret_cast register return short signed "
        "sizeof static static_cast struct switch template "
        "this throw true try typedef typeid typename "
        "union unsigned using virtual void volatile "
        "wchar_t while";
    sEditorSetLexer(SCLEX_CPP);
    sEditorSetKeywords(0, keywordList);
    sEditorStyleSetFore(SCE_C_WORD, 0x00FF0000);
    sEditorStyleSetFore(SCE_C_STRING, 0x001515A3);
    sEditorStyleSetFore(SCE_C_CHARACTER, 0x001515A3);
    sEditorStyleSetFore(SCE_C_PREPROCESSOR, 0x00808080);
    sEditorStyleSetFore(SCE_C_COMMENT, 0x00008000);
    sEditorStyleSetFore(SCE_C_COMMENTLINE, 0x00008000);
    sEditorStyleSetFore(SCE_C_COMMENTDOC, 0x00008000);
}
static void sUpdateControlDirty()
{

}
static void sUpdateControlFileType()
{

}
static void sUpdateControlReadOnly()
{
    if (gEditor.flagReadOnly) {
        sToolbarSetState(gHwndEditorOpts, IDM_READONLY, TBSTATE_ENABLED | TBSTATE_CHECKED);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_READONLY, TBSTATE_ENABLED);
    }
    sEditorSetReadOnly(gEditor.flagReadOnly);
}
static void sUpdateControlViewLineEnding()
{
    if (gEditor.flagViewLineEnding) {
        sToolbarSetState(gHwndEditorOpts, IDM_VIEWLINEENDING, TBSTATE_ENABLED | TBSTATE_CHECKED);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_VIEWLINEENDING, TBSTATE_ENABLED);
    }
    sEditorSetViewEol(gEditor.flagViewLineEnding);
}
static void sUpdateControlViewWhiteSpace()
{
    if (gEditor.flagViewWhiteSpace) {
        sToolbarSetState(gHwndEditorOpts, IDM_VIEWWHITESPACE, TBSTATE_ENABLED | TBSTATE_CHECKED);
        sEditorSetViewWs(SCWS_VISIBLEAFTERINDENT);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_VIEWWHITESPACE, TBSTATE_ENABLED);
        sEditorSetViewWs(SCWS_INVISIBLE);
    }
}
static void sUpdateControlLineWrap()
{
    if (gEditor.flagLineWrap) {
        sToolbarSetState(gHwndEditorOpts, IDM_LINEWRAP, TBSTATE_ENABLED | TBSTATE_CHECKED);
        sEditorSetWrapMode(SC_WRAP_CHAR);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_LINEWRAP, TBSTATE_ENABLED);
        sEditorSetWrapMode(SC_WRAP_NONE);
    }
}
static void sUpdateControlUseTabs()
{
    if (gEditor.flagUseTabs) {
        sToolbarSetState(gHwndEditorOpts, IDM_USETABS, TBSTATE_ENABLED | TBSTATE_CHECKED);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_USETABS, TBSTATE_ENABLED);
    }
    sEditorSetUseTabs(gEditor.flagUseTabs);
}
static void sUpdateControlOverType()
{
    if (gEditor.flagOvertype) {
        sToolbarSetState(gHwndEditorOpts, IDM_OVERTYPE, TBSTATE_ENABLED | TBSTATE_CHECKED);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_OVERTYPE, TBSTATE_ENABLED);
    }
    sEditorSetOverType(gEditor.flagOvertype);
}
static void sUpdateControlLineNumber()
{
    if (gEditor.flagLineNumber) {
        sToolbarSetState(gHwndEditorOpts, IDM_LINENUMBER, TBSTATE_ENABLED | TBSTATE_CHECKED);
        sEditorSetMarginWidthN(0, 30);
    } else {
        sToolbarSetState(gHwndEditorOpts, IDM_LINENUMBER, TBSTATE_ENABLED);
        sEditorSetMarginWidthN(0, 0);
    }
}
static void sEditorDefault()
{
    sEditorSetScrollWidth(500);
    sEditorSetMarginTypeN(0, SC_MARGIN_NUMBER);
    sEditorStyleSetFore(STYLE_DEFAULT, eColorBlack);
    sEditorStyleSetBack(STYLE_DEFAULT, eColorWhite);
    sEditorStyleSetFont(STYLE_DEFAULT, "Courier New");
    sEditorStyleSetSize(STYLE_DEFAULT, 10);
    sEditorStyleClearAll();
    sEditorSetCodePage(SC_CP_UTF8);
}
static void sUpdateAllControls()
{
    sUpdateControlDirty();
    sUpdateControlFileType();
    sUpdateControlReadOnly();
    sUpdateControlOverType();
    sUpdateControlLineWrap();
    sUpdateControlViewWhiteSpace();
    sUpdateControlViewLineEnding();
    sUpdateControlLineNumber();
    sUpdateControlUseTabs();
}
static void sNewEditor(LPCTSTR szFilePath)
{
    gEditor.handle = sCreateScintilla(ID_EDITOR);
    gEditor.directFunctionToEditor = (int (__cdecl *)(void*, int, int, int))SendMessage(gEditor.handle,
            SCI_GETDIRECTFUNCTION, 0, 0);
    gEditor.directPointerToEditor = (void *)SendMessage(gEditor.handle, SCI_GETDIRECTPOINTER, 0, 0);
    sEditorDefault();
    if (szFilePath) {
        sEditorReadFromFile(szFilePath);
        sStrNCpy(gEditor.pathSource, szFilePath, MAX_PATH);
    } else {

        sEditorReadFromFile(TEXT("F:\\LWD\\wedsimple\\codepage\\UTF-8-BOM.txt"));

        gEditor.pathSource[0] = TEXT('\0');
    }

    gEditor.indexFileType = 0;

    gEditor.flagDirty = FALSE;

    gEditor.encode[0] = TEXT('\0');

    gEditor.eolMode = sEditorGetEolMode();
    gEditor.flagOvertype = sEditorGetOverType();

    gEditor.flagReadOnly = sEditorGetReadOnly();
    gEditor.flagAutoIndent = 0;
    gEditor.flagLineWrap = ((sEditorGetWrapMode() != SC_WRAP_NONE) ? TRUE : FALSE);
    gEditor.flagViewWhiteSpace = ((sEditorGetViewWs() != SCWS_INVISIBLE) ? TRUE : FALSE);
    gEditor.flagViewLineEnding = sEditorGetViewEol(); 
    gEditor.flagUseTabs = sEditorGetUseTabs();
    gEditor.flagLineNumber = TRUE;

}

static void sResize(int widthClient, int heightClient)
{
    ::SendMessage(gHwndToolbar, TB_AUTOSIZE, (WPARAM)0, (LPARAM)0);
    ::SendMessage(gHwndStatusbar, WM_SIZE, (WPARAM)0, (LPARAM)0);
    RECT rcClient;
    ::SetRect(&rcClient, 0, 0, widthClient, heightClient);
    RECT rcWinScintilla;
    sGetRectEditor(&rcClient, &rcWinScintilla);
    HDWP hdwp;
    hdwp = BeginDeferWindowPos(1);
    DeferWindowPos(hdwp, gEditor.handle, HWND_TOP, rcWinScintilla.left, rcWinScintilla.top,
        rcWinScintilla.right - rcWinScintilla.left, rcWinScintilla.bottom - rcWinScintilla.top, (UINT)0);
    EndDeferWindowPos(hdwp);
}

static void sNew()
{
    sNewEditor((LPCTSTR)0);
    sUpdateAllControls();
}

static void sOpen()
{
    TCHAR szPathFile[MAX_PATH];
    sGetPathByOpenDialog(szPathFile);
    sNewEditor(szPathFile);
    sUpdateAllControls();
}

static void sSave()
{
}

static void sSaveAs()
{
    TCHAR szPathFile[MAX_PATH];
    sGetPathBySaveDialog(szPathFile);
    sEditorWriteToFile(szPathFile);
}

static void sClose()
{

}

static void sAbout()
{
    LOOKINT(gEditor.indexFileType);
    LOOKINT(eBufferSize);
    LOOKSTR(eAppName);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int ncmdshow)
{
    gHinstMain = hInstance;
    sInitCommonControls();
    sInitScintillaLibrary();
    sRegisterFrameClass();
    sInitFrame(ncmdshow);
    sInitMenu();
    sInitToolBar();

    sInitListFileType();
    sInitEditorOpts();
    sInitSearchOpts();
    sInitEditSearch();
    sInitEditReplace();
    sInitRebar();
    sRebarAddToolbar(gHwndRebar, gHwndToolbar);
    sRebarAddCombo(gHwndRebar, gHwndListFileType, (LPCTSTR)0);
    sRebarAddToolbar(gHwndRebar, gHwndEditorOpts);
    sRebarAddToolbar(gHwndRebar, gHwndSearchOpts);
    sRebarAddCombo(gHwndRebar, gHwndEditSearch, TEXT("Search"));
    sRebarAddCombo(gHwndRebar, gHwndEditReplace, TEXT("Replace"));
    sInitStatusBar();
    sNew();
    ::ShowWindow(gHwndFrame, ncmdshow);
    ::UpdateWindow(gHwndFrame);
    MSG msg;
    while (::GetMessage(&msg, (HWND)0, (UINT)0, (UINT)0)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT        rcClient;
    NMHDR*      lpnmhdr;
    switch (uMsg) {
    case WM_CREATE:
        return 0;
    case WM_SIZE:
        sResize(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_LIST_FILETYPE:
            switch (HIWORD(wParam)) {
            case CBN_SELCHANGE: {
                int indexFileTypeSel = sComboGetCurSel(gHwndListFileType);
                if (gEditor.indexFileType != indexFileTypeSel) {
                    gEditor.indexFileType = indexFileTypeSel;
                    gFileTypeTable[indexFileTypeSel].pFunFiletype();
                }
                return 0;
                }
            }
            break;
        case IDM_READONLY:
            gEditor.flagReadOnly = !gEditor.flagReadOnly;
            sUpdateControlReadOnly();
            return 0;
        case IDM_USETABS:
            gEditor.flagUseTabs = !gEditor.flagUseTabs;
            sUpdateControlUseTabs();
            return 0;
        case IDM_LINEWRAP:
            gEditor.flagLineWrap = !gEditor.flagLineWrap;
            sUpdateControlLineWrap();
            return 0;
        case IDM_VIEWWHITESPACE:
            gEditor.flagViewWhiteSpace = !gEditor.flagViewWhiteSpace;
            sUpdateControlViewWhiteSpace();
            return 0;
        case IDM_VIEWLINEENDING:
            gEditor.flagViewLineEnding = !gEditor.flagViewLineEnding;
            sUpdateControlViewLineEnding();
            return 0;
        case IDM_OVERTYPE:
            gEditor.flagOvertype = !gEditor.flagOvertype;
            sUpdateControlOverType();
            return 0;
        case IDM_LINENUMBER:
            gEditor.flagLineNumber = !gEditor.flagLineNumber;
            sUpdateControlLineNumber();
            return 0;
        case IDM_NEW:
            sNew();
            return 0;
        case IDM_OPEN:
            sOpen();
            return 0;
        case IDM_SAVE:
            sSave();
            return 0;
        case IDM_SAVEAS:
            sSaveAs();
            return 0;
        case IDM_EXIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;
        case IDM_UNDO:
            sEditorUndo();
            return 0;
        case IDM_CUT:
            sEditorCut();
            return 0;
        case IDM_COPY:
            sEditorCopy();
            return 0;
        case IDM_PASTE:
            sEditorPaste();
            return 0;
        case IDM_CLEAR:
            sEditorClear();
            return 0;
        case IDM_SELECTALL:
            sEditorSelectAll();
            return 0;
        case IDM_ABOUT:
            sAbout();
            return 0;
        }
        break;
    case WM_NOTIFY:
        lpnmhdr = (LPNMHDR)lParam;
        if (lpnmhdr->hwndFrom == gEditor.handle) {
            switch (lpnmhdr->code) {
            case SCN_CHARADDED:
                break;
            }
        }
        if (lpnmhdr->hwndFrom == gHwndRebar) {
            switch (lpnmhdr->code) {
            case RBN_HEIGHTCHANGE:
                ::GetClientRect(hWnd, &rcClient);
                sResize(rcClient.right, rcClient.bottom);
                break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
