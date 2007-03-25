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
	@file		AppSettings.cpp
	@brief		Implementation of AppSettings class
	@author		ICHIMIYA Hironori (Hiron)
	@date		2004.02.08 created
*/

#include "Prefix.h"
#include "File.h"
#include "FileException.h"
#include "XMLParser.h"
#include "UTF8Utils.h"
#include "AppSettings.h"
#include "DOMParseEventHandler.h"
#include "DOMUtils.h"
#include "NCDDocument.h"
#include "NCDDocumentFactory.h"
#include "NCDText.h"
#include "NCDElement.h"

static UTF8Char	Tag_CoveredCalcSettings[]		= "coveredcalcSettings";
	static UTF8Char Key_BaseFolder[]				= "baseFolder";
	static UTF8Char	Key_LastCoverDef[]				= "lastCoverDef";
	static UTF8Char	Key_LastCoverNo[]				= "lastCoverNo";
	static UTF8Char	Key_LastMainWindowX[]			= "lastMainWindowX";
	static UTF8Char	Key_LastMainWindowY[]			= "lastMainWindowY";
	static UTF8Char	Key_LastCoverBrowserX[]			= "lastCoverBrowserX";
	static UTF8Char	Key_LastCoverBrowserY[]			= "lastCoverBrowserY";
	static UTF8Char	Key_CoverBrowserVisible[]		= "coverBrowserVisible";
	static UTF8Char Key_MainWindowAlwaysOnTop[]		= "mainWindowAlwaysOnTop";
	static UTF8Char Key_MainWindowLocked[]			= "mainWindowLocked";
	static UTF8Char	Key_LangFilePath[]				= "languageFile";
#if defined(ZETA)
	static UTF8Char	Key_UseLocaleKit[]				= "useLocaleKit";
#endif	// defined(ZETA)

const Path AppSettings::Value_LangFileBuiltIn("#Built-In#");

// ---------------------------------------------------------------------
//! Constructor
// ---------------------------------------------------------------------
AppSettings::AppSettings()
{
	settingFileName.Empty();

	baseFolder.Empty();
	lastCoverDef.Empty();
	lastCoverNo = 0;
	lastMainWindowPos.x = lastMainWindowPos.y = 0;
	lastCoverBrowserPos.x = lastCoverBrowserPos.y = 0;
	isCoverBrowserVisible = false;
	isMainWindowAlwaysOnTop = false;
	isMainWindowLocked = false;
	domDocument = NULL;
	langFilePath.Empty();
#if defined (ZETA)
	isLocaleKitAvailable = true;
#endif	// defined (ZETA)
}

// ---------------------------------------------------------------------
//! Destructor
// ---------------------------------------------------------------------
AppSettings::~AppSettings()
{
	if (NULL != domDocument)
	{
		domDocument->release();
	}
}

/**
 * synchronize member variable with DOM, and conversely
 */
void AppSettings::syncMembersWithDOM(
	bool isStoreToDOM		///< if true, member variable to DOM.
							///< if false, DOM to member variable.
)
{
	if (isStoreToDOM)
	{
		if (!baseFolder.IsEmpty())
		{
			// special case
			// - don't set default base folder value to dom tree. it is a secret setting at present.
			setSettingValueToDOM(Key_BaseFolder,			SettingValue(baseFolder));
		}
		setSettingValueToDOM(Key_LastCoverDef,			SettingValue(lastCoverDef));
		setSettingValueToDOM(Key_LastCoverNo,			SettingValue(lastCoverNo));
		setSettingValueToDOM(Key_LastMainWindowX,		SettingValue(lastMainWindowPos.x));
		setSettingValueToDOM(Key_LastMainWindowY,		SettingValue(lastMainWindowPos.y));
		setSettingValueToDOM(Key_LastCoverBrowserX,		SettingValue(lastCoverBrowserPos.x));
		setSettingValueToDOM(Key_LastCoverBrowserY,		SettingValue(lastCoverBrowserPos.y));
		setSettingValueToDOM(Key_CoverBrowserVisible,	SettingValue(isCoverBrowserVisible));
		setSettingValueToDOM(Key_MainWindowAlwaysOnTop,	SettingValue(isMainWindowAlwaysOnTop));
		setSettingValueToDOM(Key_MainWindowLocked,		SettingValue(isMainWindowLocked));
		setSettingValueToDOM(Key_LangFilePath,			SettingValue(langFilePath));
#if defined (ZETA)
		setSettingValueToDOM(Key_UseLocaleKit,			SettingValue(isLocaleKitAvailable));
#endif // defined (ZETA)
	}
	else
	{
		SettingValue settingValue;
		if (getSettingValueFromDOM(Key_BaseFolder, settingValue))
		{
			settingValue.AsPath(baseFolder);
		}
		if (getSettingValueFromDOM(Key_LastCoverDef, settingValue))
		{
			settingValue.AsPath(lastCoverDef);
		}
		if (getSettingValueFromDOM(Key_LastMainWindowX, settingValue))
		{
			lastMainWindowPos.x = settingValue.AsSInt32();
		}
		if (getSettingValueFromDOM(Key_LastMainWindowY, settingValue))
		{
			lastMainWindowPos.y = settingValue.AsSInt32();
		}
		if (getSettingValueFromDOM(Key_LastCoverBrowserX, settingValue))
		{
			lastCoverBrowserPos.x = settingValue.AsSInt32();
		}
		if (getSettingValueFromDOM(Key_LastCoverBrowserY, settingValue))
		{
			lastCoverBrowserPos.y = settingValue.AsSInt32();
		}
		if (getSettingValueFromDOM(Key_CoverBrowserVisible, settingValue))
		{
			isCoverBrowserVisible = settingValue.AsBool();
		}
		if (getSettingValueFromDOM(Key_MainWindowAlwaysOnTop, settingValue))
		{
			isMainWindowAlwaysOnTop = settingValue.AsBool();
		}
		if (getSettingValueFromDOM(Key_MainWindowLocked, settingValue))
		{
			isMainWindowLocked = settingValue.AsBool();
		}
		if (getSettingValueFromDOM(Key_LangFilePath, settingValue))
		{
			settingValue.AsPath(langFilePath);
		}
#if defined (ZETA)
		if (getSettingValueFromDOM(Key_UseLocaleKit, settingValue))
		{
			isLocaleKitAvailable = settingValue.AsBool();
		}
#endif // defined (ZETA)
	}
}

// ---------------------------------------------------------------------
//! Loads settings from file.
/*!
	@throw FileException when failed in reading a setting file.
	@throw XMLParseException when failed in parsing a setting file.
*/
// ---------------------------------------------------------------------
void AppSettings::Load(
	const Path& settingFileName		//!< file in which setting is written
)
{
	if (NULL != domDocument)
	{
		domDocument->release();
		domDocument = NULL;
	}
	this->settingFileName = settingFileName;

	XMLParser parser;
	parser.Create();
	UInt32 dataSize;
	
	// read data from file
	File file;
	file.Open(settingFileName, FileConstants::OpenMode_ReadOnly, FileConstants::OpenOption_None);
	dataSize = file.GetSize();
	Byte* buf = static_cast<Byte*>(parser.GetParseBuffer(dataSize));
	file.Read(buf, dataSize);
	file.Close();
	
	// parse it
	DOMParseEventHandler handler;
	parser.SetEventHandler(&handler);
	parser.ParseBuffer(dataSize, true);
	
	domDocument = handler.DetachDocument();
	loadDefaultToMember();
	syncMembersWithDOM(false);
}

/**
 * Loads default settings to member variable.
 */
void AppSettings::loadDefaultToMember()
{
	baseFolder.Empty();
	lastCoverDef = Path("Covers").Append("default").Append("CoverDef.xml");
	lastCoverNo = 0;
	lastMainWindowPos.x = lastMainWindowPos.y = 100;
	lastCoverBrowserPos.x = lastCoverBrowserPos.y = 200;
	isCoverBrowserVisible = false;
	isMainWindowAlwaysOnTop = false;
	isMainWindowLocked = false;
	langFilePath.Empty();
#if defined (ZETA)
	isLocaleKitAvailable = true;
#endif	// defined (ZETA)
}

// ---------------------------------------------------------------------
//! Loads default settings.
// ---------------------------------------------------------------------
void AppSettings::LoadDefault()
{
	if (NULL != domDocument)
	{
		domDocument->release();
		domDocument = NULL;
	}
	settingFileName.Empty();
	loadDefaultToMember();
}

// ---------------------------------------------------------------------
//! Saves settings to the file from which setting was loaded.
// ---------------------------------------------------------------------
void AppSettings::Save()
{
	SaveAs(settingFileName);
}

// ---------------------------------------------------------------------
//! Saves settings to specified file.
// ---------------------------------------------------------------------
void AppSettings::SaveAs(
	const Path& settingFileName			//!< setting file path
)
{
	this->settingFileName = settingFileName;
	if (settingFileName.IsEmpty())
	{
		return;
	}

	syncMembersWithDOM(true);
	if (NULL == domDocument)
	{
		ASSERT(false);
		return;
	}

	File file;
	file.Open(settingFileName, FileConstants::OpenMode_WriteOnly, FileConstants::OpenOption_Create | FileConstants::OpenOption_Erase);
	DOMUtils::WriteOutAsXML(domDocument, &file);
	file.Close();
}

/// searches first text node.
class FirstTextNodeFinder : public DOMUtils::NodeVisitor
{
private:
	NCDNode*	foundNode;
public:
	FirstTextNodeFinder() { foundNode = NULL; }
	virtual bool Visit(NCDNode* node)
	{
		if (NCDNode::TEXT_NODE == node->getNodeType())
		{
			foundNode = node;
			return false;
		}
		return true;
	}
	NCDNode* GetFoundNode() { return foundNode; }
};

/**
 * retrieves setting value of specified key from DOM tree.
 *
 * @retval	true	success
 * @retval	false	failed (key is not found)
 */
bool AppSettings::getSettingValueFromDOM(
	ConstUTF8Str key,			///< key
	SettingValue& outValue		///< value of specified key is set
)
{
	if (NULL == domDocument)
	{
		return false;
	}

	NCDNode* rootNode = domDocument->getDocumentElement();
	if (NULL == rootNode)
	{
		return false;
	}

	NCDNode* settingNode = DOMUtils::GetFirstMatchNode(rootNode, key, true);
	if (NULL == settingNode)
	{
		return false;
	}
	
	FirstTextNodeFinder finder;
	if (DOMUtils::ForEachChild(settingNode, &finder))
	{
		return false;
	}
	NCDNode* settingTextNode = finder.GetFoundNode();
	
	UTF8String valueString;
	settingTextNode->getNodeValue(valueString);
	outValue.SetValue(valueString);
	return true;
}

/**
 * stores setting value of specified key to DOM tree.
 */
void AppSettings::setSettingValueToDOM(
	ConstUTF8Str key,					///< key 
	const SettingValue& value			///< value
)
{
	UTF8String valueString;
	value.AsUTF8String(valueString);

	if (NULL == domDocument)
	{
		domDocument = NCDDocumentFactory::CreateDocument();
	}

	NCDNode* rootNode = domDocument->getDocumentElement();
	if (NULL == rootNode)
	{
		rootNode = domDocument->createElement(Tag_CoveredCalcSettings);
		domDocument->appendChild(rootNode);
	}
		
	NCDNode* settingNode = DOMUtils::GetFirstMatchNode(rootNode, key, true);
	if (NULL == settingNode)
	{
		NCDNode* lastChild = rootNode->getLastChild();
		if (NULL != lastChild && NCDNode::TEXT_NODE == lastChild->getNodeType())
		{
			UTF8String lastChildText;
			lastChild->getNodeValue(lastChildText);
			if (lastChildText[lastChildText.Length() - 1] == TypeConv::AsUTF8('\n'))
			{
				static_cast<NCDText*>(lastChild)->appendData(TypeConv::AsUTF8("  "));
			}
			else
			{
				static_cast<NCDText*>(lastChild)->appendData(TypeConv::AsUTF8("\n  "));
			}
		}
		else
		{
			NCDNode* textNodeForLooking = domDocument->createTextNode(TypeConv::AsUTF8("\n  "));
			rootNode->appendChild(textNodeForLooking);
		}
		
		settingNode = domDocument->createElement(key);
		rootNode->appendChild(settingNode);
		rootNode->appendChild(domDocument->createTextNode(TypeConv::AsUTF8("\n")));
	}
	
	FirstTextNodeFinder finder;
	if (DOMUtils::ForEachChild(settingNode, &finder))
	{
		NCDNode* settingTextNode = domDocument->createTextNode(valueString);
		settingNode->appendChild(settingTextNode);
	}
	else
	{
		NCDNode* settingTextNode = finder.GetFoundNode();
		settingTextNode->setNodeValue(valueString);
	}
}
