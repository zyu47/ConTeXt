#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H
// This is used to modify the contextMenu.xml file of Npp
// Add commands to right-click context menu
#include "StdAfx.h"
#include "tinyxml.h"
#include "Scintilla.h"
#include "PluginInterface.h"

class modifyContextMenu 
{
public:
	modifyContextMenu(HWND hNotePad);
	//static modifyContextMenu* create(HWND hNotepad);
	//modifyContextMenu* getInstance() {return s_modifyContextMenu;}
	//static void deleteInstance();	
	~modifyContextMenu();
	bool loadContextXml();
	void countUserItems();
	void clear();
	bool update(const TCHAR*, idx_t, const TCHAR*);
	void editXML();
	void save() 
	{
		_pXmlFile->SaveFile(xmlDir);
	}

	size_t userChildCount;

private:
	//static modifyContextMenu* s_modifyContextMenu;
	HWND hNpp;
	TiXmlDocument *_pXmlFile;
	TCHAR menuXmlName[100] = TEXT("contextMenu.xml");
	TiXmlNode* parent; // <ScintillaContextMenu>
	TiXmlNode* firstNonUserChild;
	TiXmlNode* firstUserChild;
	TCHAR xmlDir[MAX_PATH];
	//TiXmlElement *
};

#endif // !CONTEXTMENU_H

