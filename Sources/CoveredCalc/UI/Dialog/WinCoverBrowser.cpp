/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2009 CoveredCalc Project Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
	@file		WinCoverBrowser.cpp
	@brief		WinCoverBrowser �N���X�̎���
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.3.11 created
*/

#include "Prefix.h"
#include "resource.h"
#include "Exception.h"
#include "ExceptionMessageUtils.h"
#include "CoveredCalcApp.h"
#include "UTF8Conv.h"
#include "CoverListItem.h"
#include "WinCoveredCalcApp.h"
#include "WinCoverBrowser.h"
#include "StringID.h"
#include "WinDialogControlCreator.h"
#include "DialogID.h"
#include "UICEventCode.h"

////////////////////////////////////////
#define baseWnd			WinDialog
#define baseBrowser		CoverBrowser
////////////////////////////////////////

// ---------------------------------------------------------------------
//! �R���X�g���N�^
// ---------------------------------------------------------------------
WinCoverBrowser::WinCoverBrowser() : baseWnd(IDD_COVER_BROWSER)
{
	smallIcon = NULL;
	largeIcon = NULL;
}

// ---------------------------------------------------------------------
//! �f�X�g���N�^
// ---------------------------------------------------------------------
WinCoverBrowser::~WinCoverBrowser()
{
	if (NULL != smallIcon)
	{
		::DestroyIcon(smallIcon);
	}
	if (NULL != largeIcon)
	{
		::DestroyIcon(largeIcon);
	}
}

// ---------------------------------------------------------------------
//! UI���͂ދ�`�̍��W�i�X�N���[�����W�j���擾���܂��B
// ---------------------------------------------------------------------
void WinCoverBrowser::GetUIRect(
	Rect32& rect			//!< OUTPUT. UI���͂ދ�`�̍��W���Ԃ�
) const
{
	if (::IsIconic(m_hWnd) || ::IsZoomed(m_hWnd))
	{
		RECT workArea;
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		WINDOWPLACEMENT windowPlacement;
		ZeroMemory(&windowPlacement, sizeof(windowPlacement));
		windowPlacement.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(m_hWnd, &windowPlacement);
		rect.left = windowPlacement.rcNormalPosition.left + workArea.left;
		rect.top = windowPlacement.rcNormalPosition.top + workArea.top;
		rect.right = windowPlacement.rcNormalPosition.right + workArea.left;
		rect.bottom = windowPlacement.rcNormalPosition.bottom + workArea.top;
	}
	else
	{
		RECT winRect;
		::GetWindowRect(m_hWnd, &winRect);
		rect.left = winRect.left;
		rect.top = winRect.top;
		rect.right = winRect.right;
		rect.bottom = winRect.bottom;
	}
}

/**
 *	@brief	Returns style of this dialog.
 */
DWORD WinCoverBrowser::getDialogStyle()
{
	return DS_SETFONT | DS_MODALFRAME | WS_MINIMIZEBOX | WS_POPUP | WS_CAPTION | WS_SYSMENU;
}

// ---------------------------------------------------------------------
//! �E�B���h�E�v���V�[�W��
/*!
	@return �e���b�Z�[�W�ɑΉ�����߂�l
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::wndProc(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< ���b�Z�[�W
	WPARAM wParam,		//!< ���b�Z�[�W�� WPARAM
	LPARAM lParam		//!< ���b�Z�[�W�� LPARAM
)
{
	try
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
			return onInitDialog(hWnd, uMsg, wParam, lParam);
			break;
		case WM_ENABLE:
			return onEnable(hWnd, uMsg, wParam, lParam);
			break;
		case WM_ACTIVATE:
			return onActivate(hWnd, uMsg, wParam, lParam);
			break;
		case WM_COMMAND:
			return onCommand(hWnd, uMsg, wParam, lParam);
			break;
		case WM_NOTIFY:
			return onNotify(hWnd, uMsg, wParam, lParam);
			break;
		case WM_DESTROY:
			return onDestroy(hWnd, uMsg, wParam, lParam);
			break;
		case WM_CLOSE:
			return onClose(hWnd, uMsg, wParam, lParam);
			break;
		default:
			return baseWnd::wndProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	catch (Exception* ex)
	{
		ExceptionMessageUtils::DoExceptionMessageBox(CoveredCalcApp::GetInstance(), ex);
		ex->Delete();
	}
	
	return 0;
}

/**
 *	@brief	Create cover list.
 *	@param[in] dcc WinDialogControlCreator
 */
void WinCoverBrowser::createCoverListControl(WinDialogControlCreator& dcc)
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();

	// create a control
	HWND listWnd = dcc.CreateListView(ALITERAL("IDC_COVER_LIST"), IDC_COVER_LIST, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | LVS_NOSORTHEADER, 0, 0, 0);
	DWORD exStyle = ListView_GetExtendedListViewStyle(listWnd);
	exStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(listWnd, exStyle);

	// add columns
	MBCString columnName;
	LVCOLUMN column;
	ZeroMemory(&column, sizeof(column));
	column.mask = LVCF_TEXT | LVCF_FMT;
	column.fmt = LVCFMT_LEFT;
	// -- name
	columnName = stringLoader->LoadNativeString(NSID_COVER_BROWSER_COLUMN_NAME);
	column.pszText = const_cast<LPTSTR>(columnName.CString());
	ListView_InsertColumn(listWnd, 0, &column);
	// -- description
	columnName = stringLoader->LoadNativeString(NSID_COVER_BROWSER_COLUMN_DESCRIPTION);
	column.pszText = const_cast<LPTSTR>(columnName.CString());
	ListView_InsertColumn(listWnd, 1, &column);

	// adjust column widths
	ListView_SetColumnWidth(listWnd, 0, 150);						// name
	ListView_SetColumnWidth(listWnd, 1, LVSCW_AUTOSIZE_USEHEADER);	// description

	// bind to adaptor
	uicCoverList.Init(listWnd);
}

/**
 *	@brief	Creates controls on dialog.
 */
void WinCoverBrowser::createControls()
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();
	WinDialogControlCreator dcc(m_hWnd, getDialogLayout());
	MBCString label;
	HWND hControl;

	// Select Cover label
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_COVER_BROWSER_SELECT_COVER));
	hControl = dcc.CreateStatic(ALITERAL("IDC_SELECT_COVER"), IDC_STATIC, label.CString(), WS_GROUP, 0, 0, 0);

	// Cover list
	createCoverListControl(dcc);

	// Reflesh button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_COVER_BROWSER_RELOAD));
	hControl = dcc.CreateButton(ALITERAL("IDC_RELOAD"), IDC_RELOAD, label, WS_GROUP, 0, 0, 0);

	// Apply button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_COVER_BROWSER_APPLY));
	hControl = dcc.CreateButton(ALITERAL("IDC_APPLY"), IDC_APPLY, label, WS_GROUP, 0, 0, 0);

	// Close button
	label = XMLLangFile::ConvertAccessMnemonic(stringLoader->LoadNativeString(NSID_COVER_BROWSER_CLOSE));
	hControl = dcc.CreateButton(ALITERAL("IDCLOSE"), IDCLOSE, label, 0, 0, 0, 0);
}

// ---------------------------------------------------------------------
//! WM_INITDIALOG �n���h��
/*!
	@retval TRUE wParam �Ŏw�肳�ꂽ�R���g���[���Ƀt�H�[�J�X��^���Ă��������B
	@retval FALSE �t�H�[�J�X��^���Ȃ��ł��������B
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onInitDialog(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_INITDIALOG
	WPARAM wParam,		//!< handle to the control to receive the default keyboard focus
	LPARAM lParam		//!< additional initialization data
)
{
	NativeStringLoader* stringLoader = CoveredCalcApp::GetInstance();

	baseWnd::wndProc(hWnd, uMsg, wParam, lParam);

	// create controls
	createControls();

	// set dialog title
	SetWindowText(m_hWnd, stringLoader->LoadNativeString(NSID_COVER_BROWSER_TITLE).CString());

	// �A�C�R���̃��[�h
	smallIcon = reinterpret_cast<HICON>(::LoadImage(CHrnApp::GetAppObject()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON,
											::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR));
	if (NULL != smallIcon)
	{
		::SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(smallIcon));
	}
	
	largeIcon = reinterpret_cast<HICON>(::LoadImage(CHrnApp::GetAppObject()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON,
											::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR));
	if (NULL != largeIcon)
	{
		::SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(largeIcon));
	}

	// �ʒu�𒲐�
	Point32 topLeft = getInitialLocation();
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);
	::MoveWindow(m_hWnd, topLeft.x, topLeft.y, rect.right - rect.left, rect.bottom - rect.top, TRUE);

	readyToShow();

	return TRUE;
}

// ---------------------------------------------------------------------
//! WM_ENABLE �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onEnable(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_ENABLE
	WPARAM wParam,		//!< TRUE �Ȃ�L���ɂȂ� / FALSE �Ȃ疳���ɂȂ�
	LPARAM lParam		//!< ���p���Ȃ��p�����[�^
)
{
	baseWnd::wndProc(hWnd, uMsg, wParam, lParam);

	// CoveredCalc �̑��̃E�B���h�E�����A��ɂ���B
	bool isEnabled = (wParam) ? true : false;
	WinCoveredCalcApp::GetInstance()->EnableCoveredCalcWindows(isEnabled);
	return 0;
}

// ---------------------------------------------------------------------
//! WM_ACTIVATE �n���h��
/*!
	UM_ACTIVATED ���������g�Ƀ|�X�g���܂��B
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onActivate(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_ACTIVATE
	WPARAM wParam,	//!< ���ʃ��[�h���A�N�e�B�u������邩��A�N�e�B�u������邩�������A��ʃ��[�h���E�B���h�E���ŏ�������Ă��邩�ǂ���������
	LPARAM lParam	//!< �A�N�e�B�u/��A�N�e�B�u�ɂȂ�E�B���h�E�̃n���h��
)
{
	baseWnd::wndProc(hWnd, uMsg, wParam, lParam);

	// ���[�_���_�C�A���O���o�Ă���Ƃ��̑Ώ�
	bool willBeActive = (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE) ? true : false;
	if (willBeActive && !::IsWindowEnabled(m_hWnd))
	{
		// ���Ԃ�A���[�_���_�C�A���O���o�Ă����ԂŃA�N�e�B�u�ɂ��悤�Ƃ����̂ŁA
		// �_�C�A���O�̕����A�N�e�B�u�ɂ���
		WinCoveredCalcApp::GetInstance()->ActivateModalDialog();		
	}
	return 0;
}

// ---------------------------------------------------------------------
//! WM_CLOSE �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onClose(
	HWND /*hWnd*/,		//!< �E�B���h�E�n���h��
	UINT /*uMsg*/,		//!< WM_DESTROY
	WPARAM /*wParam*/,	//!< ���p���Ȃ��p�����[�^
	LPARAM /*lParam*/	//!< ���p���Ȃ��p�����[�^
)
{
	HandleUICEvent(CID_CloseButton, UICE_ButtonClicked, 0, NULL);
	return 0;
}

// ---------------------------------------------------------------------
//! WM_DESTROY �n���h��
/*!
	@retval 0 ���������Ƃ�
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onDestroy(
	HWND hWnd,		//!< �E�B���h�E�n���h��
	UINT uMsg,		//!< WM_DESTROY
	WPARAM wParam,	//!< ���p���Ȃ��p�����[�^
	LPARAM lParam	//!< ���p���Ȃ��p�����[�^
)
{
	baseWnd::wndProc(hWnd, uMsg, wParam, lParam);
	baseBrowser::onDestroy();
	return 0;
}

// ---------------------------------------------------------------------
//! WM_COMMAND �n���h��
/*!
	@retval 0 ���̃��b�Z�[�W����������
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onCommand(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_COMMAND
	WPARAM wParam,		//!< ��ʃ��[�h���ʒm�R�[�h�A���ʃ��[�h���R�}���hID
	LPARAM lParam		//!< ���̃��b�Z�[�W�𑗂����R���g���[���̃n���h��
)
{
	WORD command = LOWORD(wParam);
	switch (command)
	{
	case IDC_RELOAD:
		HandleUICEvent(CID_ReloadButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDC_APPLY:
		HandleUICEvent(CID_ApplyButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	case IDCLOSE:
		HandleUICEvent(CID_CloseButton, UICE_ButtonClicked, 0, NULL);
		return 0;
		break;
	default:
		return baseWnd::wndProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

// ---------------------------------------------------------------------
//! WM_NOTIFY �n���h��
/*!
	@return ���Ɏw�肳��Ȃ�����A�߂�l�ɈӖ��͂Ȃ�
*/
// ---------------------------------------------------------------------
LRESULT WinCoverBrowser::onNotify(
	HWND hWnd,			//!< �E�B���h�E�n���h��
	UINT uMsg,			//!< WM_NOTIFY
	WPARAM wParam,		//!< ���b�Z�[�W�𑗐M�����R���g���[����ID (int)
	LPARAM lParam		//!< NMHDR �\���̂ւ̃|�C���^
)
{
	int idCtrl = static_cast<int>(wParam);
	NMHDR* nmHdr = reinterpret_cast<NMHDR*>(lParam);
	if (IDC_COVER_LIST == idCtrl)
	{
		if (NM_DBLCLK == nmHdr->code)
		{
			HandleUICEvent(CID_CoverList, UICE_ListItemInvoked, 0, NULL);
			return 0;
		}
	}
	
	return baseWnd::wndProc(hWnd, uMsg, wParam, lParam);
}