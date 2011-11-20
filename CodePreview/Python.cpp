/****************************** Module Header ******************************\
Module Name:  Python.cpp
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

#pragma once
#include <windows.h>
#include "Scintilla.h"
#include "SciLexer.h"

const COLORREF black     = RGB(0,0,0);
const COLORREF white     = RGB(0xff,0xff,0xff);
const COLORREF grey      = RGB(0x80,0x80,0x80);
const COLORREF red       = RGB(0xFF,0,0);
const COLORREF green     = RGB(0,0xFF,0);
const COLORREF blue      = RGB(0,0,0xFF);
const COLORREF darkRed   = RGB(0x80,0,0);
const COLORREF darkGreen = RGB(0, 0x80, 0);
const COLORREF darkBlue  = RGB(0, 0, 0x80);

const char pythonKeyWords[] = 
	"None True and assert break class continue def del elif else except exec "
	"finally for from global if import in is lambda not or pass " 
	"print raise return try while yield";

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
void SetPythonLayout(HWND editor)
{
	Editor = editor;
	SendEditor(SCI_SETLEXER, SCLEX_PYTHON);
	SendEditor(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(pythonKeyWords));
	SetAStyle(SCE_P_COMMENTLINE, darkGreen);
	SetAStyle(SCE_P_STRING, darkRed);
	SetAStyle(SCE_P_CHARACTER, darkRed);
	SetAStyle(SCE_P_WORD, blue);
	SetAStyle(SCE_P_TRIPLE, darkGreen);
	SetAStyle(SCE_P_TRIPLEDOUBLE, darkGreen);
	SetAStyle(SCE_P_COMMENTBLOCK, darkGreen);
}