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
	@file		AboutCurrentCoverDlg.h
	@brief		Definition of AboutCurrentCoverDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2005.02.02 created
*/

#ifndef _ABOUTCURRENTCOVERDLG_H_
#define _ABOUTCURRENTCOVERDLG_H_

#include "CoverChangeEventHandler.h"

class CoverDef;
class UICTextEdit;

// ---------------------------------------------------------------------
//! This dialog shows informations about the current cover.
// ---------------------------------------------------------------------
class AboutCurrentCoverDlg : public CoverChangeEventHandler
{
public:
								AboutCurrentCoverDlg();
	virtual						~AboutCurrentCoverDlg();

	// implementations of CoverChangeEventHandler interface
	virtual void				CoverDefChanged();
	virtual void				CurrentCoverChanged();

protected:
	enum ComponentID
	{
		CID_NameTextEdit,
		CID_DescriptionTextEdit,
		CID_AboutTextEdit,
	};

	virtual UICTextEdit*		getNameTextEdit() = 0;
	virtual UICTextEdit*		getDescriptionTextEdit() = 0;
	virtual UICTextEdit*		getAboutTextEdit() = 0;

protected:
	void						initialize();
	void						terminate();
	virtual void				update();

private:
	void						setDataToDialog(const CoverDef* coverDef);
};

#endif // _ABOUTCURRENTCOVERDLG_H_
