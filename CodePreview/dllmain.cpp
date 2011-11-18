/****************************** Module Header ******************************\
Module Name:  dllmain.cpp
Project:      CodePreview
Copyright (c) Microsoft Corporation.

The file implements DllMain, and the DllGetClassObject, DllCanUnloadNow, 
DllRegisterServer, DllUnregisterServer functions that are necessary for a COM 
DLL. 

DllGetClassObject invokes the class factory defined in ClassFactory.h/cpp and 
queries to the specific interface.

DllCanUnloadNow checks if we can unload the component from the memory.

DllRegisterServer registers the COM server and the preview handler in the 
registry by invoking the helper functions defined in Reg.h/cpp. The preview 
handler is associated with the .Scintilla file class.

DllUnregisterServer unregisters the COM server and the preview handler. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

/* Modified by Dennis Felsch */

#include <windows.h>
#include <Guiddef.h>
#include "ClassFactory.h"           // For the class factory
#include "Reg.h"
#include "StdAfx.h"

//Actual language is in "StdAfx.h"
//Every language needs its own extensions, name and IDs
#ifdef LANG_PYTHON
	#define SUPPORTED_FILETYPES L".py"
	// {DE7F719C-5E18-41D6-82E7-F2B201DC3F39}
	const CLSID CLSID_CodePreview = 
	{ 0xde7f719c, 0x5e18, 0x41d6, { 0x82, 0xe7, 0xf2, 0xb2, 0x1, 0xdc, 0x3f, 0x39 } };
	// {1A92DA73-0761-48A5-9A30-076A80B64E1F}
	const GUID APPID_CodePreview = 
	{ 0x1a92da73, 0x761, 0x48a5, { 0x9a, 0x30, 0x7, 0x6a, 0x80, 0xb6, 0x4e, 0x1f } };
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