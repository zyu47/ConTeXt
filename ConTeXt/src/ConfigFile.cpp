#include "StdAfx.h"

#include "atlstr.h"
#include "ConfigFile.h"
#include "resource.h"
#include "WcharMbcsConverter.h"
#include "MenuManager.h"
#include "WcharMbcsConverter.h"
#include "ConfigFile.h"
#include "PluginInterface.h"
//#include "StaticIDAllocator.h"
//#include "NppAllocator.h"
#include "Notepad_plus_msgs.h"
#include "DynamicIDManager.h"

ConfigFile* ConfigFile::s_instance;

ConfigFile* ConfigFile::create(const TCHAR *configDir, const TCHAR *pluginDir, HINSTANCE hInst)
{
	ConfigFile::s_instance = new ConfigFile(configDir, pluginDir, hInst);
	return ConfigFile::s_instance;
}


ConfigFile::ConfigFile(const TCHAR *configDir, const TCHAR *pluginDir, HINSTANCE hInst)
	: m_hInst (hInst),
      m_configFilename(configDir),
	  m_pluginDir(pluginDir),
	  m_machineScriptsDir(pluginDir),
	  m_userScriptsDir(configDir),
	  m_configDir(configDir)
{
	m_configFilename.append(_T("\\PythonScriptStartup.cnf"));
	
	m_machineScriptsDir.append(_T("\\PythonScript\\scripts"));
	m_userScriptsDir.append(_T("\\PythonScript\\scripts"));

	readConfig();
}


ConfigFile::~ConfigFile()
{
	m_hInst = NULL;
	// TODO: Clean up 
	// DeleteImage
	// 
	
}
void ConfigFile::readConfig()
{
	/*for (int i = 0; i != 3; ++i)
	{
		const char* s = std::to_string(i).c_str();
		TCHAR szName[512];
		_tcscpy(szName, CA2W(s));
		m_menuItems.push_back(tstring(szName));
	}*/
}
/*
void ConfigFile::readConfig()
{
	std::basic_ifstream<TCHAR> startupFile(m_configFilename.c_str());
	
	TCHAR buffer[500];
	
	
	HBITMAP hIcon;

	while (startupFile.good())
	{
		startupFile.getline(buffer, 500);
		TCHAR *context;
		TCHAR *element = _tcstok_s(buffer, _T("/"), &context);
		if (element)
		{

			// Menu item
			if (0 == _tcscmp(element, _T("ITEM")))
			{
				element = _tcstok_s(NULL, _T("/"), &context);
				m_menuItems.push_back(tstring(element));
				m_menuScripts.push_back(std::string(WcharMbcsConverter::tchar2char(element).get()));
			}
		
			// Toolbar item
			else if (0 == _tcscmp(element, _T("TOOLBAR")))
			{
				element = _tcstok_s(NULL, _T("/"), &context);
				TCHAR *iconPath = _tcstok_s(NULL, _T("/"), &context);
				if (!iconPath || !(*iconPath))
				{
					hIcon = static_cast<HBITMAP>(LoadImage(m_hInst, MAKEINTRESOURCE(IDB_PYTHON), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));
					iconPath = NULL;
				}
				else 
				{
					hIcon = static_cast<HBITMAP>(LoadImage(NULL, iconPath, IMAGE_BITMAP, 16, 16, LR_LOADMAP3DCOLORS | LR_LOADFROMFILE));
				}

			
				m_toolbarItems.push_back(std::pair<tstring, std::pair<HBITMAP, tstring> >(tstring(element), std::pair<HBITMAP, tstring>(hIcon, iconPath ? tstring(iconPath) : tstring())));
			}
			else if (0 == _tcscmp(element, _T("SETTING")))
			{
				element = _tcstok_s(NULL, _T("/"), &context);
				TCHAR *settingValue = _tcstok_s(NULL, _T("/"), &context);
				m_settings.insert(std::pair<tstring, tstring>(tstring(element), tstring(settingValue)));
			}
		}

	}
	startupFile.close();
}

void ConfigFile::clearItems()
{
	m_menuItems.erase(m_menuItems.begin(), m_menuItems.end());
	m_menuScripts.erase(m_menuScripts.begin(), m_menuScripts.end());
	m_toolbarItems.erase(m_toolbarItems.begin(), m_toolbarItems.end());
}

void ConfigFile::save()
{
	std::basic_ofstream<TCHAR> startupFile(m_configFilename.c_str(), std::ios_base::out | std::ios_base::trunc);
	for(MenuItemsTD::iterator it = m_menuItems.begin(); it != m_menuItems.end(); ++it)
	{
		startupFile << "ITEM/" << (*it) << "\n";
	}

	for(ToolbarItemsTD::iterator it = m_toolbarItems.begin(); it != m_toolbarItems.end(); ++it)
	{
		startupFile << _T("TOOLBAR/") << it->first << _T("/") << it->second.second << _T("\n");
	}

	for(SettingsTD::iterator it = m_settings.begin(); it != m_settings.end(); ++it)
	{
		startupFile << _T("SETTING/") << it->first << _T("/") << it->second << _T("\n");
	}

	startupFile.close();

}


void ConfigFile::addMenuItem(const tstring scriptPath)
{
	m_menuItems.push_back(scriptPath);
	m_menuScripts.push_back(std::string(WcharMbcsConverter::tchar2char(scriptPath.c_str()).get()));
}

void ConfigFile::addToolbarItem(const tstring scriptPath, const tstring iconPath)
{
	HBITMAP hIcon = static_cast<HBITMAP>(LoadImage(m_hInst, iconPath.c_str(), IMAGE_BITMAP, 16, 16, LR_LOADMAP3DCOLORS));
	m_toolbarItems.push_back(std::pair<tstring, std::pair<HBITMAP, tstring> >(scriptPath, std::pair<HBITMAP, tstring>(hIcon, iconPath)));
}

void ConfigFile::setSetting(const tstring& settingName, const tstring settingValue)
{
	m_settings[settingName] = settingValue;
}

const tstring& ConfigFile::getSetting(const TCHAR *settingName)
{
	return m_settings[tstring(settingName)];
}

const std::string& ConfigFile::getMenuScript(idx_t index) const
{ 
	if (m_menuScripts.size() > index)
	{
		return m_menuScripts[index]; 
	}
	else
	{
		return m_emptyString;
	}
}
*/