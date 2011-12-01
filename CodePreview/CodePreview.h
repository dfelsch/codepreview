/****************************** Module Header ******************************\
Module Name:  CodePreview.h
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

/********************* Original Header from code sample ********************\
Module Name:  RecipePreviewHandler.h
Project:      CppShellExtPreviewHandler
Copyright (c) Microsoft Corporation.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#include <windows.h>
#include <shlobj.h>			// IPreviewHandler, IShellItem, IInitializeWithItem, IParentAndItem
#include <thumbcache.h>
#include <wincodec.h>       // Windows Imaging Codecs

#pragma comment(lib, "windowscodecs.lib")

const COLORREF black = RGB(0,0,0);
const COLORREF white = RGB(0xff,0xff,0xff);

class CCodePreview : 
	public IInitializeWithStream, 
	public IPreviewHandler, 
	public IPreviewHandlerVisuals, 
	public IOleWindow, 
	public IObjectWithSite
{
public:
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	// IInitializeWithStream
	IFACEMETHODIMP Initialize(IStream *pStream, DWORD grfMode);

	// IPreviewHandler
	IFACEMETHODIMP SetWindow(HWND hwnd, const RECT *prc);
	IFACEMETHODIMP SetFocus();
	IFACEMETHODIMP QueryFocus(HWND *phwnd);
	IFACEMETHODIMP TranslateAccelerator(MSG *pmsg);
	IFACEMETHODIMP SetRect(const RECT *prc);
	IFACEMETHODIMP DoPreview();
	IFACEMETHODIMP Unload();

	// IPreviewHandlerVisuals (Optional)
	IFACEMETHODIMP SetBackgroundColor(COLORREF color);
	IFACEMETHODIMP SetFont(const LOGFONTW *plf);
	IFACEMETHODIMP SetTextColor(COLORREF color);

	// IOleWindow
	IFACEMETHODIMP GetWindow(HWND *phwnd);
	IFACEMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

	// IObjectWithSite
	IFACEMETHODIMP SetSite(IUnknown *punkSite);
	IFACEMETHODIMP GetSite(REFIID riid, void **ppv);

	CCodePreview();

protected:
	~CCodePreview();

private:
	// Reference count of component.
	long m_cRef;

	// Provided during initialization.
	IStream *m_pStream;

	// Parent window that hosts the previewer window.
	// Note: do NOT DestroyWindow this.
	HWND m_hwndParent;

	// Bounding rect of the parent window.
	RECT m_rcParent;

	// The actual previewer window.
	HWND hwndScintilla;

	// Site pointer from host, used to get IPreviewHandlerFrame.
	IUnknown *m_punkSite;

	// Text of the Scintilla control
	char * text;

	/*** METHODS ***/

	//Sends a message to the Scintilla-Control
	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0);

	//Sets a style in the Scintilla-Control
	void SetAStyle(int style, COLORREF fore, COLORREF back=white, int size=-1, const char *face=0);

	//Sets all settings that have to be set before text is inserted
	void InitialiseEditor();

	//Sets some basic settings of the Scintilla-Control after text is inserted
	void StyleEditor();

	//Sets the text of the Scintilla-Control
	void SetText();

	//Sets the syntax highlighting etc.
	void SetCodeLayout();
};