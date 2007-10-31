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
	@file		PreferencesDlg.h
	@brief		Definition of PreferencesDlg class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2006.1.15 created
*/

#ifndef _PREFERENCESDLG_H_
#define _PREFERENCESDLG_H_

class LangFileInfoCollection;
class Path;

/**
 *	@brief	This is a base class of preferences dialog class.
 */
class PreferencesDlg
{
public:
								PreferencesDlg();
	virtual						~PreferencesDlg();
	
protected:
	void						loadToDialog();
	bool						saveFromDialog();

protected:
	
	/**
	 *	@brief	Sets language menu and current item.
	 *	@param	langFileInfos			This collection contains informations about all language menu items.
	 *	@param	currentLangFilePath		Current selection.
	 *	@param	isLocaleKitAvailable	whether uses locale kit (only ZETA)
	 *	@note	The LangFileInfoCollection object, which is specified in langFileInfos parameter, is valid
	 *			until next setLanguage is called or the dialog is closed. 
	 */
	virtual void				setLanguage(const LangFileInfoCollection& langFileInfos, const Path& currentLangFilePath
#if defined(ZETA)
										, bool isLocaleKitAvailable
#endif
								) = 0;

	/**
	 *	@brief	Retrieves current item of language menu.
	 *	@param[out]	langFilePath			a path of the language file selected on language menu.
	 *	@param[out]	isLocaleKitAvailable	whether uses locale kit (only ZETA)
	 *	@return	true if succeeded, otherwise false.
	 *	@note	If error occured, the error message is shown in this function before it returns false.
	 */
	virtual	bool				getLanguage(Path& langFilePath
#if defined(ZETA)
										, bool& isLocaleKitAvailable
#endif
								) = 0;

#if defined (WIN32)
	/**
	 *	@brief	Set window opacity.
	 *	@param	opacity	opacity value (from 0 to 255)
	 */
	virtual void				setOpacity(SInt32 opacity) = 0;
	
	/**
	 *	@brief	Get window opacity.
	 *	@return	opacity value (from 0 to 255)
	 */
	virtual SInt32				getOpacity() = 0;
	
	/**
	 *	@brief	Enables or disables opacity control.
	 *	@param	isEnabled	enables the control when true.
	 */
	virtual void				enableOpacity(bool isEnabled) = 0;
	
	/**
	 *	@brief	Set edge smoothing level.
	 *	@param	edgeSmoothing	the level (from 0 to 10. 0 means no smoothing).
	 */
	virtual void				setEdgeSmoothing(SInt32 edgeSmoothing) = 0;
	
	/**
	 *	@brief	Get edge smoothing level.
	 *	@return	the level (from 0 to 10. 0 means no smoothing).
	 */
	virtual SInt32				getEdgeSmoothing() = 0;
	
	/**
	 *	@brief	Enables or disables edge-smoothing control.
	 *	@param	isEnabled	enables the control when true.
	 */
	virtual void				enableEdgeSmoothing(bool isEnabled) = 0;
#endif	// defined (WIN32)
};

#endif // _PREFERENCESDLG_H_