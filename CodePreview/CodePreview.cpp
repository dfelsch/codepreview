/****************************** Module Header ******************************\
Module Name:  CodePreview.cpp
Project:      CodePreview
Copyright (c) Microsoft Corporation.



This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "CodePreview.h"
#include <Shlwapi.h>
#include <Wincrypt.h>   // For CryptStringToBinary.
#include <msxml6.h>
#include <WindowsX.h>
#include <assert.h>
#include "StdAfx.h"

#include "Languages.h"

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "msxml6.lib")

extern HINSTANCE   g_hInst;

const int MARGIN_FOLD_INDEX = 1;

inline int RECTWIDTH(const RECT &rc)
{
	return (rc.right - rc.left);
}

inline int RECTHEIGHT(const RECT &rc )
{
	return (rc.bottom - rc.top);
}

//Constructor
CCodePreview::CCodePreview() : m_cRef(1), m_pStream(NULL), 
	m_hwndParent(NULL), hwndScintilla(NULL), m_punkSite(NULL)
{
}

LRESULT CCodePreview::SendEditor(UINT Msg, WPARAM wParam, LPARAM lParam) 
{
	return ::SendMessage(hwndScintilla, Msg, wParam, lParam);
}

void CCodePreview::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face) {
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

void CCodePreview::InitialiseEditor()
{
	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(STYLE_DEFAULT, black, white, 10, "Courier New");
	SendEditor(SCI_STYLECLEARALL);	// Copies global style to all others
}

void CCodePreview::SetText()
{
	STATSTG stats;
	ULONG pcbRead;
	m_pStream->Stat(&stats, STATFLAG_NONAME);
	text = new char [stats.cbSize.LowPart + 1]; //should be enough, +1 for \0

	m_pStream->Read(text, stats.cbSize.LowPart, &pcbRead);
	text[stats.cbSize.LowPart] = '\0'; //terminate text
	SendEditor(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
}

void CCodePreview::StyleEditor()
{
	SendEditor(SCI_SETSTYLEBITS, 7);

	SendEditor(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold"), reinterpret_cast<LPARAM>("1"));

	//Set margin to show folding-symbols
	SendEditor(SCI_SETMARGINTYPEN,  MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL);
	SendEditor(SCI_SETMARGINMASKN, MARGIN_FOLD_INDEX, SC_MASK_FOLDERS);
	SendEditor(SCI_SETMARGINWIDTHN, MARGIN_FOLD_INDEX, 16);
	SendEditor(SCI_SETMARGINSENSITIVEN, MARGIN_FOLD_INDEX, TRUE);

	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL,  SC_MARK_LCORNER);

	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDER, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEREND, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPEN, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDEROPENMID, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, black);
	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL,  black);

	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDER, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEREND, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPEN, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDEROPENMID, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERMIDTAIL, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERSUB, ::GetSysColor(COLOR_3DFACE));
	SendEditor(SCI_MARKERSETFORE, SC_MARKNUM_FOLDERTAIL,  ::GetSysColor(COLOR_3DFACE));

	SendEditor(SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEAFTER_CONTRACTED, 0);

	int lineCount = SendEditor(SCI_GETLINECOUNT); //Get the number of lines
	char lineCountBuffer[12] = "_"; //A 32-bit-number can be at most 10 digits long, +1 for '\0', +1 for '_'
	_itoa_s(lineCount, lineCountBuffer + 1, 11, 10); //Convert to string, leave the "_" at beginning
	SendEditor(SCI_SETMARGINWIDTHN, 0, SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, reinterpret_cast<LPARAM>(lineCountBuffer))); //Adjust line-number-margin-width 
}

void CCodePreview::SetCodeLayout()
{
	#ifdef LANG_PYTHON
	SetPythonLayout(hwndScintilla);
	#endif
	#ifdef LANG_CPP
	SetCppLayout(hwndScintilla);
	#endif
}

//Destructor
CCodePreview::~CCodePreview()
{
	if (hwndScintilla)
	{
		DestroyWindow(hwndScintilla);
		hwndScintilla = NULL;
	}
	if (m_punkSite)
	{
		m_punkSite->Release();
		m_punkSite = NULL;
	}
	if (m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}
}


#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP CCodePreview::QueryInterface(REFIID riid, void **ppv)
{
	HRESULT hr = S_OK;

	if (IsEqualIID(IID_IUnknown, riid) || 
		IsEqualIID(IID_IPreviewHandler, riid))
	{
		*ppv = static_cast<IPreviewHandler *>(this);
	}
	else if (IsEqualIID(IID_IInitializeWithStream, riid))
	{
		*ppv = static_cast<IInitializeWithStream *>(this);
	}
	else if (IsEqualIID(IID_IPreviewHandlerVisuals, riid))
	{
		*ppv = static_cast<IPreviewHandlerVisuals *>(this);
	}
	else if (IsEqualIID(IID_IOleWindow, riid))
	{
		*ppv = static_cast<IOleWindow *>(this);
	}
	else if (IsEqualIID(IID_IObjectWithSite, riid))
	{
		*ppv = static_cast<IObjectWithSite *>(this);
	}
	else
	{
		hr = E_NOINTERFACE;
		*ppv = NULL;
	}

	if (*ppv)
	{
		AddRef();
	}

	return hr;
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) CCodePreview::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) CCodePreview::Release()
{
	ULONG cRef = InterlockedDecrement(&m_cRef);
	if (0 == cRef)
	{
		delete this;
	}

	return cRef;
}

#pragma endregion


#pragma region IInitializeWithStream

// Initializes the preview handler with a stream. 
// Store the IStream and mode parameters so that you can read the item's data 
// when you are ready to preview the item. Do not load the data in Initialize. 
// Load the data in IPreviewHandler::DoPreview just before you render.
IFACEMETHODIMP CCodePreview::Initialize(IStream *pStream, DWORD grfMode)
{
	HRESULT hr = E_INVALIDARG;
	if (pStream)
	{
		// Initialize can be called more than once, so release existing valid 
		// m_pStream.
		if (m_pStream)
		{
			m_pStream->Release();
			m_pStream = NULL;
		}

		m_pStream = pStream;
		m_pStream->AddRef();
		hr = S_OK;
	}
	return hr;
}

#pragma endregion


#pragma region IPreviewHandler

// This method gets called when the previewer gets created. It sets the parent 
// window of the previewer window, as well as the area within the parent to be 
// used for the previewer window.
IFACEMETHODIMP CCodePreview::SetWindow(HWND hwnd, const RECT *prc)
{
	if (hwnd && prc)
	{
		m_hwndParent = hwnd;  // Cache the HWND for later use
		m_rcParent = *prc;    // Cache the RECT for later use

		if (hwndScintilla)
		{
			// Update preview window parent and rect information
			SetParent(hwndScintilla, m_hwndParent);
			SetWindowPos(hwndScintilla, NULL, m_rcParent.left, m_rcParent.top, 
				RECTWIDTH(m_rcParent), RECTHEIGHT(m_rcParent), 
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	return S_OK;
}

// Directs the preview handler to set focus to itself.
IFACEMETHODIMP CCodePreview::SetFocus()
{
	HRESULT hr = S_FALSE;
	if (hwndScintilla)
	{
		::SetFocus(hwndScintilla);
		hr = S_OK;
	}
	return hr;
}

// Directs the preview handler to return the HWND from calling the GetFocus 
// function.
IFACEMETHODIMP CCodePreview::QueryFocus(HWND *phwnd)
{
	HRESULT hr = E_INVALIDARG;
	if (phwnd != NULL)
	{
		*phwnd = ::GetFocus();
		if (*phwnd != NULL)
		{
			hr = S_OK;
		}
		else
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
	}
	
	return hr;
}

// Directs the preview handler to handle a keystroke passed up from the 
// message pump of the process in which the preview handler is running.
HRESULT CCodePreview::TranslateAccelerator(MSG *pmsg)
{
	HRESULT hr = S_FALSE;
	IPreviewHandlerFrame *pFrame = NULL;
	if (m_punkSite && SUCCEEDED(m_punkSite->QueryInterface(&pFrame)))
	{
		// If your previewer has multiple tab stops, you will need to do 
		// appropriate first/last child checking. This sample previewer has 
		// no tabstops, so we want to just forward this message out.
		hr = pFrame->TranslateAccelerator(pmsg);

		pFrame->Release();
	}

	return hr;
}

// This method gets called when the size of the previewer window changes 
// (user resizes the Reading Pane). It directs the preview handler to change 
// the area within the parent hwnd that it draws into.
IFACEMETHODIMP CCodePreview::SetRect(const RECT *prc)
{
	HRESULT hr = E_INVALIDARG;
	if (prc != NULL)
	{
		m_rcParent = *prc;
		if (hwndScintilla)
		{
			// Preview window is already created, so set its size and position.
			SetWindowPos(hwndScintilla, NULL, m_rcParent.left, m_rcParent.top,
				(m_rcParent.right - m_rcParent.left), // Width
				(m_rcParent.bottom - m_rcParent.top), // Height
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
		hr = S_OK;
	}
	return hr;
}

// The method directs the preview handler to load data from the source 
// specified in an earlier Initialize method call, and to begin rendering to 
// the previewer window.
IFACEMETHODIMP CCodePreview::DoPreview()
{
	HRESULT hr = E_FAIL;

	// Cannot call more than once. (Unload should be called before another DoPreview)
	if (hwndScintilla != NULL || !m_pStream) 
		return E_FAIL;

	//Load Scintilla
	if (!Scintilla_RegisterClasses(g_hInst))
		return E_FAIL;

	//
	// Create the Scintilla window
	// 
	hwndScintilla = CreateWindow(
					  L"Scintilla",
					  L"Editor",
					  WS_CHILD | WS_VSCROLL | WS_VISIBLE,
					  m_rcParent.left, 
					  m_rcParent.top, 
					  RECTWIDTH(m_rcParent), 
					  RECTHEIGHT(m_rcParent),
					  m_hwndParent,
					  0,
					  g_hInst,
					  0);

	if (hwndScintilla == NULL)
		return hr = HRESULT_FROM_WIN32(GetLastError());

	InitialiseEditor();

	//Read stream and put it into the control
	SetText();

	//Sets the syntax highlighting etc.
	SetCodeLayout();

	//Sets some basic settings of the Scintilla-Control
	StyleEditor();

	//Make control read-only (has to be done after setting the text)
	SendEditor(SCI_SETREADONLY, TRUE);

	//make control visible
	ShowWindow(hwndScintilla, SW_SHOW);

	return hr = S_OK;
}

// This method gets called when a shell item is de-selected. It directs the 
// preview handler to cease rendering a preview and to release all resources 
// that have been allocated based on the item passed in during the 
// initialization.
HRESULT CCodePreview::Unload()
{
	if (m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

	if (hwndScintilla)
	{
		DestroyWindow(hwndScintilla);
		hwndScintilla = NULL;
	}

	Scintilla_ReleaseResources();

	if (text)
	{
		delete[] text;
		text = NULL;
	}

	return S_OK;
}

#pragma endregion


#pragma region IPreviewHandlerVisuals (Optional)

// Sets the background color of the preview handler.
IFACEMETHODIMP CCodePreview::SetBackgroundColor(COLORREF color)
{
	return S_OK;
}

// Sets the font attributes to be used for text within the preview handler.
IFACEMETHODIMP CCodePreview::SetFont(const LOGFONTW *plf)
{
	return S_OK;
}

// Sets the color of the text within the preview handler.
IFACEMETHODIMP CCodePreview::SetTextColor(COLORREF color)
{
	return S_OK;
}

#pragma endregion


#pragma region IOleWindow

// Retrieves a handle to one of the windows participating in in-place 
// activation (frame, document, parent, or in-place object window).
IFACEMETHODIMP CCodePreview::GetWindow(HWND *phwnd)
{
	HRESULT hr = E_INVALIDARG;
	if (phwnd)
	{
		*phwnd = m_hwndParent;
		hr = S_OK;
	}
	return hr;
}

// Determines whether context-sensitive help mode should be entered during an 
// in-place activation session
IFACEMETHODIMP CCodePreview::ContextSensitiveHelp(BOOL fEnterMode)
{
	return E_NOTIMPL;
}

#pragma endregion


#pragma region IObjectWithSite

// Provides the site's IUnknown pointer to the object.
IFACEMETHODIMP CCodePreview::SetSite(IUnknown *punkSite)
{
	if (m_punkSite)
	{
		m_punkSite->Release();
		m_punkSite = NULL;
	}
	return punkSite ? punkSite->QueryInterface(&m_punkSite) : S_OK;
}

// Gets the last site set with IObjectWithSite::SetSite. If there is no known 
// site, the object returns a failure code.
IFACEMETHODIMP CCodePreview::GetSite(REFIID riid, void **ppv)
{
	*ppv = NULL;
	return m_punkSite ? m_punkSite->QueryInterface(riid, ppv) : E_FAIL;
}

#pragma endregion