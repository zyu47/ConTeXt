//Code apapted from WebEdit
//This marks the main module of a program or library.

//	MODULE ConTeXtEditU;

#include "ConTeXtEditU.h"

//	SYSTEM, Win: = Windows, Sci : = Scintilla, Npp : = NotepadPPU, oberonRTS, Str, StrU, Tags,
//	Ver : = ConTeXtEditUVer;


//CRLF = '' + 0DX + 0AX;
//AboutMsg = 'This small freeware plugin allows you to wrap the selected text in tag pairs and expand abbreviations using a hotkey.' + CRLF
//+ 'For more information refer to ' + PluginName + '.txt.' + CRLF
//+ CRLF
//+ 'Created by Alexander Iljin (Amadeus IT Solutions) using XDS Oberon, March 2008 - March 2010.' + CRLF
//+ 'Contact e-mail: AlexIljin@users.SourceForge.net';
//
ConTeXtEditU* ConTeXtEditU::s_contextEditU = NULL;

vector<Pair> ConTeXtEditU::pairs;
HWND ConTeXtEditU::hSci;


//map<char*, char*> ConTeXtEditU::Tags;

ConTeXtEditU* ConTeXtEditU::create(HWND hNotepad, HWND hSCI, HINSTANCE hInst)
{
	if (s_contextEditU == NULL)
		s_contextEditU = new ConTeXtEditU(hNotepad, hSCI, hInst);
	return s_contextEditU;
}

ConTeXtEditU::ConTeXtEditU(HWND hNotepad, HWND hSCI, HINSTANCE hInst): tag_ins(hSCI, hNotepad)
{
	hNpp = hNotepad;
	hSci = hSCI;
	ReadConfig();
	/*
	Funcs[0] = Func00;
	Funcs[1] = Func01;
	Funcs[2] = Func02;
	Funcs[3] = Func03;
	Funcs[4] = Func04;
	Funcs[5] = Func05;
	Funcs[6] = Func06;
	Funcs[7] = Func07;
	Funcs[8] = Func08;
	Funcs[9] = Func09;
	Funcs[10] = Func10;
	Funcs[11] = Func11;
	Funcs[12] = Func12;
	Funcs[13] = Func13;
	Funcs[14] = Func14;
	Funcs[15] = Func15;
	Funcs[16] = Func16;
	Funcs[17] = Func17;
	Funcs[18] = Func18;
	Funcs[19] = Func19;
	Funcs[20] = Func20;
	Funcs[21] = Func21;
	Funcs[22] = Func22;
	Funcs[23] = Func23;
	Funcs[24] = Func24;
	Funcs[25] = Func25;
	Funcs[26] = Func26;
	Funcs[27] = Func27;
	Funcs[28] = Func28;
	Funcs[29] = Func29;
	Funcs[30] = Func30;
	Funcs[31] = Func31;
	Funcs[32] = Func32;
	Funcs[33] = Func33;
	Funcs[34] = Func34;
	Funcs[35] = Func35;
	Funcs[36] = Func36;
	Funcs[37] = Func37;
	Funcs[38] = Func38;
	Funcs[39] = Func39;
	Funcs[40] = Func40;
	Funcs[41] = Func41;
	Funcs[42] = Func42;
	Funcs[43] = Func43;
	Funcs[44] = Func44;
	Funcs[45] = Func45;
	Funcs[46] = Func46;
	Funcs[47] = Func47;
	Funcs[48] = Func48;
	Funcs[49] = Func49;
	Funcs[50] = Func50;
	Funcs[51] = Func51;
	Funcs[52] = Func52;
	Funcs[53] = Func53;
	Funcs[54] = Func54;
	Funcs[55] = Func55;
	Funcs[56] = Func56;
	Funcs[57] = Func57;
	Funcs[58] = Func58;
	Funcs[59] = Func59;
	Funcs[60] = Func60;
	Funcs[61] = Func61;
	Funcs[62] = Func62;
	Funcs[63] = Func63;
	Funcs[64] = Func64;
	Funcs[65] = Func65;
	Funcs[66] = Func66;
	Funcs[67] = Func67;
	Funcs[68] = Func68;
	Funcs[69] = Func69;
	Funcs[70] = Func70;
	Funcs[71] = Func71;
	Funcs[72] = Func72;
	Funcs[73] = Func73;
	Funcs[74] = Func74;
	Funcs[75] = Func75;
	Funcs[76] = Func76;
	Funcs[77] = Func77;
	Funcs[78] = Func78;
	Funcs[79] = Func79;
	Funcs[80] = Func80;
	Funcs[81] = Func81;
	Funcs[82] = Func82;
	Funcs[83] = Func83;
	Funcs[84] = Func84;
	Funcs[85] = Func85;
	Funcs[86] = Func86;
	Funcs[87] = Func87;
	Funcs[88] = Func88;
	Funcs[89] = Func89;
	Funcs[90] = Func90;
	Funcs[91] = Func91;
	Funcs[92] = Func92;
	Funcs[93] = Func93;
	Funcs[94] = Func94;
	Funcs[95] = Func95;
	Funcs[96] = Func96;
	Funcs[97] = Func97;
	Funcs[98] = Func98;
	*/
}

//void ConTeXtEditU::ClearPairs()
//{
//	/*for (int i = 0; i < MaxFuncs; ++i)
//	{
//		pairs[i].name[0] = '\0';
//		pairs[i].left[0] = '\0';
//		pairs[i].right[0] = '\0';
//	}*/
//	
//}

//PROCEDURE LoadBitmap(VAR fname : ARRAY OF Npp.Char) : Win.HBITMAP;
//(*Load a bitmap image from the given file name and return the handle. *)
//BEGIN
//RETURN SYSTEM.VAL(Win.HBITMAP,
//	Win.LoadImageW(
//		NIL, SYSTEM.VAL(Win.PWSTR, SYSTEM.ADR(fname)), Win.IMAGE_BITMAP, 0, 0,
//		Win.LR_DEFAULTSIZE + Win.LR_LOADMAP3DCOLORS + Win.LR_LOADFROMFILE
//	)
//)
//END LoadBitmap;

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
	//Sci.SetSelectionExtent(sc, start, end, bool);
	::SendMessage(hSci, SCI_ENDUNDOACTION, 0, 0);
}

void ConTeXtEditU::ApplyPair(Pair pair)
{
	//(*Surround currently selected text in the current Scintilla view with a pair of tags. *)
	SurroundSelection(pair.left.c_str(), pair.right.c_str());
}

//
//
//PROCEDURE['C'] ReplaceTag();
//(*Replace current tag with a replacement text. *)
//BEGIN
//Tags.Do(Npp.GetCurrentScintilla());
//END ReplaceTag;
//
//PROCEDURE DigitToChar(digit: INTEGER) : Npp.Char;
//(*Return the last decimal digit  *)
//BEGIN
//RETURN(digit MOD 10) + ORD('0')
//END DigitToChar;

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

bool ConTeXtEditU::ReadLine(char* line, DWORD &buffPos, DWORD &buffLen, bool &eof,
	HANDLE &hFile, char* buff, LPDWORD read, const char &commentChar, bool &section)
{
	//TCHAR line[2049];

	/*PROCEDURE ReadLine() : BOOLEAN;
	(*Read until not empty line is read, return TRUE on success.If a new section header is found,
		*return FALSE, line will contain the section string "[section name]", section = TRUE, otherwise
		* section = FALSE.   *)*/
		//CONST Tab = 09X;

	section = FALSE;
	//char ch = ReadChar(buffPos, buffLen, eof, hFile, buff, read);
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
			else if ((line[0] == '[') & (line[i - 1] == ']')) //THEN(*new section *)
				section = TRUE;
		}
	}
	line[i] = '\0';
	//MessageBox(hNpp, CA2T(line),L"muhaha", MB_OK);
	return (i > 0) && !section;
	//return line;
}

void ConTeXtEditU::UnescapeStr(char* str)
{
	/*(*Process line replacing escaped characters with their literal equivalents.
		* Unescaped string is shorter or of equal length, null - terminated. *)*/
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

bool ConTeXtEditU::LineToPair(Pair &pair, char* line)
{
	//(*Initialize pair with data from line, return TRUE on success. *)
	int eqPos, selPos, len;

	eqPos = 0;
	while ((line[eqPos] != '\0') & (line[eqPos] != '='))
		++eqPos;
	if (line[eqPos] == '\0')
	{
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
	//if (strcmp("Ornate Arabic Parens", pair.name) == 0)
	//{
	//	char buff[100];
	//	sprintf_s(buff, "left is: %s\nright is :%s", pair.left, pair.right);
	//	MessageBox(hNpp, CA2T(buff), L"ornate", MB_OK);
	//}

	return TRUE;
}

void ConTeXtEditU::LineToToolbar(char* line, DWORD &maxFnameLen)
{	
	int i = 0;
	int num = 0;
	int len= MaxLineLen;
	//for (; line[len] != '\0'; ++len) {}
	while ((i < len) && (line[i] != '='))
		++i;
	//ASSERT((0 < MaxFuncs) & (MaxFuncs < 100), 20);
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
				//Npp.MenuItemToToolbar(num, LoadBitmap(fname), NIL)
				::SendMessage(hNpp, NPPM_ADDTOOLBARICON, num, reinterpret_cast<LPARAM>(&icons));
			}
		}
	}
}

void ConTeXtEditU::LineToTag(char* line)
{
	//Tag* tag_ins = Tag::getinstance();
	//(*Create new tag with data from line. *)
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
			//key[eqPos] = '\0';
			tag_ins.tags.insert(pair<string, string>(string(key), string(value)));
		}
	}
}

void ConTeXtEditU::ReadConfig()//VAR numRead : INTEGER; initToolbar: BOOLEAN)
{
	//Tag* tag_ins = Tag::getinstance();
	CONST char commentChar = ';';
	char buff[MaxBuffLen];

	//configDir, fname: ARRAY Win.MAX_PATH OF Npp.Char;
	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(IniFileDir));
	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(configDir));
	//strcpy_s(g_configDir, MAX_PATH, WcharMbcsConverter::tchar2char(g_tConfigDir).get());
	//wcstombs(configDir, t_configDir, wcslen(t_configDir) + 1);
	_tcscat_s(IniFileDir, MAX_PATH, _T("\\"));
	_tcscat_s(IniFileDir, MAX_PATH, IniFileName);

	DWORD buffPos = 0, buffLen = 0, configDirLen = 0, maxFnameLen = 0;
	//hFile: Win.HANDLE;
	HANDLE hFile = CreateFile(IniFileDir,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	//TCHAR ch;
	bool eof = FALSE, section = FALSE;

	LPDWORD read = &buffLen;	//VAR read : Win.DWORD;

	tag_ins.tags.clear();
	//ClearPairs();
	pairs.clear();
	groupStarts.clear();
	groupNames.clear();
	//oberonRTS.Collect;
	eof= FALSE;
	buffPos= 0;
	buffLen= 0;
	numRead= 0;
	numGroups = 0;
	//StrU.AppendC(configDir, '\');
	configDirLen = wcslen(configDir);
	maxFnameLen= MAX_PATH;
	//StrU.Copy(configDir, fname);
	//StrU.AppendC(fname, Inif (ileName);
	//hFile= Win.CreateFileW(fname, Win.FILE_READ_DATA, Win.FILE_SHARE_READ,
	//NIL, Win.OPEN_EXISTING, Win.FILE_ATTRIBUTE_NORMAL, NIL);
	char line[MaxLineLen];

	if ((hFile != INVALID_HANDLE_VALUE))
	{
		while (ReadLine(line, buffPos, buffLen, eof,
			hFile, buff, read, commentChar, section)) {
		}
	}

	while (section)
		if (strcmp(line, "[Commands]") == 0)
		{
			while (section) 
			{
				if (strcmp(line, "[Commands]") != 0)
				{
					groupNames.push_back(string(line));
					groupNames[numGroups].erase(groupNames[numGroups].begin(), groupNames[numGroups].begin()+1); //remove [
					groupNames[numGroups].erase(groupNames[numGroups].end()-1, groupNames[numGroups].end()); // remove ]
					groupStarts.push_back(numRead);
					++numGroups;
				}
				while (ReadLine(line, buffPos, buffLen, eof,
					hFile, buff, read, commentChar, section))
				{
					pairs.push_back(Pair());
					if (LineToPair(pairs[numRead], line))
						++numRead;
				}
				/*if (numRead >= MaxFuncs)
					while (ReadLine(line, buffPos, buffLen, eof,
						hFile, buff, read, commentChar, section)) {
					}*/
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



/*
int GetCharPos(VAR str : ARRAY OF Npp.Char; ch: Npp.Char) : INTEGER;
//(*Return index of the first occurence of the ch character in str, -1 if( none found. *)
VAR res : INTEGER;
BEGIN
res : = 0;
while ((str[res] # StrU.Null) & (str[res] # ch))
INC(res)
END;
if (str[res] # ch)
res : = -1;
END;
RETURN res
END GetCharPos;

PROCEDURE MakeDummyFuncName(VAR str : ARRAY OF Npp.Char; pos, num: INTEGER);
//(*Replace characters at pos and (pos + 1) in str with num in decimal notation.
*if (pos < 0, ) nothing. *)
	BEGIN
	ASSERT((0 <= num) & (num < 100), 20);
if (pos >= 0)
str[pos] : = DigitToChar(num DIV 10);
str[pos + 1] : = DigitToChar(num)
END
END MakeDummyFuncName;

PROCEDURE UpdateMenuItems(forShortcutMapper: BOOLEAN);
VAR
i, numPos: INTEGER;
fname: ARRAY Npp.MenuItemNameLength OF Npp.Char;
BEGIN
//(*enable and update text for loaded menu items *)
i : = 0;
while (i < numPairs)
	fname[0] : = 0;
if (forShortcutMapper)
StrU.AppendC(fname, PluginName);
StrU.AppendC(fname, ' - ');
END;
StrU.Append(fname, pairs[i].name^);
Npp.SetMenuItemName(i, fname);
Npp.EnableMenuItem(i, TRUE);
INC(i)
END;
//(*disable and reset text for the rest *)
if (i < MaxFuncs)
	StrU.Assign(NotUsedFuncStr, fname);
numPos: = GetCharPos(fname, NumChar);
REPEAT
MakeDummyFuncName(fname, numPos, i + 1);
Npp.SetMenuItemName(i, fname);
Npp.EnableMenuItem(i, FALSE);
INC(i)
UNTIL i >= MaxFuncs
END
END UpdateMenuItems;
*/
void ConTeXtEditU::EditConfig()
{
	//(*Open ini - file for editing in Notepad++. *)
	::SendMessage(hNpp, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(IniFileDir));
	_tcscat_s(IniFileDir, MAX_PATH, _T("\\"));
	_tcscat_s(IniFileDir, MAX_PATH, IniFileName);
	if (!::SendMessage(hNpp, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(IniFileDir)))
		;
}
//void ConTeXtEditU::LoadConfig();\
//{
//	ReadConfig(numPairs, FALSE);
//	UpdateMenuItems(FALSE)
//		END LoadConfig;
//}
/*
PROCEDURE OnReady();
BEGIN
UpdateMenuItems(FALSE)
END OnReady;

PROCEDURE OnSetInfo();
BEGIN
ReadConfig(numPairs, TRUE);
UpdateMenuItems(TRUE)
END OnSetInfo;

PROCEDURE Init();
CONST
AdditionalMenuItems = 5; //(*Number of item added to MaxFuncs *)
EnterKey = 0DX;
VAR
i, numPos: INTEGER;
funcs: ARRAY MaxFuncs OF Npp.Function;
fname: ARRAY LEN(NotUsedFuncStr) OF Npp.Char;
replaceTagStr: ARRAY LEN(ReplaceTagStr) OF Npp.Char;
editConfigStr: ARRAY LEN(EditConfigStr) OF Npp.Char;
loadConfigStr: ARRAY LEN(LoadConfigStr) OF Npp.Char;
aboutStr: ARRAY LEN(AboutStr) OF Npp.Char;
shortcut: Npp.Shortcut;
BEGIN
if (MaxFuncs + AdditionalMenuItems > Npp.DefNumMenuItems)
Npp.SetNumMenuItems(MaxFuncs + AdditionalMenuItems)
END;
funcs[00] : = Func00;
funcs[01] : = Func01;
funcs[02] : = Func02;
funcs[03] : = Func03;
funcs[04] : = Func04;
funcs[05] : = Func05;
funcs[06] : = Func06;
funcs[07] : = Func07;
funcs[08] : = Func08;
funcs[09] : = Func09;
funcs[10] : = Func10;
funcs[11] : = Func11;
funcs[12] : = Func12;
funcs[13] : = Func13;
funcs[14] : = Func14;
funcs[15] : = Func15;
funcs[16] : = Func16;
funcs[17] : = Func17;
funcs[18] : = Func18;
funcs[19] : = Func19;
funcs[20] : = Func20;
funcs[21] : = Func21;
funcs[22] : = Func22;
funcs[23] : = Func23;
funcs[24] : = Func24;
funcs[25] : = Func25;
funcs[26] : = Func26;
funcs[27] : = Func27;
funcs[28] : = Func28;
funcs[29] : = Func29;
funcs[30] : = Func30;
funcs[31] : = Func31;
funcs[32] : = Func32;
funcs[33] : = Func33;
funcs[34] : = Func34;
funcs[35] : = Func35;
funcs[36] : = Func36;
funcs[37] : = Func37;
funcs[38] : = Func38;
funcs[39] : = Func39;
funcs[40] : = Func40;
funcs[41] : = Func41;
funcs[42] : = Func42;
funcs[43] : = Func43;
funcs[44] : = Func44;
funcs[45] : = Func45;
funcs[46] : = Func46;
funcs[47] : = Func47;
funcs[48] : = Func48;
funcs[49] : = Func49;
funcs[50] : = Func50;
funcs[51] : = Func51;
funcs[52] : = Func52;
funcs[53] : = Func53;
funcs[54] : = Func54;
funcs[55] : = Func55;
funcs[56] : = Func56;
funcs[57] : = Func57;
funcs[58] : = Func58;
funcs[59] : = Func59;
funcs[60] : = Func60;
funcs[61] : = Func61;
funcs[62] : = Func62;
funcs[63] : = Func63;
funcs[64] : = Func64;
funcs[65] : = Func65;
funcs[66] : = Func66;
funcs[67] : = Func67;
funcs[68] : = Func68;
funcs[69] : = Func69;
funcs[70] : = Func70;
funcs[71] : = Func71;
funcs[72] : = Func72;
funcs[73] : = Func73;
funcs[74] : = Func74;
funcs[75] : = Func75;
funcs[76] : = Func76;
funcs[77] : = Func77;
funcs[78] : = Func78;
funcs[79] : = Func79;
funcs[80] : = Func80;
funcs[81] : = Func81;
funcs[82] : = Func82;
funcs[83] : = Func83;
funcs[84] : = Func84;
funcs[85] : = Func85;
funcs[86] : = Func86;
funcs[87] : = Func87;
funcs[88] : = Func88;
funcs[89] : = Func89;
funcs[90] : = Func90;
funcs[91] : = Func91;
funcs[92] : = Func92;
funcs[93] : = Func93;
funcs[94] : = Func94;
funcs[95] : = Func95;
funcs[96] : = Func96;
funcs[97] : = Func97;
funcs[98] : = Func98;
StrU.Assign(PluginName, Npp.PluginName);
Npp.onReady : = OnReady;
Npp.onSetInfo : = OnSetInfo;
StrU.Assign(NotUsedFuncStr, fname);
numPos: = GetCharPos(fname, NumChar);
i: = 0;
while (i < MaxFuncs)
	MakeDummyFuncName(fname, numPos, i + 1);
Npp.AddMenuItem(fname, funcs[i], FALSE, NIL);
INC(i)
END;
NEW(shortcut);
shortcut.alt : = TRUE;
shortcut.key : = EnterKey;
StrU.Assign(ReplaceTagStr, replaceTagStr);
Npp.AddMenuItem(replaceTagStr, ReplaceTag, FALSE, shortcut);
Npp.AddMenuSeparator;
StrU.Assign(EditConfigStr, editConfigStr);
Npp.AddMenuItem(editConfigStr, EditConfig, FALSE, NIL);
StrU.Assign(LoadConfigStr, loadConfigStr);
Npp.AddMenuItem(loadConfigStr, LoadConfig, FALSE, NIL);
StrU.Assign(AboutStr, aboutStr);
Npp.AddMenuItem(aboutStr, About, FALSE, NIL)
END Init;

BEGIN Init
END ConTeXtEditU.
*/