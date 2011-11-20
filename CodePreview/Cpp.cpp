/****************************** Module Header ******************************\
Module Name:  Cpp.cpp
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

#pragma once
#include <windows.h>
#include "Scintilla.h"
#include "SciLexer.h"

const COLORREF black      = RGB(0,0,0);
const COLORREF white      = RGB(0xff,0xff,0xff);
const COLORREF grey       = RGB(0x80,0x80,0x80);
const COLORREF red        = RGB(0xFF,0,0);
const COLORREF green      = RGB(0,0xFF,0);
const COLORREF blue       = RGB(0,0,0xFF);
const COLORREF darkRed    = RGB(0x80,0,0);
const COLORREF darkGreen  = RGB(0, 0x80, 0);
const COLORREF darkBlue   = RGB(0, 0, 0x80);

const char cppKeyWords[] = 
	"and and_eq asm auto bitand bitor bool break "
	"case catch char class compl const const_cast continue "
	"default delete do double dynamic_cast else enum explicit export extern false float for "
	"friend goto if inline int long mutable namespace new not not_eq "
	"operator or or_eq private protected public "
	"register reinterpret_cast return short signed sizeof static static_cast struct switch "
	"template this throw true try typedef typeid typename union unsigned using "
	"virtual void volatile wchar_t while xor xor_eq";

const char doxygenKeyWords[] = 
	"a addindex addtogroup anchor arg attention "
	"author b brief bug c class code date def defgroup deprecated dontinclude "
	"e em endcode endhtmlonly endif endlatexonly endlink endverbatim enum example exception "
	"f$ f[ f] file fn hideinitializer htmlinclude htmlonly "
	"if image include ingroup internal invariant interface latexonly li line link "
	"mainpage name namespace nosubgrouping note overload "
	"p page par param param[in] param[out] "
	"post pre ref relates remarks return retval "
	"sa section see showinitializer since skip skipline struct subsection "
	"test throw throws todo typedef union until "
	"var verbatim verbinclude version warning weakgroup $ @ \\ & < > # { }";

HWND Editor;

/*** HELPER METHODS ***/
LRESULT SendEditor(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) 
{
	return ::SendMessage(Editor, Msg, wParam, lParam);
}

void SetAStyle(int style, COLORREF fore, COLORREF back=white, int size=-1, const char *face=0)
{
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

/*** HERE THE REAL WORK IS DONE ***/
void SetCppLayout(HWND editor)
{
	Editor = editor;
	SendEditor(SCI_SETLEXER, SCLEX_CPP);
	SendEditor(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(cppKeyWords));
	SendEditor(SCI_SETKEYWORDS, 2, reinterpret_cast<LPARAM>(doxygenKeyWords));
	SetAStyle(SCE_C_COMMENT, darkGreen);
	SetAStyle(SCE_C_COMMENTLINE, darkGreen);
	SetAStyle(SCE_C_COMMENTDOC, darkGreen);
	SetAStyle(SCE_C_WORD, blue);
	SetAStyle(SCE_C_STRING, darkRed);
	SetAStyle(SCE_C_CHARACTER, darkRed);
	SetAStyle(SCE_C_PREPROCESSOR, blue);
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, blue); 
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, red);
}