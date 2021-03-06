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
	@file		UIManager.h
	@brief		Definition of UIManager class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2003.12.28 created
*/

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "UIController.h"
#include "KeyMappingManager.h"
#include "KeyEventParameter.h"
#include "UITaskManager.h"

class UITask;
class ColorCodedSkin;
class ColorCodedSkinAppearance;

// ---------------------------------------------------------------------
//! Base class for user interface management
// ---------------------------------------------------------------------
class UIManager
{
public:
	/// command state combination flags
	enum {
		CommandState_None		= 0x0000,	///< normal state only
		CommandState_Disabled	= 0x0001,	///< command is disabled
		CommandState_Checked	= 0x0002,	///< command is checked (selected)
		CommandState_Radio		= 0x0004,	///< command is radio-button behavior
	};

	enum {
		Command_None = -1,	// no command
	};

public:
								UIManager();
	virtual						~UIManager();

	void						Init(UIController* uiController, const KeyMappingManager* keyMappingManager);

	virtual void				Create() = 0;
	virtual void				Destroy();

	virtual ColorCodedSkin*		GetSkin() { return getSkin(); }
	virtual ColorCodedSkinAppearance*	GetSkinAppearance() { return getSkinAppearance(); }

	void						RereadSkin() { readSkin(); }

	virtual void				StartTask(UITask* task);
	virtual void				EndTask(UITask* task);

	virtual SInt32				GetSkinAreaFromCommandId(SInt32 commandId) = 0;
	virtual UInt32				GetCommandState(SInt32 commandId) = 0;
	virtual void				ExecuteCommand(SInt32 commandId) = 0;
	
	// event handler for user interface
	virtual void				MouseMove();
	virtual void				Button1Down();
	virtual void				Button1Up();
	virtual void				Button2Down();
	virtual void				Button2Up();
	virtual void				UIActivated();
	virtual bool				KeyDown(const KeyEventParameter& parameter);
	virtual bool				KeyUp(const KeyEventParameter& parameter);
	virtual void				TimerEvent(SInt32 timerId);

	UIController*				GetUIController() { return uiController; }
	const UIController*			GetUIController() const { return uiController; }

protected:
	virtual void				clearMembers();
	void						setSkin(ColorCodedSkin* skin);
	void						setSkinAppearance(ColorCodedSkinAppearance* appearance);
	void						setBasePoint(const Point32& basePoint);
	void						setLastCursorSkinArea(SInt32 area);
	virtual void				changeLastCursorSkinArea(SInt32 area);

	UIController*				getUIController() { return uiController; }
	const KeyMappingManager*	getKeyMappingManager() { return keyMappingManager; }
	ColorCodedSkin*				getSkin() { return skin; }
	ColorCodedSkinAppearance*	getSkinAppearance() { return appearance; }
	UITaskManager*				getTaskManager() { return &taskManager; }
	const Point32&				getBasePoint() { return basePoint; }

	virtual void				readSkin() = 0;

private:
	UIController*				uiController;
	const KeyMappingManager*	keyMappingManager;
	ColorCodedSkin*				skin;
	ColorCodedSkinAppearance*	appearance;			///< skin appearance.
	UITaskManager				taskManager;
	Point32						basePoint;
	SInt32						lastCursorSkinArea;	///< skin area at cursor pos.
};

#endif // _UIMANAGER_H_
