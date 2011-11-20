// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// Include Scintilla parser
#include "Scintilla.h"
#include "SciLexer.h"

//Set the language for which the DLL is compiled
#define LANG_CPP

#ifdef LANG_PYTHON
#define HANDLER_NAME L"Python Preview"
#endif
#ifdef LANG_CPP
#define HANDLER_NAME L"C++ Preview"
#endif
