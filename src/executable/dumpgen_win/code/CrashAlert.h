///////////// Copyright © 2010 DesuraNet. All rights reserved. /////////////
//
//   Project     : dumpgen
//   File        : CrashAlert.h
//   Description :
//      [TODO: Write the purpose of CrashAlert.h.]
//
//   Created On: 9/24/2010 7:37:23 PM
//   Created By:  <mailto:>
////////////////////////////////////////////////////////////////////////////

#ifndef DESURA_CRASHALERT_H
#define DESURA_CRASHALERT_H
#ifdef _WIN32
#pragma once
#endif

#include "Resource.h"

// CrashAlert frame

class CrashAlert : public CDialog
{
public:
	CrashAlert();           // protected constructor used by dynamic creation
	virtual ~CrashAlert();

protected:
	virtual BOOL OnInitDialog();

private:

	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
};

#endif //DESURA_CRASHALERT_H






