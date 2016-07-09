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

	inline CString GetLastHyperlink()
	{ return m_cstrLastHyperlink; }

	inline CPoint GetLastScreenPoint()
	{ return m_cpLastScreenPoint; }

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);

	virtual HINSTANCE OpenHyperlink(const CString& hyperlink, CPoint point);

private:
	CString m_cstrLastHyperlink;
	CPoint m_cpLastScreenPoint;

};


