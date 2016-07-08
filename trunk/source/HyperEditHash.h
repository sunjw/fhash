#pragma once

#include "HyperEdit.h"

// CHyperEditHash

class CHyperEditHash : public CHyperEdit
{
	DECLARE_DYNAMIC(CHyperEditHash)

public:
	CHyperEditHash();
	virtual ~CHyperEditHash();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
};


