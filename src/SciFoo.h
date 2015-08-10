#pragma once

#include "Scintilla.h"

sptr_t sEditorCall(unsigned int uMsg, uptr_t wParam = 0, sptr_t lParam = 0);
static void sEditorKey(unsigned int uKey)
{
    sEditorCall(uKey);
}
void sEditorGetText(int length, char* text);
void sEditorSetText(const char* text);
static void sEditorSetSavePoint()
{
    sEditorCall(SCI_SETSAVEPOINT);
}
void sEditorGetLine(int line, char* text);
void sEditorReplaceSel(const char* text);
static void sEditorSetReadOnly(bool readOnly)
{
    sEditorCall(SCI_SETREADONLY, (uptr_t)readOnly);
}
static bool sEditorGetReadOnly()
{
    return (bool)(sEditorCall(SCI_GETREADONLY));
}
static long sEditorGetTextRange(long start, long end, char* strText)
{
    Sci_TextRange textrange;
    textrange.chrg.cpMin = start;
    textrange.chrg.cpMax = end;
    textrange.lpstrText = strText;
    return (long)sEditorCall(SCI_GETTEXTRANGE, 0, (sptr_t)&textrange);
}
void sEditorGetStyledText(void);
void sEditorAllocate(int bytes);
void sEditorAddText(int length, const char* s);
void sEditorAddStyledText(void);
void sEditorAppendText(int length, const char* s);
void sEditorInsertText(int pos, const char* text);
void sEditorChangeInsertion(int length, const char* text);
void sEditorClearAll()
{
    sEditorCall(SCI_CLEARALL);
}
void sEditorDeleteRange(int pos, int deleteLength);
void sEditorClearDocumentStyle();
void sEditorGetCharAt(int pos);
void sEditorGetStyleAt(int post);
void sEditorReleaseAllExtendedStyles();
void sEditorAllocateExtendedStyles(int numberStyles);
void sEditorTargetAsUTF8(char* s);
void sEditorEncodedFromUTF8(const char* utf8, char* encoded);
void sEditorSetLengthForEncode(int bytes);
void sEditorSetTargetStart(int pos);
void sEditorGetTargetStart();
void sEditorSetTargetEnd(int pos);
void sEditorGetTargetEnd();
void sEditorSetTargetRange(int start, int end);
void sEditorTargetFromSelection();
void sEditorTargetWholeDocument();
void sEditorSetSearchFlags(int searchFlags);
void sEditorGetSearchFlags();
void sEditorSearchInTarget(int length, const char* text);
void sEditorGetTargetText(char* text);
void sEditorReplaceTarget(int length, const char* text);
void sEditorReplaceTargetRe(int length, const char* text);
void sEditorGetTag(int tagNumber, char* tagValue);
void sEditorFindText(int searchFlags, void*);
void sEditorSearchAnchor();
void sEditorSearchNext(int searchFlags, const char* text);
void sEditorSearchPrev(int searchFlags, const char* text);
static void sEditorSetOverType(bool overType)
{
    sEditorCall(SCI_SETOVERTYPE, (uptr_t)overType);
}
static bool sEditorGetOverType()
{
    return (bool)(sEditorCall(SCI_GETOVERTYPE));
}
static void sEditorCut()
{
    sEditorCall(SCI_CUT);
}
static void sEditorCopy()
{
    sEditorCall(SCI_COPY);
}
static void sEditorPaste()
{
    sEditorCall(SCI_PASTE);
}
static void sEditorClear()
{
    sEditorCall(SCI_CLEAR);
}
void sEditorCanPaste();
void sEditorCopyAllowLine();
void sEditorCopyRange(int start, int end);
void sEditorCopyText(int length, const char* text);
void sEditorSetPasteConvertEndings(bool convert);
void sEditorGetPasteConvertEndings();
void sEditorSetStatus(int status);
void sEditorGetStatus();
static void sEditorUndo()
{
    sEditorCall(SCI_UNDO);
}
void sEditorCanUndo();
static void sEditorRedo()
{
    sEditorCall(SCI_REDO);
}
void sEditorCanRedo();
static void sEditorEmptyUndoBuffer()
{
    sEditorCall(SCI_EMPTYUNDOBUFFER);
}
static void sEditorSetUndoCollection(bool collectUndo)
{
    sEditorCall(SCI_SETUNDOCOLLECTION, (uptr_t)collectUndo);
}
void sEditorGetUndoCollection();
void sEditorBeginUndoAction();
void sEditorEndUndoAction();
void sEditorAddUndoAction(int token, int flags);
static int sEditorGetTextLength()
{
    return (int)sEditorCall(SCI_GETTEXTLENGTH);
}
int sEditorGetLength();
static int sEditorGetLineCount()
{
    return (int)sEditorCall(SCI_GETLINECOUNT);
}
static void sEditorSetFirstVisibleLine(int lineDisplay)
{
    sEditorCall(SCI_SETFIRSTVISIBLELINE, (uptr_t)lineDisplay);
}
static int sEditorGetFirstVisibleLine()
{
    return (int)sEditorCall(SCI_GETFIRSTVISIBLELINE);
}
static int sEditorLinesOnScreen()
{
    return (int)sEditorCall(SCI_LINESONSCREEN);
}
static bool sEditorGetModify()
{
    return (bool)sEditorCall(SCI_GETMODIFY);
}
void sEditorSetSel(int anchorPos, int currentPos);
static void sEditorGotoPos(int pos)
{
    sEditorCall(SCI_GOTOPOS, (uptr_t)pos);
}
void sEditorGotoLine(int line);
void sEditorSetCurrentPos(int pos);
void sEditorGetCurrentPos();
void sEditorSetAnchor(int pos);
void sEditorGetAnchor();
void sEditorSetSelectionStart(int pos);
void sEditorSetSelectionEnd(int pos);
void sEditorGetSelectionStart();
void sEditorSetSelectionEnd();
void sEditorSetEmptySelection(int pos);
static void sEditorSelectAll()
{
    sEditorCall(SCI_SELECTALL);
}
void sEditorLineFromPosition(int pos);
void sEditorPositionFromLine(int line);
void sEditorGetLineEndPosition(int line);
void sEditorLineLength(int line);
void sEditorGetSelText(char* text);
void sEditorGetCurLine(int textLen, char* text);
void sEditorSelectionIsRectangle();
void sEditorSetSelectionMode(int mode);
void sEditorGetSelectionMode();
void sEditorGetLineSelStartPosition(int line);
void sEditorGetLineSelEndPosition(int line);
void sEditorMoveCaretInSideView();
void sEditorWordEndPosition(int position, bool onlyWordCharacters);
void sEditorWordStartPosition(int position, bool onlyWordCharacters);
void sEditorIsRangeWord(int start, int end);
void sEditorPositionBefore(int position);
void sEditorPositionAfter(int position);
void sEditorPositionRelative(int position, int relative);
void sEditorCountCharacters(int startPos, int endPos);
void sEditorTextWidth(int styleNumber, const char* text);
void sEditorTextHeight(int line);
void sEditorGetColumn(int pos);
void sEditorFindColumn(int line, int column);
void sEditorPositionFromPoint(int x, int y);
void sEditorPositionFromPointClose(int x, int y);
void sEditorCharPositionFromPoint(int x, int y);
void sEditorCharPositionFromPointClose(int x, int y);
void sEditorPointXFromPosition(int pos);
void sEditorPointYFromPosition(int pos);
void sEditorHideSelection(bool hide);
void sEditorChooseCaretX();
void sEditorMoveSelectedLinesUp();
void sEditorMoveSelectedLinesDown();
void sEditorSetMouseSelectionRectangularSwitch(bool mouseSelectionRectangularSwitch);
void sEditorGetMouseSelectionRectangularSwitch();
void sEditorSetMultipleSelection(bool multipleSelection);
void sEditorGetMultipleSelection();
void sEditorSetAdditionalSelectionTyping(bool additionalSelectionTyping);
void sEditorGetAdditionalSelectionTyping();
void sEditorSetMultiPaste(int multiPaste);
void sEditorGetMultiPaste();
void sEditorSetVirtualSpaceOptions(int virtualSpaceOptions);
void sEditorGetVirtualSpaceOptions();
void sEditorSetRectangularSelectionModifier(int modifier);
void sEditorGetRectangularSelectionModifier();
void sEditorGetSelections();
void sEditorGetSelectionEmpty();
void sEditorClearSelections();
void sEditorSetSelection(int caret, int anchor);
void sEditorAddSelection(int caret, int anchor);
void sEditorDropSelectionN(int selection);
void sEditorSetMainSelection(int selection);
void sEditorGetMainSelection();
void sEditorSetSelectionNCaret(int selection, int pos);
void sEditorGetSelectionNCaret(int selection);
void sEditorSetSelectionNCaretVirtualSpace(int selection, int space);
void sEditorGetSelectionNCaretVirtualSpace(int selection);
void sEditorSetSelectionNAnchor(int selection, int posAnchor);
void sEditorGetSelectionNAnchor(int selection);
void sEditorSetSelectionNAnchorVirtualSpace(int selection, int space);
void sEditorGetSelectionNAnchorVirtualSpace(int selection);
void sEditorSetSelectionNStart(int selection, int pos);
void sEditorGetSelectionNStart(int selection);
void sEditorSetSelectionNEnd(int selection, int pos);
void sEditorGetSelectionNEnd(int selection);
void sEditorSetRectangularSelectionCaret(int pos);
void sEditorGetRectangularSelectionCaret();;
void sEditorSetRectangularSelectionCaretVirtualSpace(int space);
void sEditorGetRectangularSelectionCaretVirtualSpace();
void sEditorSetRectangularSelectionAnchor(int posAnchor);
void sEditorGetRectangularSelectionAnchor();
void sEditorSetRectangularSelectionAnchorVirtualSpace(int space);
void sEditorGetRectangularSelectionAnchorVirtualSpace();
void sEditorSetAdditionalSelAlpha(int alpha);
void sEditorGetAdditionalSelAlpha();
void sEditorSetAdditionalSelFore(int colour);
void sEditorSetAdditionalSelBack(int colour);
void sEditorSetAdditionalCaretFore(int colour);
void sEditorGetAdditionalCaretFore();
void sEditorSetAdditionalCaretsBlink(bool additionalCaretsBlink);
void sEditorGetAdditionalCaretsBlink();
void sEditorSetAdditionalCaretsVisible(bool additionalCaretsVisible);
void sEditorGetAdditionalCaretsVisible();
void sEditorSwapMainAnchorCaret();
void sEditorRotateSelection();
void sEditorMultipleSelectAddNext();
void sEditorMultipleSelectAddEach();
void sEditorLineScroll(int column, int line);
void sEditorScrollCaret();
void sEditorScrollRange(int secondary, int primary);
void sEditorSetXCaretPolicy(int caretPolicy, int caretSlop);
void sEditorSetYCaretPolicy(int caretPolicy, int caretSlop);
void sEditorSetVisiblePolicy(int caretPolicy, int caretSlop);
void sEditorSetHScrollbar(bool visible);
void sEditorGetHScrollbar();
void sEditorSetVScrollbar(bool visible);
void sEditorGetVScrollbar();
void sEditorGetXOffset();
void sEditorSetXOffset(int xOffset);
static void sEditorSetScrollWidth(int pixelWidth)
{
    sEditorCall(SCI_SETSCROLLWIDTH, (uptr_t)pixelWidth);
}
static int sEditorGetScrollWidth()
{
    return (int)sEditorCall(SCI_GETSCROLLWIDTH);
}
static void sEditorSetScrollWidthTracking(bool tracking)
{
    sEditorCall(SCI_SETSCROLLWIDTHTRACKING, (uptr_t)tracking);
}
void sEditorGetScrollWidthTracking();
void sEditorSetEndAtLastLine(bool endAtLastLine);
void sEditorGetEndAtLastLine();
static void sEditorSetViewWs(int wsMode)
{
    sEditorCall(SCI_SETVIEWWS, wsMode);
}
static int sEditorGetViewWs()
{
    return (int)sEditorCall(SCI_GETVIEWWS);
}
void sEditorSetWhitespaceFore(bool useWhitespaceForeColour, int colour);
void sEditorSetWhitespaceBack(bool useWhitespaceBackColour, int colour);
void sEditorSetWhitespaceSize(int size);
void sEditorGetWhitespaceSize();
void sEditorSetExtraAscent(int extraAscent);
void sEditorGetExtraAscent();
void sEditorSetExtraDescent(int extraDescent);
void sEditorGetExtraDescent();
void sEditorSetCursor(int curType);
void sEditorGetCursor();
void sEditorSetMouseDownCaptures(bool captures);
void sEditorGetMouseDownCaptures();
static void sEditorSetEolMode(int eolMode)
{
    ::sEditorCall(SCI_SETEOLMODE, eolMode);
}
static int sEditorGetEolMode()
{
    return (int)::sEditorCall(SCI_GETEOLMODE);
}
void sEditorConvertEols(int eolMode);
static void sEditorSetViewEol(bool visible)
{
    ::sEditorCall(SCI_SETVIEWEOL, visible);
}
static bool sEditorGetViewEol()
{
    return (bool)sEditorCall(SCI_GETVIEWEOL);
}
void sEditorGetLineEndTypesSupported();
void sEditorSetLineEndTypesAllowed(int lineEndBitSet);
void sEditorGetLineEndTypesAllowed();
void sEditorGetLineEndTypesActive();
void sEditorGetEndStyled();
void sEditorStartStyling(int position, int unused);
void sEditorSetStyling(int length, int style);
void sEditorSetStylingEx(int length, const char *styles);
void sEditorSetLineState(int line, int value);
void sEditorGetlineState(int line);
void sEditorGetMaxLineState();
static void sEditorStyleResetDefault()
{
    sEditorCall(SCI_STYLERESETDEFAULT);
}
static void sEditorStyleClearAll()
{
    sEditorCall(SCI_STYLECLEARALL);
}
static void sEditorStyleSetFont(int styleNumber, char *fontName)
{
    sEditorCall(SCI_STYLESETFONT, (uptr_t)styleNumber, (sptr_t)fontName);
}
static void sEditorStyleGetFont(int styleNumber, char *fontName)
{
    sEditorCall(SCI_STYLEGETFONT, (uptr_t)styleNumber, (sptr_t)fontName);
}
static void sEditorStyleSetSize(int styleNumber, int sizeInPoints)
{
    sEditorCall(SCI_STYLESETSIZE, (uptr_t)styleNumber, (sptr_t)sizeInPoints);
}
void sEditorStyleGetSize(int styleNumber);
void sEditorStyleSetSizeFractional(int styleNumber, int sizeInHundredthPoints);
void sEditorStyleGetSizeFractional(int styleNumber);
void sEditorStyleSetBold(int styleNumber, bool bold);
void sEditorStyleGetBold(int styleNumber);
void sEditorStyleSetWeight(int styleNumber, int weight);
void sEditorStyleGetWeight(int styleNumber);
void sEditorStyleSetItalic(int styleNumber, bool italic);
void sEditorStyleGetItalic(int styleNumber);
void sEditorStyleSetUnderline(int styleNumber, bool underline);
void sEditorStyleGetUnderline(int styleNumber);
static void sEditorStyleSetFore(int styleNumber, int colour)
{
    sEditorCall(SCI_STYLESETFORE, styleNumber, colour);
}
void sEditorStyleGetFore(int styleNumber);
static void sEditorStyleSetBack(int styleNumber, int colour)
{
    sEditorCall(SCI_STYLESETBACK, styleNumber, colour);
}
void sEditorStyleGetBack(int styleNumber);
void sEditorStyleSetEolFilled(int styleNumber, bool eolFilled);
void sEditorStyleGetEolFilled(int styleNumber);
void sEditorStyleSetCharacterSet(int styleNumber, int charSet);
void sEditorStyleGetCharacterSet(int styleNumber);
void sEditorStyleSetCase(int styleNumber, int caseMode);
void sEditorStyleGetCase(int styleNumber);
void sEditorStyleSetVisible(int styleNumber, bool visible);
void sEditorStyleGetVisible(int styleNumber);
void sEditorStyleSetChangeable(int styleNumber, bool changeable);
void sEditorStyleGetChangeable(int styleNumber);
void sEditorStyleSetHotspot(int styleNumber, bool hotspot);
void sEditorStyleGetHotspot(int styleNumber);
void sEditorSetSelFore(bool useSelectionForeColour, int colour);
void sEditorSetSelBack(bool useSelectionBackColour, int colour);
void sEditorSetSelAlpha(int alpha);
void sEditorGetSelAlpha();
void sEditorSetSelEolFilled(bool filled);
void sEditorGetSelEolFilled();
void sEditorSetCaretFore(int colour);
void sEditorGetCaretFore();
void sEditorSetCaretLineVisible(bool show);
void sEditorGetCaretLineVisible();
void sEditorSetCaretLineBack(int colour);
void sEditorGetCaretLineBack();
void sEditorSetCaretLineBackAlpha(int alpha);
void sEditorGetCaretLineBackAlpha();
void sEditorSetCaretLineVisibleAlways(bool alwaysVisible);
void sEditorGetCaretLineVisibleAlways();
void sEditorSetCaretPeriod(int milliseconds);
void sEditorGetCaretPeriod();
void sEditorSetCaretStyle(int style);
void sEditorGetCaretStyle();
void sEditorSetCaretWidth(int pixels);
void sEditorGetCaretWidth();
void sEditorSetHotspotActiveFore(bool useSetting, int colour);
void sEditorGetHotspotActiveFore();
void sEditorSetHotspotActiveBack(bool useSetting, int colour);
void sEditorGetHotspotActiveBack();
void sEditorSetHotspotActiveUnderline(bool underline);
void sEditorGetHotspotActiveUnderline();
void sEditorSetHotspotSingleLine(bool singleLine);
void sEditorGetHotspotSingleLine();
void sEditorSetCaretSticky(int useCaretStickyBehaviour);
void sEditorGetCaretSticky();
void sEditorToggleCaretSticky();
void sEditorSetRepresentation(const char *encodedCharacter, const char *representation);
void sEditorGetRepresentation(const char *encodedCharacter, char *representation);
void sEditorClearRepresentation(const char *encodedCharacter);
void sEditorSetControlCharSymbol(int symbol);
void sEditorGetControlCharSymbol();
static void sEditorSetMarginTypeN(int margin, int type)
{
    sEditorCall(SCI_SETMARGINTYPEN, type);
}
void sEditorGetMarginTypeN(int margin);
static void sEditorSetMarginWidthN(int margin, int pixelWidth)
{
    sEditorCall(SCI_SETMARGINWIDTHN, margin, pixelWidth);
}
static int sEditorGetMarginWidthN(int margin)
{
    return (int)sEditorCall(SCI_GETMARGINWIDTHN, margin);
}
void sEditorSetMarginMaskN(int margin, int mask);
void sEditorGetMarginMaskN(int margin);
void sEditorSetMarginSensitiveN(int margin, bool sensitive);
void sEditorGetMarginSensitiveN(int margin);
void sEditorSetMarginCursorN(int margin, int cursor);
void sEditorGetMarginCursorN(int margin);
void sEditorSetMarginLeft(int pixels);
void sEditorGetMarginLeft();
void sEditorSetMarginRight(int pixels);
void sEditorGetMarginRight();
void sEditorSetFoldMarginColour(bool useSetting, int colour);
void sEditorSetFoldMarginHiColour(bool useSetting, int colour);
void sEditorMarginSetText(int line, char *text);
void sEditorMarginGetText(int line, char *text);
void sEditorMarginSetStyle(int line, int style);
void sEditorMarginGetStyle(int line);
void sEditorMarginSetStyles(int line, char *styles);
void sEditorMarginGetStyles(int line, char *styles);
void sEditorMarginTextClearAll();
void sEditorMarginSetStyleOffset(int style);
void sEditorMarginGetStyleOffset();
void sEditorSetMarginOptions(int marginOptions);
void sEditorGetMarginOptions();
void sEditorannotationsettext(int line, char *text);
void sEditorannotationgettext(int line, char *text);
void sEditorannotationsetstyle(int line, int style);
void sEditorannotationgetstyle(int line);
void sEditorannotationsetstyles(int line, char *styles);
void sEditorannotationgetstyles(int line, char *styles);
void sEditorannotationgetlines(int line);
void sEditorannotationclearall();
void sEditorannotationsetvisible(int visible);
void sEditorannotationgetvisible();
void sEditorannotationsetstyleoffset(int style);
void sEditorannotationgetstyleoffset();
void sEditorSetBufferedDraw(bool isBuffered);
void sEditorGetBufferedDraw();
void sEditorSetPhasesDraw(int phases);
void sEditorGetPhasesDraw();
void sEditorSetTwoPhaseDraw(bool twoPhase);
void sEditorGetTwoPhaseDraw();
void sEditorSetTechnology(int technology);
void sEditorGetTechnology();
void sEditorSetFontQuality(int fontQuality);
void sEditorGetFontQuality();
static void sEditorSetCodePage(int codePage)
{
    sEditorCall(SCI_SETCODEPAGE, codePage);
}
static int sEditorGetCodePage()
{
    return (int)sEditorCall(SCI_GETCODEPAGE);
}
void sEditorSetImeInteraction(int imeInteraction);
void sEditorGetImeInteraction();
void sEditorSetWordChars(const char *characters);
void sEditorGetWordChars(char *characters);
void sEditorSetWhitespaceChars(const char *characters);
void sEditorGetWhitespaceChars(char *characters);
void sEditorSetPunctuationChars(const char *characters);
void sEditorGetPunctuationChars(char *characters);
void sEditorSetCharsDefault();
void sEditorGrabFocus();
void sEditorSetFocus(bool focus);
void sEditorGetFocus();
void sEditorBraceHighlight(int pos1, int pos2);
void sEditorBraceBadlight(int pos1);
void sEditorBraceHighlightIndicator(bool useBraceHighlightIndicator, int indicatorNumber);
void sEditorBraceBadlightIndicator(bool useBraceBadlightIndicator, int indicatorNumber);
void sEditorBraceMatch(int position, int maxReStyle);
void sEditorSetTabWidth(int widthInChars);
void sEditorGetTabWidth();

void sEditorClearTabStops(int line);
void sEditorAddTabStop(int line, int x);
void sEditorGetNextTabStop(int line, int x);
static void sEditorSetUseTabs(bool useTabs)
{
    sEditorCall(SCI_SETUSETABS, useTabs);
}
static bool sEditorGetUseTabs()
{
    return (bool)::sEditorCall(SCI_GETUSETABS);
}
void sEditorSetIndent(int widthInChars);
void sEditorGetIndent();

void sEditorSetTabIndents(bool tabIndents);
void sEditorGetTabIndents();

void sEditorSetBackspaceUnindents(bool bsUnindents);
void sEditorGetBackspaceUnindents();
void sEditorSetLineIndentation(int line, int indentation);
void sEditorGetLineIndentation(int line);
void sEditorGetLineIndentPosition(int line);
void sEditorSetIndentationGuides(int indentview);
void sEditorGetIndentationGuides();
void sEditorSetHighlightGuide(int column);
void sEditorGetHighlightGuide();
void sEditormarkerdefine(int markernumber, int markersymbols);
void sEditormarkerdefinepixmap(int markernumber, const char *xpm);
void sEditorrgbaimagesetwidth(int width);
void sEditorrgbaimagesetheight(int height);
void sEditorrgbaimagesetscale(int scalepercent);
void sEditormarkerdefinergbaimage(int markernumber, const char *pixels);
void sEditormarkersymboldefined(int markernumber);
void sEditormarkersetfore(int markernumber, int colour);
void sEditormarkersetback(int markernumber, int colour);
void sEditormarkersetbackselected(int markernumber, int colour);
void sEditormarkerenablehighlight(int enabled);
void sEditormarkersetalpha(int markernumber, int alpha);
void sEditormarkeradd(int line, int markernumber);
void sEditormarkeraddset(int line, int markermask);
void sEditormarkerdelete(int line, int markernumber);
void sEditormarkerdeleteall(int markernumber);
void sEditormarkerget(int line);
void sEditormarkernext(int linestart, int markermask);
void sEditormarkerprevious(int linestart, int markermask);
void sEditormarkerlinefromhandle(int handle);
void sEditormarkerdeletehandle(int handle);
void sEditorindicsetstyle(int indicatornumber, int indicatorstyle);
void sEditorindicgetstyle(int indicatornumber);
void sEditorindicsetfore(int indicatornumber, int colour);
void sEditorindicgetfore(int indicatornumber);
void sEditorindicsetalpha(int indicatornumber, int alpha);
void sEditorindicgetalpha(int indicatornumber);
void sEditorindicsetoutlinealpha(int indicatornumber, int alpha);
void sEditorindicgetoutlinealpha(int indicatornumber);
void sEditorindicsetunder(int indicatornumber, bool under);
void sEditorindicgetunder(int indicatornumber);
void sEditorindicsethoverstyle(int indicatornumber, int indicatorstyle);
void sEditorindicgethoverstyle(int indicatornumber);
void sEditorindicsethoverfore(int indicatornumber, int colour);
void sEditorindicgethoverfore(int indicatornumber);
void sEditorindicsetflags(int indicatornumber, int flags);
void sEditorindicgetflags(int indicatornumber);
void sEditorsetindicatorcurrent(int indicator);
void sEditorgetindicatorcurrent();
void sEditorsetindicatorvalue(int value);
void sEditorgetindicatorvalue();
void sEditorindicatorfillrange(int position, int filllength);
void sEditorindicatorclearrange(int position, int clearlength);
void sEditorindicatorallonfor(int position);
void sEditorindicatorvalueat(int indicator, int position);
void sEditorindicatorstart(int indicator, int position);
void sEditorindicatorend(int indicator, int position);
void sEditorfindindicatorshow(int start, int end);
void sEditorfindindicatorflash(int start, int end);
void sEditorfindindicatorhide();
void sEditorautocshow(int lenentered, const char *list);
void sEditorautoccancel();
void sEditorautocactive();
void sEditorautocposstart();
void sEditorautoccomplete();
void sEditorautocstops(const char *chars);
void sEditorautocsetseparator(char separator);
void sEditorautocgetseparator();
void sEditorautocselect(const char *select);
void sEditorautocgetcurrent();
void sEditorautocgetcurrenttext(char *text);
void sEditorautocsetcancelatstart(bool cancel);
void sEditorautocgetcancelatstart();
void sEditorautocsetfillups(const char *chars);
void sEditorautocsetchoosesingle(bool choosesingle);
void sEditorautocgetchoosesingle();
void sEditorautocsetignorecase(bool ignorecase);
void sEditorautocgetignorecase();
void sEditorautocsetcaseinsensitivebehaviour(int behaviour);
void sEditorautocgetcaseinsensitivebehaviour();
void sEditorautocsetmulti(int multi);
void sEditorautocgetmulti();
void sEditorautocsetorder(int order);
void sEditorautocgetorder();
void sEditorautocsetautohide(bool autohide);
void sEditorautocgetautohide();
void sEditorautocsetdroprestofword(bool droprestofword);
void sEditorautocgetdroprestofword();
void sEditorregisterimage(int type, const char *xpmdata);
void sEditorregisterrgbaimage(int type, const char *pixels);
void sEditorclearregisteredimages();
void sEditorautocsettypeseparator(char separatorcharacter);
void sEditorautocgettypeseparator();
void sEditorautocsetmaxheight(int rowcount);
void sEditorautocgetmaxheight();
void sEditorautocsetmaxwidth(int charactercount);
void sEditorautocgetmaxwidth();
void sEditorUserListShow(int listType, const char* list);
void sEditorcalltipshow(int posstart, const char *definition);
void sEditorcalltipcancel();
void sEditorcalltipactive();
void sEditorcalltipposstart();
void sEditorcalltipsetposstart(int posstart);
void sEditorcalltipsethlt(int highlightstart, int highlightend);
void sEditorcalltipsetback(int colour);
void sEditorcalltipsetfore(int colour);
void sEditorcalltipsetforehlt(int colour);
void sEditorcalltipusestyle(int tabsize);
void sEditorcalltipsetposition(bool above);
void sEditorAssignCmdKey(int keyDefinition, int sciCommand);
void sEditorClearCmdKey(int keyDefinition);
void sEditorClearAllCmdKeys();
void sEditorNull();
void sEditorUsePopup(bool bEnablePopup);
void sEditorStartRecord();
void sEditorStopRecord();
void sEditorformatrange(void);
void sEditorsetprintmagnification(int magnification);
void sEditorgetprintmagnification();
void sEditorsetprintcolourmode(int mode);
void sEditorgetprintcolourmode();
void sEditorsetprintwrapmode();
void sEditorgetprintwrapmode();
void sEditorgetdirectfunction();
void sEditorgetdirectpointer();
void sEditorgetcharacterpointer();
void sEditorgetrangepointer(int position, int rangelength);
void sEditorgetgapposition();
void sEditorGetDocPointer();
void sEditorSetDocPointer(void);
void sEditorCreateDocument();
void sEditorAddRefDocument(void);
void sEditorReleaseDocument(void);
void sEditorvisiblefromdocline(int docline);
void sEditordoclinefromvisible(int displayline);
void sEditorshowlines(int linestart, int lineend);
void sEditorhidelines(int linestart, int lineend);
void sEditorgetlinevisible(int line);
void sEditorgetalllinesvisible();
void sEditorsetfoldlevel(int line, int level);
void sEditorgetfoldlevel(int line);
void sEditorsetautomaticfold(int automaticfold);
void sEditorgetautomaticfold();
void sEditorsetfoldflags(int flags);
void sEditorgetlastchild(int line, int level);
void sEditorgetfoldparent(int line);
void sEditorsetfoldexpanded(int line, bool expanded);
void sEditorgetfoldexpanded(int line);
void sEditorcontractedfoldnext(int linestart);
void sEditortogglefold(int line);
void sEditorfoldline(int line, int action);
void sEditorfoldchildren(int line, int action);
void sEditorfoldall(int action);
void sEditorexpandchildren(int line, int level);
void sEditorensurevisible(int line);
void sEditorensurevisibleenforcepolicy(int line);
static void sEditorSetWrapMode(int wrapMode)
{
    sEditorCall(SCI_SETWRAPMODE, wrapMode);
}
static int sEditorGetWrapMode()
{
    return (int)sEditorCall(SCI_GETWRAPMODE);
}
void sEditorSetWrapVisualFlags(int wrapVisualFlags);
void sEditorGetWrapVisualFlags();
void sEditorSetWrapVisualFlagsLocation(int wrapVisualFlagsLocation);
void sEditorGetWrapVisualFlagsLocation();
void sEditorSetWrapIndentMode(int indentMode);
void sEditorGetWrapIndentMode();

void sEditorSetWrapStartIndent(int indent);
void sEditorGetWrapStartIndent();
void sEditorSetLayoutCache(int cacheMode);
void sEditorGetLayoutCache();
void sEditorSetPositionCache(int size);
void sEditorGetPositionCache();
void sEditorLinesSplit(int pixelWidth);
void sEditorLinesJoin();
void sEditorWrapCount(int docLine);
void sEditorZoomIn();
void sEditorZoomOut();
void sEditorSetZoom(int zoomInPoints);
void sEditorGetZoom();
void sEditorSetEdgeMode(int mode);
void sEditorGetEdgeMode();
static void sEditorSetEdgeColumn(int column)
{
    sEditorCall(SCI_SETEDGECOLUMN, (uptr_t)column);
}
static int sEditorGetEdgeColumn()
{
    return (int)sEditorCall(SCI_GETEDGECOLUMN);
}
static void sEditorSetEdgeColour(int colour)
{
    sEditorCall(SCI_SETEDGECOLOUR, (uptr_t)colour);
}
void sEditorGetEdgeColour();
void sEditorSetLexer(int lexer)
{
    sEditorCall(SCI_SETLEXER, lexer);
}
void sEditorGetLexer();
void sEditorSetLexerLanguage(const char *name);
void sEditorGetLexerLanguage(char *name);
void sEditorLoadLexerLibrary(const char *path);
void sEditorColourise(int start, int end);
void sEditorChangeLexerState(int start, int end);
void sEditorPropertyNames(char *names);
void sEditorPropertyType(const char *name);
void sEditorDescribeProperty(const char *name, char *description);
void sEditorSetProperty(const char *key, const char *value);
void sEditorGetProperty(const char *key, char *value);
void sEditorGetPropertyExpanded(const char *key, char *value);
void sEditorGetPropertyInt(const char *key, int default);
void sEditorDescribeKeywordSets(char *descriptions);
void sEditorSetKeywords(int keywordSet, const char *keywordList)
{
    sEditorCall(SCI_SETKEYWORDS, keywordSet, (sptr_t)keywordList);
}
void sEditorGetSubStyleBases(char *styles);
void sEditorDistanceToSecondaryStyles();
void sEditorAllocateSubStyles(int styleBase, int numberStyles);
void sEditorFreeSubStyles();
void sEditorGetSubStylesStart(int styleBase);
void sEditorGetSubStylesLength(int stylebase);
void sEditorGetStyleFromSubStyle(int subStyle);
void sEditorGetPrimaryStyleFromStyle(int style);
void sEditorSetIdentifiers(int style, const char *identifiers);
