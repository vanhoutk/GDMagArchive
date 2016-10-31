/*===========================================================================*\
 | 
 |  FILE:	ViewWindow.cpp
 |			Test of the ViewWindow class
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Harry Denholm
 |			Developer Consulting Group
 |			Copyright(c) Discreet 1999
 |
 |  HIST:	Started 21-1-99
 | 
 |  Portions Copyright (c) 2001 Microsoft Corporation. All rights reserved.
\*===========================================================================*/

#include "pch.h"
#include "GlobalUtility.h"
#include <d3d8.h>
#include <D3dx8mesh.h>

#include "skinnedmesh.h"


extern CMyD3DApplication g_d3dApp;

extern HINSTANCE ghInstance;

extern void pauseRenderer(bool b);

extern HWND createRenderWindow(HINSTANCE hInst, HWND hParent, int x, int y, int w, int h);

static SkeletonGUP *gContext;

/*
BOOL CALLBACK ViewpanelProc(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SkeletonGUP *gp = (SkeletonGUP*)GetWindowLong(hWnd,GWL_USERDATA);
	if (!gp && msg!=WM_INITDIALOG) return FALSE;

	switch (msg) {

		// Update the pointer to the GUP 
		case WM_INITDIALOG:{
			gp = (SkeletonGUP*)lParam;
			SetWindowLong(hWnd,GWL_USERDATA,(LONG)gp);
			break;}

		// Resize/move any controls on our page
		case WM_SIZE:{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);
			SetWindowPos(GetDlgItem(hWnd,IDC_FRAME), NULL, 10, 10, w-20, h-20, SWP_NOZORDER);
			break;}


		// Make us the active viewpanel
		case WM_LBUTTONDOWN:
			gp->Max()->MakeExtendedViewportActive(hWnd);
			break;


		// Display the list of view windows
		case WM_RBUTTONDOWN:{
			POINT lpPt; GetCursorPos(&lpPt);
			RECT r;
			GetWindowRect(hWnd,&r);
			lpPt.x -= r.left;
			lpPt.y -= r.top;

			gp->Max()->MakeExtendedViewportActive(hWnd);
			gp->Max()->PutUpViewMenu(hWnd,lpPt);
			break;}


		default:
			return FALSE;
	}
	return TRUE;
}
*/

LRESULT MaxHandleWindowMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch(uMsg)
   {
      case WM_SETFOCUS:
         DisableAccelerators();
         break;

      case WM_KILLFOCUS:
         EnableAccelerators();
         break;

		// Make us the active viewpanel
		case WM_LBUTTONDOWN:
			gContext->Max()->MakeExtendedViewportActive(hWnd);
			break;


		// Display the list of view windows
		case WM_RBUTTONDOWN:
         {
			   POINT lpPt; GetCursorPos(&lpPt);
			   RECT r;
			   GetWindowRect(hWnd,&r);
			   lpPt.x -= r.left;
			   lpPt.y -= r.top;

   	      gContext->Max()->MakeExtendedViewportActive(hWnd);

            // only display Max context window if we're at the top of the window
            if (lpPt.y < 50)
			      gContext->Max()->PutUpViewMenu(hWnd,lpPt);
         }
			break;

      default:
         break;
   }

   return 0;

}


HWND SkeletonViewWindow::CreateViewWindow(HWND hParent, int x, int y, int w, int h)
{
   gContext = gupPtr;

   if (IsWindow(g_d3dApp.GetMainWindow()))
   {
      pauseRenderer(false);

      return g_d3dApp.GetMainWindow();
   }

   return createRenderWindow(ghInstance, hParent, x, y, w, h);

/*
	// Create a dialog box, parented to the viewpane
	// Pass in the pointer to the GUP
	HWND newWnd = CreateDialogParam(
		(HINSTANCE)hInstance,
		MAKEINTRESOURCE(IDD_VIEWPANEL),
		hParent,
		(DLGPROC)ViewpanelProc,
		(LPARAM)(SkeletonGUP*)gupPtr);

	// Set the window's position
	SetWindowPos(newWnd, NULL, x, y, w, h, SWP_NOZORDER);
	ShowWindow(newWnd,TRUE);


	return newWnd;
*/

}

void SkeletonViewWindow::DestroyViewWindow(HWND hWnd)
{
   pauseRenderer(true);

	//EndDialog(hWnd,0);

//   DestroyWindow(hWnd);
}
