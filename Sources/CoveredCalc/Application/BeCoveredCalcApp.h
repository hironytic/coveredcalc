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
	@file		BeCoveredCalcApp.h
	@brief		Definition of BeCoveredCalcApp class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.16 created
*/

#ifndef _BECOVEREDCALCAPP_H_
#define _BECOVEREDCALCAPP_H_

#include <app/Application.h>
#include "CoveredCalcAppBase.h"

class BeMainWindow;
class BeCoverBrowser;

// ---------------------------------------------------------------------
//! CoveredCalc application class for BeOS
// ---------------------------------------------------------------------
class BeCoveredCalcApp : public BApplication, public CoveredCalcAppBase
{
public:
	static BeCoveredCalcApp*	GetInstance() { return reinterpret_cast<BeCoveredCalcApp*>(be_app); }

	virtual void				ShowCoverBrowser(bool isShow);
	virtual bool				IsCoverBrowserVisible();

#if defined(ZETA)
	virtual void				GetCurrentLanguageCode(MBCString& outLanguage);
#endif // degined(ZETA)

	virtual bool				CheckPlatform(ConstUTF8Str platform);

	// implementation of MessageBoxProvider
	virtual Button				DoMessageBox(ConstAStr message, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);
	virtual Button				DoMessageBox(SInt32 messageId, ButtonType buttonType, AlertType alertType, Button defaultButton = Button_None);

	// implementation of WaitingUI
	virtual	void				BeginWaitingUI();
	virtual	void				EndWaitingUI();

								BeCoveredCalcApp();
	virtual						~BeCoveredCalcApp();

#if defined(ZETA)
	bool						IsLocaleKitAvailable() const { return isLocaleKitAvailable; }
	virtual MBCString			LoadNativeString(SInt32 stringId);
#endif // defined(ZETA)

private:
	virtual	void				ReadyToRun();
	virtual bool				QuitRequested();
	virtual void				AboutRequested();
	virtual void				ArgvReceived(int32 argc, char** argv);

protected:
	virtual const Path&			getAppFolderPath();
	virtual const Path&			getUserSettingsPath();

private:
	void						loadKeyMappingsOnInit();
	void						loadKeyNameDB();

private:
	Path						appFolderPath;		///< the folder in which app program exists.
	Path						userSettingsPath;	///< the folder in which user settings is stored.
	BeMainWindow*				mainWindow;			//!< main window
	BeCoverBrowser*				coverBrowser;		//!< cover browser	
	bool						commandLineParamProcessed;	///< whether command line parameter is already processed.
#if defined(ZETA)
	bool						isLocaleKitAvailable;	///< whether uses locale kit.
#endif // defined(ZETA)
};

#endif // _BECOVEREDCALCAPP_H_
