/****************************** Module Header ******************************\
Module Name:  CSharp.cpp
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

#include "StdAfx.h"

#ifdef LANG_CSHARP

#pragma once
#include <windows.h>

const COLORREF black      = RGB(0,0,0);
const COLORREF white      = RGB(0xff,0xff,0xff);
const COLORREF grey       = RGB(0x80,0x80,0x80);
const COLORREF red        = RGB(0xFF,0,0);
const COLORREF green      = RGB(0,0xFF,0);
const COLORREF blue       = RGB(0,0,0xFF);
const COLORREF darkRed    = RGB(0x80,0,0);
const COLORREF darkGreen  = RGB(0, 0x80, 0);
const COLORREF darkBlue   = RGB(0, 0, 0x80);

const char cSharpKeyWords[] = 
	"abstract as ascending base bool break by byte case catch char checked "
	"class const continue decimal default delegate descending do double else enum "
	"equals event explicit extern false finally fixed float for foreach from goto group if "
	"implicit in int interface internal into is join lock let long namespace new null "
	"object on operator orderby out override params private protected public "
	"readonly ref return sbyte sealed select short sizeof stackalloc static "
	"string struct switch this throw true try typeof uint ulong "
	"unchecked unsafe ushort using var virtual void volatile where while get set";

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
void SetCSharpLayout(HWND editor)
{
	Editor = editor;
	SendEditor(SCI_SETLEXER, SCLEX_CPP);
	SendEditor(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(cSharpKeyWords));
	SetAStyle(SCE_C_COMMENT, darkGreen);
	SetAStyle(SCE_C_COMMENTLINE, darkGreen);
	SetAStyle(SCE_C_COMMENTDOC, darkGreen);
	SetAStyle(SCE_C_COMMENTLINEDOC, darkGreen);
	SetAStyle(SCE_C_WORD, blue);
	SetAStyle(SCE_C_STRING, darkRed);
	SetAStyle(SCE_C_CHARACTER, darkRed);
	SetAStyle(SCE_C_PREPROCESSOR, blue);
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, blue); 
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, red);
}

#endif