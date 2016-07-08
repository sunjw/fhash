// HyperLink.cpp : implementation file
//
// HyperLink static control. Will open the default browser with the given URL
// when the user clicks on the link.
//
// Copyright (C) 1997 - 1999 Chris Maunder
// All rights reserved. May not be sold for profit.
//
// Thanks to Pål K. Tønder for auto-size and window caption changes.
//
// "GotoURL" function by Stuart Patterson
// As seen in the August, 1997 Windows Developer's Journal.
// Copyright 1997 by Miller Freeman, Inc. All rights reserved.
// Modified by Chris Maunder to use TCHARs instead of chars.
//
// "Default hand cursor" from Paul DiLascia's Jan 1998 MSJ article.

// These function have been borrowed from the article:
// http://www.codeproject.com/miscctrl/hyperlink.asp

#include "stdafx.h"
//#include "timeline.h"
#include "HyperEdit.h"

#include "atlconv.h"    // for Unicode conversion - requires #include <afxdisp.h> // MFC OLE automation classes
