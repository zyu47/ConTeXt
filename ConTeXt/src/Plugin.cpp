#include "StdAfx.h"
#include "PluginInterface.h"
#include "Version.h"
#include "Menumanager.h"
#include "ContextMenu.h"
#include "ConTeXtEditU.h"
#include "Tag.h"

static FuncItem	*funcItem = NULL;
static NppData nppData;
HWND g_NppWindow = nullptr;
HWND g_SCI = nullptr;


void About()
{
	MessageBox(
		g_NppWindow,
		L"By Jason Yu.\n"
		L"GitHub zyu47",
		ConTeXt_TITLE,
		MB_OK);
}

/// Notepad++ exports
BOOL isUnicode()
{
	return TRUE;
}

/// Notepad++ exports, return the name of this plugin
const TCHAR * getName()
{
	return L"&ConTeXt";
}

void replace()
{
	ConTeXtEditU::getInstance()->replaceTag();
}

void editConfig()
{
	ConTeXtEditU::getInstance()->EditConfig();
}

void loadConfig()
{
	MenuManager::getInstance()->loadConfig();
}

void updateContextMenu()
{
	MenuManager::getInstance()->updateContextMenu();
	MessageBox(nppData._nppHandle, L"Please restart Notepad++", L"ConTeXt", MB_OK);
}

void removeContextMenu()
{
	MenuManager::getInstance()->removeContextMenu();
	MessageBox(nppData._nppHandle, L"Please restart Notepad++", L"ConTeXt", MB_OK);
}

void editContextXML()
{
	modifyContextMenu mc(nppData._nppHandle);
	mc.editXML();
}

static FuncItem* getGeneratedFuncItemArray(int *nbF)
{

	MenuManager* menuManager = MenuManager::getInstance();
	MenuManager::ItemVectorTD items;

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("Replace tags"), replace));

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("Edit config"), editConfig));
	items.push_back(pair<tstring, void(*)()>(_T("Load config"), loadConfig));

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("Update Context Menu"), updateContextMenu));
	items.push_back(pair<tstring, void(*)()>(_T("Remove Context Menu"), removeContextMenu));
	items.push_back(pair<tstring, void(*)()>(_T("Edit Context XML"), editContextXML));

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("About"), About));

	FuncItem* funcItems = menuManager->getFuncItemArray(nbF, items);

	return funcItems;
}


/// Notepad++ exports
FuncItem * getFuncsArray(int *count) 
{
	funcItem = getGeneratedFuncItemArray(count);
	return funcItem;
}

/// Notepad++ exports
void setInfo(NppData data)
{
	nppData = data;
	g_NppWindow = data._nppHandle;
	
	// Get the current scintilla
	// Copied from notepad++ source code
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND g_SCI = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	// Create instances of ConTeXtEditU and MenuManager
	ConTeXtEditU::create(nppData._nppHandle, g_SCI);
	MenuManager::create(nppData._nppHandle);
}

/// Notepad++ exports
void beNotified(SCNotification *scn) 
{
	switch (scn->nmhdr.code)
	{
		case NPPN_READY:
		{
			MenuManager* menuManager = MenuManager::getInstance();
			menuManager->idsInitialised();
			menuManager->populatePairMenu();
		}
		break;

		case SCN_CHARADDED:
			// Place for in-plugin autocompletion
			break;

		case NPPN_SHUTDOWN:
		{
			ConTeXtEditU::deleteInstance();
			MenuManager::deleteInstance();
		}
		break;
	}
}

/// Notepad++ exports
LRESULT messageProc(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_COMMAND:
		// When user clicks a submenu, WM_COMMAND is processed here
		MenuManager::getInstance()->processWmCommand(wParam, lParam);
		break;

	default:
		// Other messages can just be ignored
		break;

	}
	return TRUE;
}


