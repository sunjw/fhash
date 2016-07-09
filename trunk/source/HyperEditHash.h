#pragma once

#include "HyperEdit.h"

// CHyperEditHash
#define CHyperEditHash_BASE_CLASS CHyperEdit

class CHyperEditHash : public CHyperEditHash_BASE_CLASS
{
	DECLARE_DYNAMIC(CHyperEditHash)

public:
	CHyperEditHash();
	virtual ~CHyperEditHash();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);

	virtual HINSTANCE OpenHyperlink(const CString& hyperlink, CPoint point) const;

};


