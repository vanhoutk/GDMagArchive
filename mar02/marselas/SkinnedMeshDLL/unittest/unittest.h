// unittest.h : main header file for the UNITTEST application
//

#if !defined(AFX_UNITTEST_H__74890A22_0BF8_4AE9_8FAB_86C28414C30D__INCLUDED_)
#define AFX_UNITTEST_H__74890A22_0BF8_4AE9_8FAB_86C28414C30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnittestApp:
// See unittest.cpp for the implementation of this class
//

class CUnittestApp : public CWinApp
{
public:
	CUnittestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnittestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnittestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNITTEST_H__74890A22_0BF8_4AE9_8FAB_86C28414C30D__INCLUDED_)
