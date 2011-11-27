/****************************** Module Header ******************************\
Module Name:  Languages.h
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

#pragma once
#include <windows.h>

#ifdef LANG_PYTHON
void SetPythonLayout(HWND);
#endif
#ifdef LANG_CPP
void SetCppLayout(HWND);
#endif
#ifdef LANG_JAVA
void SetJavaLayout(HWND);
#endif
#ifdef LANG_CSHARP
void SetCSharpLayout(HWND);
#endif
#ifdef LANG_PERL
void SetPerlLayout(HWND);
#endif