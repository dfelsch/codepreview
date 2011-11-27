/****************************** Module Header ******************************\
Module Name:  Perl.cpp
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

#include "StdAfx.h"

#ifdef LANG_PERL

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
const COLORREF lightBlue  = RGB(0x2B, 0x91, 0xAF);

const char perlKeyWords[] = 
	"NULL __FILE__ __LINE__ __PACKAGE__ __DATA__ __END__ AUTOLOAD "
	"BEGIN CORE DESTROY END EQ GE GT INIT LE LT NE CHECK abs accept "
	"alarm and atan2 bind binmode bless caller chdir chmod chomp chop "
	"chown chr chroot close closedir cmp connect continue cos crypt "
	"dbmclose dbmopen defined delete die do dump each else elsif endgrent "
	"endhostent endnetent endprotoent endpwent endservent eof eq eval "
	"exec exists exit exp fcntl fileno flock for foreach fork format "
	"formline ge getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname "
	"gethostent getlogin getnetbyaddr getnetbyname getnetent getpeername "
	"getpgrp getppid getpriority getprotobyname getprotobynumber getprotoent "
	"getpwent getpwnam getpwuid getservbyname getservbyport getservent "
	"getsockname getsockopt glob gmtime goto grep gt hex if index "
	"int ioctl join keys kill last lc lcfirst le length link listen "
	"local localtime lock log lstat lt map mkdir msgctl msgget msgrcv "
	"msgsnd my ne next no not oct open opendir or ord our pack package "
	"pipe pop pos print printf prototype push quotemeta qu "
	"rand read readdir readline readlink readpipe recv redo "
	"ref rename require reset return reverse rewinddir rindex rmdir "
	"scalar seek seekdir select semctl semget semop send setgrent "
	"sethostent setnetent setpgrp setpriority setprotoent setpwent "
	"setservent setsockopt shift shmctl shmget shmread shmwrite shutdown "
	"sin sleep socket socketpair sort splice split sprintf sqrt srand "
	"stat study sub substr symlink syscall sysopen sysread sysseek "
	"system syswrite tell telldir tie tied time times truncate "
	"uc ucfirst umask undef unless unlink unpack unshift untie until "
	"use utime values vec wait waitpid wantarray warn while write "
	"xor given when default break say state UNITCHECK";

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
void SetPerlLayout(HWND editor)
{
	Editor = editor;
	SendEditor(SCI_SETLEXER, SCLEX_PERL);
	SendEditor(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(perlKeyWords));
	SetAStyle(SCE_PL_ERROR, red);
	SetAStyle(SCE_PL_COMMENTLINE, darkGreen);
	SetAStyle(SCE_PL_WORD, blue);
	SetAStyle(SCE_PL_STRING, darkRed);
	SetAStyle(SCE_PL_CHARACTER, darkRed);
	SetAStyle(SCE_PL_PREPROCESSOR, blue);
	SetAStyle(SCE_PL_REGEX, lightBlue);
	SetAStyle(SCE_PL_REGSUBST, lightBlue);
}

#endif