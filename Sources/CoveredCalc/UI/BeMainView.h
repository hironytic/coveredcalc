/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2008 CoveredCalc Project Contributors
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
	@file       BeMainView.h
	@brief      Definition of BeMainView class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#ifndef _BEMAINVIEW_H_
#define _BEMAINVIEW_H_

#include "BeSkinView.h"
#include "MainUIController.h"
#include "BeDeskbarPseudoWindow.h"
#include "BeDialogManager.h"

class BeXMLLangFile;
class BPopUpMenu;

// ---------------------------------------------------------------------
//! Main view in the main window for BeOS
// ---------------------------------------------------------------------
class BeMainView : public BeSkinView, public MainUIController
{
public:
							BeMainView(BRect frame, const char* name);
	virtual					~BeMainView();

	virtual void			Init();

	virtual void			Minimize();
	virtual void 			SetAlwaysOnTopFlag(bool isFlagOn);

	BeDeskbarPseudoWindow*	GetDeskbarWindow() const { return deskbarWindow; }

	void					MainWindowActivated();

	// MainUIController interface
	virtual void			ShowMainUIContextMenu(Point32 menuPos);
	virtual void			ShowAboutDialog();
	virtual void			ShowAboutCurrentCoverDialog();
	virtual void			ShowPreferencesDialog();

protected:
	virtual UIManager*		createUIManager();
	virtual void			deleteUIManager(UIManager* uiManager);
	
	virtual SInt32			getMenuCommand(uint32 menuCommand);

private:
	BeDeskbarPseudoWindow*	deskbarWindow;		///< pseudo window for main window.

private:
	class BeAboutDialogManager : public BeGenericDialogManager
	{
	protected:
		virtual BeDialog*	newDialogObject();
	};
	BeAboutDialogManager	aboutDialogManager;	//!< About dialog manager object.

	class BeAboutCCDialogManager : public BeGenericDialogManager
	{
	protected:
		virtual BeDialog*	newDialogObject();
	};
	BeAboutCCDialogManager	aboutCCDialogManager;	//!< About Current Cover dialog manager object.
	
	class BePrefDialogManager : public BeGenericDialogManager
	{
	protected:
		virtual BeDialog*	newDialogObject();
	};
	BePrefDialogManager		prefDialogManager;	//!< Preferences dialog manager object.
};

#endif // _BEMAINVIEW_H_
