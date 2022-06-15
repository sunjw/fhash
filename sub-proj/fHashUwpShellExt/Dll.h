// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once
#include "ShellHelpers.h"
#include "RegisterExtension.h"
#include <strsafe.h>
#include <new>  // std::nothrow

void DllAddRef();
void DllRelease();

// use UUDIGEN.EXE to generate unique CLSID values for your objects
#if defined (_DEBUG)
class __declspec(uuid("DFCD2B9E-91B8-4529-B8FD-A512A2046D52")) CExplorerCommandVerb;
#else
class __declspec(uuid("F4137605-293D-4FE1-A223-9C53CC2D3949")) CExplorerCommandVerb;
#endif

HRESULT CExplorerCommandVerb_CreateInstance(REFIID riid, void **ppv);
HRESULT CExplorerCommandVerb_RegisterUnRegister(bool fRegister);
