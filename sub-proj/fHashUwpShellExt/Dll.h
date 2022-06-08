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

class __declspec(uuid("A90C1F88-96D8-47E1-8F96-6C37E1162C8D")) CExplorerCommandVerb;

HRESULT CExplorerCommandVerb_CreateInstance(REFIID riid, void **ppv);
HRESULT CExplorerCommandVerb_RegisterUnRegister(bool fRegister);
