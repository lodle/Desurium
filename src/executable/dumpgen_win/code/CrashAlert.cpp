// CrashAleart.cpp : implementation file
//

#include "stdafx.h"
#include "CrashAlert.h"

BEGIN_MESSAGE_MAP(CrashAlert, CDialog)
	ON_BN_CLICKED(IDYES, &CrashAlert::OnBnClickedYes)
END_MESSAGE_MAP()

// CrashAleart
CrashAlert::CrashAlert()  : CDialog(IDD_DIALOG1, NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CrashAlert::~CrashAlert()
{
}

BOOL CrashAlert::OnInitDialog()
{
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return CDialog::OnInitDialog();
}

void CrashAlert::OnBnClickedYes()
{
	EndDialog(IDYES);
}
