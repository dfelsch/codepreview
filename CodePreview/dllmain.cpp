/****************************** Module Header ******************************\
Module Name:  dllmain.cpp
Project:      CodePreview
Copyright (c) Dennis Felsch
\***************************************************************************/

/********************* Original Header from code sample ********************\
Module Name:  dllmain.cpp
Project:      CppShellExtContextMenuHandler
Copyright (c) Microsoft Corporation.

The file implements DllMain, and the DllGetClassObject, DllCanUnloadNow, 
DllRegisterServer, DllUnregisterServer functions that are necessary for a COM 
DLL. 

DllGetClassObject invokes the class factory defined in ClassFactory.h/cpp and 
queries to the specific interface.

DllCanUnloadNow checks if we can unload the component from the memory.

DllRegisterServer registers the COM server and the preview handler in the 
registry by invoking the helper functions defined in Reg.h/cpp. The preview 
handler is associated with the .recipe file class.

DllUnregisterServer unregisters the COM server and the preview handler. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include <windows.h>
#include <Guiddef.h>
#include "ClassFactory.h"           // For the class factory
#include "Reg.h"
#include "StdAfx.h"

//Actual language is in "StdAfx.h"
//Every language needs its own extensions, name and IDs
#ifdef LANG_PYTHON
	#define SUPPORTED_FILETYPES L".py;.pyw"
	// {DE7F719C-5E18-41D6-82E7-F2B201DC3F39}
	const CLSID CLSID_CodePreview = 
	{ 0xde7f719c, 0x5e18, 0x41d6, { 0x82, 0xe7, 0xf2, 0xb2, 0x1, 0xdc, 0x3f, 0x39 } };
	// {1A92DA73-0761-48A5-9A30-076A80B64E1F}
	const GUID APPID_CodePreview = 
	{ 0x1a92da73, 0x761, 0x48a5, { 0x9a, 0x30, 0x7, 0x6a, 0x80, 0xb6, 0x4e, 0x1f } };
#endif
#ifdef LANG_CPP
	#define SUPPORTED_FILETYPES L".c;.cc;.cpp;.cxx;.h;.hh;.hpp;.hxx"
	// {77B65FCC-A2A3-4279-B742-B4B0ABB4421B}
	const CLSID CLSID_CodePreview = 
	{ 0x77b65fcc, 0xa2a3, 0x4279, { 0xb7, 0x42, 0xb4, 0xb0, 0xab, 0xb4, 0x42, 0x1b } };
	// {1675B27D-FBC3-4DD4-917F-3F78709A2E82}
	const GUID APPID_CodePreview = 
	{ 0x1675b27d, 0xfbc3, 0x4dd4, { 0x91, 0x7f, 0x3f, 0x78, 0x70, 0x9a, 0x2e, 0x82 } };
#endif
#ifdef LANG_JAVA
	#define SUPPORTED_FILETYPES L".java"
	// {C7651AD8-688D-4CEC-B73D-4487A30970B6}
	const CLSID CLSID_CodePreview = 
	{ 0xc7651ad8, 0x688d, 0x4cec, { 0xb7, 0x3d, 0x44, 0x87, 0xa3, 0x9, 0x70, 0xb6 } };
	// {03565EB3-860D-49AE-A2B5-2531D2621A30}
	const GUID APPID_CodePreview = 
	{ 0x3565eb3, 0x860d, 0x49ae, { 0xa2, 0xb5, 0x25, 0x31, 0xd2, 0x62, 0x1a, 0x30 } };
#endif
#ifdef LANG_CSHARP
	#define SUPPORTED_FILETYPES L".cs"
	// {5E3E85CC-A0DA-4CB6-9C13-E431BEFA7100}
	const CLSID CLSID_CodePreview = 
	{ 0x5e3e85cc, 0xa0da, 0x4cb6, { 0x9c, 0x13, 0xe4, 0x31, 0xbe, 0xfa, 0x71, 0x0 } };
	// {37146319-815A-4BE6-95FB-A3ADF03FB1C3}
	const GUID APPID_CodePreview = 
	{ 0x37146319, 0x815a, 0x4be6, { 0x95, 0xfb, 0xa3, 0xad, 0xf0, 0x3f, 0xb1, 0xc3 } };
#endif
#ifdef LANG_PERL
	#define SUPPORTED_FILETYPES L".pl;.pm"
	// {4B7345C6-4C44-413D-BE1E-3FCE9427F409}
	const CLSID CLSID_CodePreview = 
	{ 0x4b7345c6, 0x4c44, 0x413d, { 0xbe, 0x1e, 0x3f, 0xce, 0x94, 0x27, 0xf4, 0x9 } };
	// {7F115E3C-830F-42AA-B6C1-F4FFAB51FAEE}
	const GUID APPID_CodePreview = 
	{ 0x7f115e3c, 0x830f, 0x42aa, { 0xb6, 0xc1, 0xf4, 0xff, 0xab, 0x51, 0xfa, 0xee } };
#endif

HINSTANCE   g_hInst     = NULL;
long        g_cDllRef   = 0;


BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Hold the instance of this DLL module, we will use it to get the 
		// path of the DLL to register the component.
		g_hInst = hModule;
		DisableThreadLibraryCalls(hModule);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

HRESULT DllInstall(BOOL bInstall, __in_opt  PCWSTR pszCmdLine)
{
	//Just to prevent that dumb message saying that the entry point wasn't found...
	return S_OK;
}


//
//   FUNCTION: DllGetClassObject
//
//   PURPOSE: Create the class factory and query to the specific interface.
//
//   PARAMETERS:
//   * rclsid - The CLSID that will associate the correct data and code.
//   * riid - A reference to the identifier of the interface that the caller 
//     is to use to communicate with the class object.
//   * ppv - The address of a pointer variable that receives the interface 
//     pointer requested in riid. Upon successful return, *ppv contains the 
//     requested interface pointer. If an error occurs, the interface pointer 
//     is NULL. 
//
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	if (IsEqualCLSID(CLSID_CodePreview, rclsid))
	{
		hr = E_OUTOFMEMORY;

		ClassFactory *pClassFactory = new ClassFactory();
		if (pClassFactory)
		{
			hr = pClassFactory->QueryInterface(riid, ppv);
			pClassFactory->Release();
		}
	}

	return hr;
}


//
//   FUNCTION: DllCanUnloadNow
//
//   PURPOSE: Check if we can unload the component from the memory.
//
//   NOTE: The component can be unloaded from the memory when its reference 
//   count is zero (i.e. nobody is still using the component).
// 
STDAPI DllCanUnloadNow(void)
{
	return g_cDllRef > 0 ? S_FALSE : S_OK;
}


//
//   FUNCTION: DllRegisterServer
//
//   PURPOSE: Register the COM server and the context menu handler.
// 
STDAPI DllRegisterServer(void)
{
	HRESULT hr;

	wchar_t szModule[MAX_PATH];
	if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	// Register the component.
	hr = RegisterInprocServer(szModule, CLSID_CodePreview, 
		HANDLER_NAME L".CodePreview Class", 
		L"Apartment", APPID_CodePreview, HANDLER_NAME);
	if (SUCCEEDED(hr))
	{
		//Split supported filetypes and register each of them
		static wchar_t filetypes[] = SUPPORTED_FILETYPES;
		wchar_t *token, *next_token;
		token = wcstok_s(filetypes, L";", &next_token);
		while(token != NULL)
		{
			// Register the preview handler
			hr = RegisterShellExtPreviewHandler(token, CLSID_CodePreview, 
				HANDLER_NAME);
			token = wcstok_s( NULL, L";", &next_token);
		}
	}

	return hr;
}


//
//   FUNCTION: DllUnregisterServer
//
//   PURPOSE: Unregister the COM server and the context menu handler.
// 
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = S_OK;

	wchar_t szModule[MAX_PATH];
	if (GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule)) == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	// Unregister the component.
	hr = UnregisterInprocServer(CLSID_CodePreview, 
		APPID_CodePreview);
	if (SUCCEEDED(hr))
	{
		//Split supported filetypes and unregister each of them
		static wchar_t filetypes[] = SUPPORTED_FILETYPES;
		wchar_t *token, *next_token;
		token = wcstok_s(filetypes, L";", &next_token);
		while(token != NULL)
		{
			// Unregister the preview handler
			hr = UnregisterShellExtPreviewHandler(token, CLSID_CodePreview);
			token = wcstok_s( NULL, L";", &next_token);
		}
	}

	return hr;
}