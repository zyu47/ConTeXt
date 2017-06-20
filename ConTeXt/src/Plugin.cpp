/*
  Copyright (C) 2010-2012 Birunthan Mohanathas <http://poiru.net>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "StdAfx.h"
#include "PluginInterface.h"
#include "Version.h"
#include "Menumanager.h"
#include "ContextMenu.h"
//#include "ConfigFile.h"
#include "ConTeXtEditU.h"
#include "Tag.h"

const int WM_QUERY_ConTeXt = WM_APP + 1000;
const int ConTeXt_QUERY_ID_SKINS_PATH = 4101;

static FuncItem	*funcItem = NULL;
static NppData nppData;
static HANDLE		g_hModule = NULL;

static char  g_pluginDir[MAX_PATH];
static char  g_configDir[MAX_PATH];
static TCHAR g_tPluginDir[MAX_PATH];
static TCHAR g_tConfigDir[MAX_PATH];

static bool g_infoSet = false;

HWND g_ConTeXtWindow = nullptr;
HWND g_NppWindow = nullptr;
HWND g_SCI = nullptr;
//WCHAR g_SkinsPath[MAX_PATH] = {0};
/*
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_COPYDATA)
	{
		COPYDATASTRUCT* cds = (COPYDATASTRUCT*)lParam;
		if (cds->dwData == ConTeXt_QUERY_ID_SKINS_PATH)
		{
			wcsncpy(g_SkinsPath, (const WCHAR*)cds->lpData, _countof(g_SkinsPath));
			g_SkinsPath[_countof(g_SkinsPath) - 1] = L'\0';
		}

		return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}*/
/*
bool GetConTeXt()
{
	if (!g_ConTeXtWindow || !IsWindow(g_ConTeXtWindow))
	{
		HWND trayWindow = FindWindow(L"ConTeXtTrayClass", nullptr);
		HWND meterWindow = FindWindow(L"ConTeXtMeterWindow", nullptr);
		if (trayWindow && meterWindow)
		{
			// Create window to recieve WM_COPYDATA from ConTeXt
			HWND wnd = CreateWindow(
				L"STATIC",
				L"",
				WS_DISABLED,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				nullptr,
				nullptr,
				nullptr,
				nullptr);

			if (wnd)
			{
				SetWindowLongPtr(wnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

				SendMessage(trayWindow, WM_QUERY_ConTeXt, ConTeXt_QUERY_ID_SKINS_PATH, (LPARAM)wnd);
				DestroyWindow(wnd);

				if (*g_SkinsPath)
				{
					g_ConTeXtWindow = meterWindow;
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

void RefreshSkin()
{
	if (!GetConTeXt()) return;

	WCHAR currentPath[MAX_PATH];
	BOOL ret = (BOOL)SendMessage(g_NppWindow, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)&currentPath);

	if (ret)
	{
		const size_t skinsPathLen = wcslen(g_SkinsPath);
		const size_t currentPathLen = wcslen(currentPath);

		// Make sure the file is in the skins folder and that extension is .ini
		if (wcsncmp(g_SkinsPath, currentPath, skinsPathLen) == 0 &&
			_wcsicmp(&currentPath[currentPathLen - 4], L".ini") == 0)
		{
			WCHAR* relativePath = &currentPath[skinsPathLen];
			WCHAR* pos = wcsrchr(relativePath, L'\\');
			if (pos)
			{
				relativePath[pos - relativePath] = L'\0';
				WCHAR buffer[512];
				const int len = _snwprintf(
					buffer, _countof(buffer), L"!Refresh \"%s\"", relativePath);
				buffer[_countof(buffer) - 1] = L'\0';

				COPYDATASTRUCT cds;
				cds.dwData = 1;
				cds.cbData = (DWORD)(len + 1) * sizeof(WCHAR);
				cds.lpData = (void*)buffer;
				SendMessage(g_ConTeXtWindow, WM_COPYDATA, 0, (LPARAM)&cds);
			}
		}
	}
}

void RefreshAll()
{
	if (!GetConTeXt()) return;

	COPYDATASTRUCT cds;
	cds.dwData = 1;
	cds.cbData = sizeof(L"!Refresh *");
	cds.lpData = L"!Refresh *";
	SendMessage(g_ConTeXtWindow, WM_COPYDATA, 0, (LPARAM)&cds);
}
*/
void About()
{
	MessageBox(
		g_NppWindow,
		L"By Jason Yu.\n"
		L"GitHub zyu47",
		ConTeXt_TITLE,
		MB_OK);
}

//
// Notepad++ exports
//

//static HANDLE		g_hModule = NULL;
//BOOL APIENTRY DllMain(HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID /* lpReserved */
//)
//{
//	g_hModule = hModule;
//	switch (ul_reason_for_call)
//	{
//	case DLL_PROCESS_ATTACH:
//		break;
//
//	case DLL_THREAD_ATTACH:
//		break;
//
//	case DLL_THREAD_DETACH:
//		break;
//
//	case DLL_PROCESS_DETACH:
//		break;
//
//	}
//	return TRUE;
//}

//#ifdef _UNICODE
//extern "C" __declspec(dllexport) BOOL isUnicode()
BOOL isUnicode()
{
	return TRUE;
}
//#endif

//extern "C" __declspec(dllexport) const TCHAR * getName()
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
	//stopScriptIndex = items.size() - 1;

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("Edit config"), editConfig));
	items.push_back(pair<tstring, void(*)()>(_T("Load config"), loadConfig));

	//runPreviousIndex = items.size() - 1;

	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	//scriptsMenuIndex = items.size() - 1;
	items.push_back(pair<tstring, void(*)()>(_T("Update Context Menu"), updateContextMenu));
	items.push_back(pair<tstring, void(*)()>(_T("Remove Context Menu"), removeContextMenu));
	items.push_back(pair<tstring, void(*)()>(_T("Edit Context XML"), editContextXML));
	items.push_back(pair<tstring, void(*)()>(_T("--"), reinterpret_cast<void(*)()>(NULL)));
	items.push_back(pair<tstring, void(*)()>(_T("About"), About));
	//dynamicStartIndex = 0; //items.size() - 1;

	FuncItem* funcItems = menuManager->getFuncItemArray(nbF, items);


	return funcItems;

}


//extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *count)
FuncItem * getFuncsArray(int *count) 
{
	//static FuncItem funcItems[] =
	//{
	//	{ _T("--"), 0, 0, false, nullptr},
	//	{ _T("Replace tags"), replace, 1, false, nullptr },
	//	{ _T("--"), 0, 2, false, nullptr },
	//	//{ L"Refresh all", RefreshAll, 0, false, nullptr },
	//	//{ L"&Menu1", menu1, 0, false, nullptr},
	//	{ _T("Edit config"), editConfig, 3, false, nullptr },
	//	{ _T("--"), 0, 4, false, nullptr },
	//	{ _T("About"), About,5, false, nullptr }
	//};

	//*count = _countof(funcItems);

	//return funcItems;
	funcItem = getGeneratedFuncItemArray(count);
	return funcItem;

}

//static void runScript(const char *filename, bool synchronous, HANDLE completedEvent /* = NULL */, bool allowQueuing /* = false */) {}

//extern "C" __declspec(dllexport) void setInfo(NppData data)
void setInfo(NppData data)
{
	nppData = data;
	g_NppWindow = data._nppHandle;
	
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return;
	HWND g_SCI = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;


	// Get the two key directories (plugins config and the Npp dir)
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(g_tConfigDir));
	//strcpy_s(g_configDir, MAX_PATH, WcharMbcsConverter::tchar2char(g_tConfigDir).get());
	wcstombs(g_configDir, g_tConfigDir, wcslen(g_tConfigDir) + 1);

	::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, reinterpret_cast<LPARAM>(g_tPluginDir));
	_tcscat_s(g_tPluginDir, MAX_PATH, _T("\\plugins"));
	//strcpy_s(g_pluginDir, MAX_PATH, WcharMbcsConverter::tchar2char(g_tPluginDir).get());
	wcstombs(g_pluginDir, g_tPluginDir, wcslen(g_tPluginDir) + 1);

	//ConfigFile::create(g_tConfigDir, g_tPluginDir, reinterpret_cast<HINSTANCE>(g_hModule));
	ConTeXtEditU::create(nppData._nppHandle, g_SCI, reinterpret_cast<HINSTANCE>(g_hModule));
	MenuManager::create(nppData._nppHandle, reinterpret_cast<HINSTANCE>(g_hModule));// , runScript);

	//Tag::create(nppData._nppHandle);
	g_infoSet = true;
}

//extern "C" __declspec(dllexport) void beNotified(SCNotification *scn)
void beNotified(SCNotification *scn) 
{
	switch (scn->nmhdr.code)
	{
		case NPPN_READY:
		{
			MenuManager* menuManager = MenuManager::getInstance();
			menuManager->idsInitialised();
			menuManager->populateScriptsMenu();
			//menuManager->idsInitialised();
			//menuManager->stopScriptEnabled(false);
			//menuManager->initPreviousScript();
		}
		break;

		case SCN_CHARADDED:
			break;
			//MessageBox(0, L"char added", L"muhaha", MB_OK);

		case NPPN_SHUTDOWN:
		{
			ConTeXtEditU::deleteInstance();
			MenuManager::deleteInstance();
		}
		break;
	}
}

//extern "C" __declspec(dllexport) LRESULT messageProc(UINT uMsg, WPARAM wParam, LPARAM lParam)

LRESULT messageProc(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

	//char *s = new char[100];
	//sprintf(s, "My variable is %d\n", uMsg);
	//wchar_t wtext[20];
	//mbstowcs(wtext, s, strlen(s) + 1);//Plus null
	//LPWSTR ptr = wtext;
	//for (int i = 0; i <= 100; ++i)
	////	OutputDebugString(ptr);

	//wchar_t buffer[256];
	//wsprintfW(buffer, L"%d", uMsg);
	//if (uMsg != 3 && uMsg != 5)
	//	MessageBox(0, buffer, L"ConTeXt", MB_OK);

	switch (uMsg)
	{
	case WM_COMMAND:
		//MenuManager::getInstance()->processWmCommand(wParam, lParam);
		//char tmp[100];
		//sprintf_s(tmp, "identifier: %d", wParam);
		//MessageBox(0, CA2T(tmp), L"muhaha",MB_OK);
		MenuManager::getInstance()->processWmCommand(wParam, lParam);
		break;

	default:
		// Other messages can just be ignored
		break;

	}
	return TRUE;
}


