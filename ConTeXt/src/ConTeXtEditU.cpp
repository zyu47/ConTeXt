///Code apapted from WebEdit

#include "ConTeXtEditU.h"

///static instance
ConTeXtEditU* ConTeXtEditU::s_contextEditU = NULL;

vector<Pair> ConTeXtEditU::pairs;
HWND ConTeXtEditU::hSci;

/// Create an static instance of ConTeXtEditU
ConTeXtEditU* ConTeXtEditU::create(HWND hNotepad, HWND hSCI)
{
	if (s_contextEditU == NULL)
		s_contextEditU = new ConTeXtEditU(hNotepad, hSCI);
	return s_contextEditU;
}

ConTeXtEditU::ConTeXtEditU(HWND hNotepad, HWND hSCI): tag_ins(hSCI, hNotepad)
{
	hNpp = hNotepad;
	hSci = hSCI;
	ReadConfig();
}

/// Insert pairs around all selections
void ConTeXtEditU::SurroundSelection(const char* leftText, const char* rightText)
{
	::SendMessage(hSci, SCI_BEGINUNDOACTION, 0, 0);
	int numSel = static_cast<int32_t>(::SendMessage(hSci, SCI_GETSELECTIONS, 0, 0));
	for (int i = 0; i < numSel; ++i) 
	{
		size_t stpos = ::SendMessage(hSci, SCI_GETSELECTIONNSTART, i, 0);
		size_t endpos = ::SendMessage(hSci, SCI_GETSELECTIONNEND, i, 0);
		::SendMessage(hSci, SCI_INSERTTEXT, endpos, reinterpret_cast<LPARAM>(rightText));
		::SendMessage(hSci, SCI_INSERTTEXT, stpos, reinterpret_cast<LPARAM>(leftText));
		
	}
	::SendMessage(hSci, SCI_ENDUNDOACTION, 0, 0);
}

/// Surround currently selected text in the current Scintilla view with a pair of tags.
void ConTeXtEditU::ApplyPair(Pair pair)
{
	SurroundSelection(pair.left.c_str(), pair.right.c_str());
}

/// Read character by character from the ini file
char ConTeXtEditU::ReadChar(DWORD &buffPos, DWORD &buffLen, bool &eof,
							HANDLE &hFile, char* buff, LPDWORD read)
{

	if (buffPos >= buffLen)
	{	
		buffPos = 0;
		eof = !ReadFile(hFile, buff, MaxBuffLen, read, NULL);
		eof = eof || (buffLen == 0) || (buffLen > MaxBuffLen);
	}
	char ch = '\0';
	if (!eof)
	{
		ch = buff[buffPos];
		++buffPos;
	}
	return ch;
}

/// Parse every line from the ini file
/// Return False if a new tag was identified
bool ConTeXtEditU::ReadLine(char* line, DWORD &buffPos, DWORD &buffLen, bool &eof,
	HANDLE &hFile, char* buff, LPDWORD read, const char &commentChar, bool &section)
{
	section = FALSE;
	int i = 0;
	bool eol = FALSE;
	while (!eof && !section && i <= 0)
	{
		i = 0; 
		eol = FALSE;
		char ch = ReadChar(buffPos, buffLen, eof, hFile, buff, read);
		while (!eof && !eol && (i < MaxLineLen - 1))
		{
			if (static_cast<unsigned char>(ch) < 32 && ch != '\t')
			{
				eol = TRUE;
			}
			else
			{
				line[i] = ch;
				++i;
				ch = ReadChar(buffPos, buffLen, eof, hFile, buff, read);
			}
		}

		if ((i == MaxLineLen - 1) && !eol && !eof) //(*line too long *)
		{
			i = 0;
			eof = TRUE;
		}
		else if (i > 0)
		{
			if (line[0] == commentChar) //(*comment *)
				i = 0;
			else if ((line[0] == '[') & (line[i - 1] == ']')) //(* found a new section *)
				section = TRUE;
		}
	}
	line[i] = '\0';
	return (i > 0) && !section;
}

/* Process line replacing escaped characters with their literal equivalents.
* Unescaped string is shorter or of equal length, null - terminated. */
void ConTeXtEditU::UnescapeStr(char* str)
{
	string res;
	int i = 0;
	int c = 0;

	while (str[i] != '\0')
	{
		if ((str[i]) == '\\')
		{
			switch (str[i + 1])
			{
			case 't':
				str[c] = '\t';
				++i;
				break;
			case 'n':
				str[c] = '\n';
				++i;
				break;
			case 'r':
				str[c] = '\r';
				++i;
				break;
			case '\\':
				str[c] = '\\';
				++i;
				break;
			default:
				str[c] = str[i];
				break;
			}
		}
		else
			str[c] = str[i];
		++i;
		++c;
	}
	str[c] = '\0';
}

/* Initialize pair with data from line, return TRUE on success.
*/
bool ConTeXtEditU::LineToPair(Pair &pair, char* line)
{
	int eqPos, selPos, len;

	eqPos = 0;
	while ((line[eqPos] != '\0') & (line[eqPos] != '='))
		++eqPos;
	if (line[eqPos] == '\0')
	{
		// If line is '---', add a separator between submenus
		if (Constants::SEP.compare(line) == 0)
		{
			pair.name = line;
			return TRUE;
		}
		return FALSE;
	}

	selPos = eqPos + 1;
	while ((line[selPos] != '\0') & (line[selPos] != '|'))
		++selPos;
	if (line[selPos] == '\0')
		return FALSE;
	len = selPos + 1;
	while (line[len] != '\0')
		++len;

	char name[MaxKeyLen];
	char left[MaxValueLen];
	char right[MaxValueLen];
	strncpy_s(name, line, eqPos);
	name[eqPos] = '\0';
	strncpy_s(left, line+eqPos+1, selPos - eqPos - 1);
	left[selPos- eqPos -1] = '\0';
	strncpy_s(right, line + selPos + 1, len - selPos);
	right[len - selPos] = '\0';
	
	UnescapeStr(left);
	UnescapeStr(right);
	
	pair.name = name;
	pair.left = left;
	pair.right = right;

	return TRUE;
}

/// Initialize toolbar with data from line. NEED TO BE BETTER IMPLEMENTED. 
void ConTeXtEditU::LineToToolbar(char* line, DWORD &maxFnameLen)
{	
	int i = 0;
	int num = 0;
	int len= MaxLineLen;
	while ((i < len) && (line[i] != '='))
		++i;
	if ((i > 0) && (i <= 2)
		&& isdigit(line[0]) && ((i = 1) || (isdigit(line[1])))) //(*2 digits maximum *)
	{
		num = line[0] - '0';
		if (i = 2)
			num = num * 10 + line[1] - '0';

		TCHAR bar_img_dir[MAX_PATH];
		_tcscpy_s(bar_img_dir, MAX_PATH, configDir);
		TCHAR fname[MAX_PATH];
		if ((0 < num) && (num <= numRead))
		{
			--num; //(*items are numbered from 1, in ini - file *)
			++i; //(*skip '=' *)
			TCHAR tline[MAX_PATH];
			_tcscpy(tline, CA2T(line));
			if ((i < len) && (len - i <= maxFnameLen))
			{
				_tcsncpy(fname, tline + i, len - i);
				_tcscat_s(bar_img_dir, _T("\\"));
				_tcscat_s(bar_img_dir, fname);
				HBITMAP bitmap = LoadBitmap(NULL, bar_img_dir);
				toolbarIcons icons;
				icons.hToolbarBmp = bitmap;
				icons.hToolbarIcon = NULL;
				::SendMessage(hNpp, NPPM_ADDTOOLBARICON, num, reinterpret_cast<LPARAM>(&icons));
			}
		}
	}
}

/// Parse line into tags
/// Detailed tag implementation can be found in Tag class
void ConTeXtEditU::LineToTag(char* line)
{
	int eqPos, len;
	char key[MaxKeyLen];
	char value[MaxValueLen];

	eqPos = 0;
	while((line[eqPos] != '\0') && line[eqPos] != '=')
		++eqPos;
	
	if ((line[eqPos] != '\0') && (eqPos <= MaxKeyLen))
	{
		len = eqPos + 1;
		while (line[len] != '\0')
			++len;
		if (len > eqPos + 1)
		{
			strncpy_s(value, line + eqPos + 1, len - eqPos);
			strncpy_s(key, line, eqPos);
			tag_ins.tags.insert(pair<string, string>(string(key), string(value)));
		}
	}
}

/// Read ini file
void ConTeXtEditU::ReadConfig()
{
	CONST char commentChar = ';';
	char buff[MaxBuffLen];

	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(IniFileDir));
	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(configDir));
	_tcscat_s(IniFileDir, MAX_PATH, _T("\\"));
	_tcscat_s(IniFileDir, MAX_PATH, IniFileName);

	DWORD buffPos = 0, buffLen = 0, configDirLen = 0, maxFnameLen = 0;
	HANDLE hFile = CreateFile(IniFileDir,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	bool eof = FALSE, section = FALSE;
	LPDWORD read = &buffLen;

	// Initialize all the important information
	tag_ins.tags.clear(); // Remove all exisiting tags
	pairs.clear(); // Remove all existing pairs
	groupStarts.clear();
	groupNames.clear();
	eof= FALSE;
	buffPos= 0;
	buffLen= 0;
	numRead= 0;
	numGroups = 0;

	configDirLen = wcslen(configDir);
	maxFnameLen= MAX_PATH;
	char line[MaxLineLen];

	//Read until the first non-comment line
	if ((hFile != INVALID_HANDLE_VALUE)) 
	{
		while (ReadLine(line, buffPos, buffLen, eof,
			hFile, buff, read, commentChar, section)) {
		}
	}

	//Start parsing the ini file
	while (section)
		if (strcmp(line, "[Commands]") == 0)
		{
			// Under [Commands], each section is a subsection
			while (section) 
			{
				if (strcmp(line, "[Commands]") != 0)
				{
					groupNames.push_back(string(line));
					groupNames[numGroups].erase(groupNames[numGroups].begin(), groupNames[numGroups].begin()+1); //remove '[' 
					groupNames[numGroups].erase(groupNames[numGroups].end()-1, groupNames[numGroups].end()); // remove ']'
					groupStarts.push_back(numRead); //The start index of the next group
					++numGroups;
				}
				while (ReadLine(line, buffPos, buffLen, eof,
					hFile, buff, read, commentChar, section))
				{
					pairs.push_back(Pair());
					if (LineToPair(pairs[numRead], line))
						++numRead;
				}
				// Pairs end when [CommandsEnd] is reached
				if (strcmp(line, "[CommandsEnd]") == 0)
				{
					groupStarts.push_back(numRead);
					break;
				}
			}
		}
		else if (initToolbar &&	strcmp(line, "[Toolbar]") == 0)
		{
			//(*read toolbar items *)
			while (ReadLine(line, buffPos, buffLen, eof,
				hFile, buff, read, commentChar, section))
				LineToToolbar(line, maxFnameLen);
		}
		else if (strcmp(line, "[Tags]") == 0)
		{
			//(*read tags *)
			while (ReadLine(line, buffPos, buffLen, eof,
				hFile, buff, read, commentChar, section))
				LineToTag(line);
		}
		else
			while (ReadLine(line, buffPos, buffLen, eof,
				hFile, buff, read, commentChar, section)){
			}
	CloseHandle(hFile);
}

/// Open ini file for editing in Notepad++. 
void ConTeXtEditU::EditConfig()
{
	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(IniFileDir));
	_tcscat_s(IniFileDir, MAX_PATH, _T("\\"));
	_tcscat_s(IniFileDir, MAX_PATH, IniFileName);
	::SendMessage(hNpp, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(IniFileDir));
}