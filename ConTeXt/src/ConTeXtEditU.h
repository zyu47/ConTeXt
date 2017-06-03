#include "StdAfx.h"
#include "PluginInterface.h"
#include "Scintilla.h"
#include "Tag.h"
//class ConTeXtEditU;
struct Tag;

struct Pair
{
	Pair() {}
	//Pair(TCHAR *name, TCHAR* left, TCHAR* right) :name(name), left(left), right(right) {}
	char name[MaxKeyLen];
	char left[500];
	char right[500];
};

class ConTeXtEditU
{
public:

	ConTeXtEditU(HWND hNotePad, HWND hSCI, HINSTANCE hInst);
	static ConTeXtEditU* create(HWND hNotepad, HWND hSCI, HINSTANCE hInst);
	void ClearPairs();
	static void SurroundSelection(char* leftText, char* rightText);
	static void ApplyPair(Pair pair);
	static ConTeXtEditU* getInstance() { return s_contextEditU; }
	void EditConfig();
	
	static void Func00() { ApplyPair(pairs[0]); }
	static void Func01() { ApplyPair(pairs[1]); }
	static void Func02() { ApplyPair(pairs[2]); }
	static void Func03() { ApplyPair(pairs[3]); }
	static void Func04() { ApplyPair(pairs[4]); }
	static void Func05() { ApplyPair(pairs[5]); }
	static void Func06() { ApplyPair(pairs[6]); }
	static void Func07() { ApplyPair(pairs[7]); }
	static void Func08() { ApplyPair(pairs[8]); }
	static void Func09() { ApplyPair(pairs[9]); }
	static void Func10() { ApplyPair(pairs[10]); }
	static void Func11() { ApplyPair(pairs[11]); }
	static void Func12() { ApplyPair(pairs[12]); }
	static void Func13() { ApplyPair(pairs[13]); }
	static void Func14() { ApplyPair(pairs[14]); }
	static void Func15() { ApplyPair(pairs[15]); }
	static void Func16() { ApplyPair(pairs[16]); }
	static void Func17() { ApplyPair(pairs[17]); }
	static void Func18() { ApplyPair(pairs[18]); }
	static void Func19() { ApplyPair(pairs[19]); }
	static void Func20() { ApplyPair(pairs[20]); }
	static void Func21() { ApplyPair(pairs[21]); }
	static void Func22() { ApplyPair(pairs[22]); }
	static void Func23() { ApplyPair(pairs[23]); }
	static void Func24() { ApplyPair(pairs[24]); }
	static void Func25() { ApplyPair(pairs[25]); }
	static void Func26() { ApplyPair(pairs[26]); }
	static void Func27() { ApplyPair(pairs[27]); }
	static void Func28() { ApplyPair(pairs[28]); }
	static void Func29() { ApplyPair(pairs[29]); }
	static void Func30() { ApplyPair(pairs[30]); }
	static void Func31() { ApplyPair(pairs[31]); }
	static void Func32() { ApplyPair(pairs[32]); }
	static void Func33() { ApplyPair(pairs[33]); }
	static void Func34() { ApplyPair(pairs[34]); }
	static void Func35() { ApplyPair(pairs[35]); }
	static void Func36() { ApplyPair(pairs[36]); }
	static void Func37() { ApplyPair(pairs[37]); }
	static void Func38() { ApplyPair(pairs[38]); }
	static void Func39() { ApplyPair(pairs[39]); }
	static void Func40() { ApplyPair(pairs[40]); }
	static void Func41() { ApplyPair(pairs[41]); }
	static void Func42() { ApplyPair(pairs[42]); }
	static void Func43() { ApplyPair(pairs[43]); }
	static void Func44() { ApplyPair(pairs[44]); }
	static void Func45() { ApplyPair(pairs[45]); }
	static void Func46() { ApplyPair(pairs[46]); }
	static void Func47() { ApplyPair(pairs[47]); }
	static void Func48() { ApplyPair(pairs[48]); }
	static void Func49() { ApplyPair(pairs[49]); }
	static void Func50() { ApplyPair(pairs[50]); }
	static void Func51() { ApplyPair(pairs[51]); }
	static void Func52() { ApplyPair(pairs[52]); }
	static void Func53() { ApplyPair(pairs[53]); }
	static void Func54() { ApplyPair(pairs[54]); }
	static void Func55() { ApplyPair(pairs[55]); }
	static void Func56() { ApplyPair(pairs[56]); }
	static void Func57() { ApplyPair(pairs[57]); }
	static void Func58() { ApplyPair(pairs[58]); }
	static void Func59() { ApplyPair(pairs[59]); }
	static void Func60() { ApplyPair(pairs[60]); }
	static void Func61() { ApplyPair(pairs[61]); }
	static void Func62() { ApplyPair(pairs[62]); }
	static void Func63() { ApplyPair(pairs[63]); }
	static void Func64() { ApplyPair(pairs[64]); }
	static void Func65() { ApplyPair(pairs[65]); }
	static void Func66() { ApplyPair(pairs[66]); }
	static void Func67() { ApplyPair(pairs[67]); }
	static void Func68() { ApplyPair(pairs[68]); }
	static void Func69() { ApplyPair(pairs[69]); }
	static void Func70() { ApplyPair(pairs[70]); }
	static void Func71() { ApplyPair(pairs[71]); }
	static void Func72() { ApplyPair(pairs[72]); }
	static void Func73() { ApplyPair(pairs[73]); }
	static void Func74() { ApplyPair(pairs[74]); }
	static void Func75() { ApplyPair(pairs[75]); }
	static void Func76() { ApplyPair(pairs[76]); }
	static void Func77() { ApplyPair(pairs[77]); }
	static void Func78() { ApplyPair(pairs[78]); }
	static void Func79() { ApplyPair(pairs[79]); }
	static void Func80() { ApplyPair(pairs[80]); }
	static void Func81() { ApplyPair(pairs[81]); }
	static void Func82() { ApplyPair(pairs[82]); }
	static void Func83() { ApplyPair(pairs[83]); }
	static void Func84() { ApplyPair(pairs[84]); }
	static void Func85() { ApplyPair(pairs[85]); }
	static void Func86() { ApplyPair(pairs[86]); }
	static void Func87() { ApplyPair(pairs[87]); }
	static void Func88() { ApplyPair(pairs[88]); }
	static void Func89() { ApplyPair(pairs[89]); }
	static void Func90() { ApplyPair(pairs[90]); }
	static void Func91() { ApplyPair(pairs[91]); }
	static void Func92() { ApplyPair(pairs[92]); }
	static void Func93() { ApplyPair(pairs[93]); }
	static void Func94() { ApplyPair(pairs[94]); }
	static void Func95() { ApplyPair(pairs[95]); }
	static void Func96() { ApplyPair(pairs[96]); }
	static void Func97() { ApplyPair(pairs[97]); }
	static void Func98() { ApplyPair(pairs[98]); }

	void ReadConfig();//VAR numRead : INTEGER; initToolbar: BOOLEAN);
	char ReadChar(DWORD &buffPos, DWORD &buffLen, bool &eof, HANDLE &hFile, char* buff, LPDWORD read);
	bool ReadLine(char* line, DWORD &buffPos, DWORD &buffLen, bool &eof,
		HANDLE &hFile, char* buff, LPDWORD read, const char &commentChar, bool &section);
	void UnescapeStr(char* str);
	bool LineToPair(Pair &pair, char* line);
	void LineToToolbar(char* line, DWORD &maxFnameLen);
	void LineToTag(char* line);
	void(*Funcs[MaxFuncs])();

	void replaceTag() { //Tag* tag_ins = Tag::getinstance();
		tag_ins.replace(); }


	static Pair pairs[MaxFuncs];
	size_t numRead;

private:
	static ConTeXtEditU* s_contextEditU;
	static HWND hSci;
	HWND hNpp;

	bool initToolbar;

	std::string PluginName = "ConTeXt";
	//static const int MaxFuncs = 99; //(*0 < MaxFuncs < 100 * )
	//NumChar = ORD('X'); //(*the placeholder character for a number in NotUsedFuncStr *)
	std::string NotUsedFuncStr = "ConTeXtEdit Slot XX";
	std::string ReplaceTagStr = "Replace Tag";
	std::string EditConfigStr = "Edit Config";
	std::string LoadConfigStr = "Load Config";
	std::string AboutStr = "About...";

	std::string CommandsIniSection = "[Commands]";
	std::string ToolbarIniSection = "[Toolbar]";
	std::string TagsIniSection = "[Tags]";
	//std::string IniFileName = PluginName + ".ini";
	std::string AboutMsg = "By Jason Yu";
	
	
	Tag tag_ins;

	//char configDir[MAX_PATH];
	TCHAR configDir[MAX_PATH];
	TCHAR IniFileDir[MAX_PATH];
	TCHAR IniFileName[100] = TEXT("ConTeXtEdit.ini");

};