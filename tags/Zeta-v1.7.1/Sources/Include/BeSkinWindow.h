/*
 * CoveredCalc
 *
 * Copyright (c) 2004-2007 CoveredCalc Project Contributors
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
	@file       BeSkinWindow.h
	@brief      Definition of BeSkinWindow class
	@author     ICHIMIYA Hironori (Hiron)
	@date       2004.02.18 created
*/

#ifndef _BESKINWINDOW_H_
#define _BESKINWINDOW_H_

#include <interface/Window.h>

class BeSkinView;

// ---------------------------------------------------------------------
//! Skin window for BeOS
/*!
	The window classes which use skins are derived from this class.
*/
// ---------------------------------------------------------------------
class BeSkinWindow : public BWindow
{
public:
						BeSkinWindow(BRect frame, const char* name, uint32 workspace, bool isAlwaysOnTop = false);
	virtual				~BeSkinWindow();
	
	virtual void		Init();

	BeSkinView*			GetSkinView() { return skinView; }

	virtual void		Quit();
	virtual void		WindowActivated(bool active);
	virtual void		MessageReceived(BMessage *message);

protected:
	virtual BeSkinView*	newSkinView() = 0;

private:
	BeSkinView*			skinView;		//!< skin view
};

#endif // _BESKINWINDOW_H_
