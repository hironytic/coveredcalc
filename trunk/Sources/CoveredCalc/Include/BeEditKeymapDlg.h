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
	@file		BeEditKeymapDlg.h
	@brief		Definition of BeEditKeymapDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2008.06.01 created
*/

#ifndef _BEEDITKEYMAPDLG_H_
#define _BEEDITKEYMAPDLG_H_

#include "BeDialog.h"
#include "EditKeymapDlg.h"
#include "DefaultUICTextEdit.h"
#include "DefaultUICListBox.h"
#include "DefaultUICButton.h"
#include "DefaultUICKeyInput.h"

class BeDialogDesign;

/**
 *	@brief	Edit-Keymapping dialog on BeOS.
 */
class BeEditKeymapDlg : public BeDialog, public EditKeymapDlg
{
public:
								BeEditKeymapDlg(BeDialogDesign* dialogDesign);
	virtual						~BeEditKeymapDlg();
	
	void						Init(BWindow* parent, sem_id semDialog, bool isReadOnly, const KeyNameDB* keyNameDB);
	bool						IsDialogClosedByOK() { return isDialogOK; }
	
protected:
	virtual void				MessageReceived(BMessage *message);
	virtual bool				QuitRequested();
#if defined (ZETA)
	virtual void				languageChanged();
#endif

	virtual MessageBoxProvider*	getMessageBoxProvider();

	virtual UICTextEdit*		getNameTextEdit()				{ return &uicNameTextEdit; }
	virtual UICListBox*			getFunctionListBox()			{ return &uicFunctionListBox; }
	virtual UICListBox*			getCurrentKeysListBox()			{ return &uicCurrentKeyListBox; }
	virtual	UICKeyInput*		getKeyInput()					{ return &uicKeyInput; }
	virtual UICTextEdit*		getAssignedFunctionTextEdit()	{ return &uicAssignedFunctionTextEdit; }
	virtual UICButton*			getAssignButton()				{ return &uicAssignButton; }
	virtual UICButton*			getRemoveButton()				{ return &uicRemoveButton; }
	virtual UICButton*			getOKButton()					{ return &uicOkButton; }
	virtual UICButton*			getCancelButton()				{ return &uicCancelButton; }

private:
	void						createViews();

private:
	BeDialogDesign*				dialogDesign;		///< dialog design
	sem_id						semDialog;			///< this semaphore is released when dialog is closed.
	bool						isDialogOK;			///< whether dialog is closed by OK button.
	
	// adapters
	DefaultUICTextEdit			uicNameTextEdit;
	DefaultUICListBox			uicFunctionListBox;
	DefaultUICListBox			uicCurrentKeyListBox;
	DefaultUICKeyInput			uicKeyInput;
	DefaultUICTextEdit			uicAssignedFunctionTextEdit;
	DefaultUICButton			uicAssignButton;
	DefaultUICButton			uicRemoveButton;
	DefaultUICButton			uicOkButton;
	DefaultUICButton			uicCancelButton;
};

#endif // _BEEDITKEYMAPDLG_H_
