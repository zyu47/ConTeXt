#include "StdAfx.h"
#include "Lexer.h"

namespace ConTeXt {

ILexer* ConTeXt::LexerFactory()
{
	return new ConTeXt();
}

/// Determine if a character is considered as a ConTeXt operator
inline bool ConTeXt::isOperator(char ch) {
	if (ch >= 0x80 || isalnum(ch)) {
		return false;
	}
	if (ch == '*' || ch == '/' || ch == '-' ||
		ch == '+' || ch == '(' || ch == ')' ||
		ch == '=' || ch == '{' || ch == '}' ||
		ch == '~' || ch == '[' || ch == ']' ||
		ch == ';' || ch == '<' || ch == '>' ||
		ch == '.' ||
		ch == '^' || ch == ':' || ch == '#' ||
		ch == '&' || ch == '|' )
		return true;
	return false;
}

/// Once a backslash is met, continue reading the following word
int ConTeXt::ParseTeXCommand(unsigned int pos, Accessor &styler, char *command)
{
	int length = 0;
	char ch = styler.SafeGetCharAt(pos + 1);

	if (ch == ',' || ch == ':' || ch == ';' || ch == '%') {
		command[0] = '\\';
		command[1] = ch;
		command[2] = 0;
		return 1;
	}
	command[length] = '\\';
	length++;
	// find end
	while (isalpha(ch) && !IsADigit(ch) && ch != '_' && ch != '.' && length<100) {
		command[length] = ch;
		length++;
		ch = styler.SafeGetCharAt(pos + length);
	}

	command[length] = '\0';
	length--;
	if (!length) return 0;
	return length + 1;
}

bool ConTeXt::IsTeXCommentLine(int line, Accessor &styler) {
	int pos = styler.LineStart(line);
	int eol_pos = styler.LineStart(line + 1) - 1;

	int startpos = pos;

	while (startpos<eol_pos) {
		char ch = styler[startpos];
		if (ch != '%' && ch != ' ') return false;
		else if (ch == '%') return true;
		startpos++;
	}

	return false;
}

//
// ILexer
//

void SCI_METHOD ConTeXt::Release() {
	delete this;
}

int SCI_METHOD ConTeXt::Version() const {
	return lvOriginal;
}

const char* SCI_METHOD ConTeXt::PropertyNames() {
	return "";
}

int SCI_METHOD ConTeXt::PropertyType(const char* name) {
	return SC_TYPE_BOOLEAN;
}

const char* SCI_METHOD ConTeXt::DescribeProperty(const char* name) {
	return "";
}

int SCI_METHOD ConTeXt::PropertySet(const char* key, const char* val) {
	return -1;
}

const char* SCI_METHOD ConTeXt::DescribeWordListSets() {
	return "";
}

int SCI_METHOD ConTeXt::WordListSet(int n, const char *wl) {
	if (n < _countof(m_WordLists)) {
		WordList wlNew;
		wlNew.Set(wl);
		if (m_WordLists[n] != wlNew) {
			m_WordLists[n].Set(wl);
			return 0;
		}
	}
	return -1;
}

void SCI_METHOD ConTeXt::Lex(unsigned int startPos, int length, int initStyle, IDocument* pAccess)
{
	Accessor styler(pAccess, nullptr);

	const WordList& keywords0 = m_WordLists[0];
	const WordList& keywords1 = m_WordLists[1];
	const WordList& keywords2 = m_WordLists[2];
	const WordList& keywords3 = m_WordLists[3];
	const WordList& keywords4 = m_WordLists[4];
	const WordList& keywords5 = m_WordLists[5]; //folder in code 2, open
	const WordList& keywords6 = m_WordLists[6]; //folder in code 2, close

	length += startPos;
	styler.StartAt(startPos);
	styler.StartSegment(startPos);

	int state = TS_DEFAULT;
	char* name = nullptr;
	int count = 0;
	char buffer[128];
	int i = startPos;
//	int digits = 0;
	for (; i <= length ; ++i)
	{
		// Make ch 0 if at EOF.
		char ch = (i == length) ? '\0' : styler[i];

		// Amount of EOL chars is 2 (\r\n) with the Windows format and 1 (\n) with Unix format.
		int chEOL = (styler[i] == '\n' && styler[i - 1] == '\r') ? 2 : 1;

		switch(state)
		{
		case TS_DEFAULT:
			switch (ch)
			{
			case '\0':
			case '\n':
				styler.ColourTo(i, TC_DEFAULT);
				break;

			case '%':
				styler.ColourTo(i-1, TC_DEFAULT);
				state = TS_COMMENT;
				break;

			case '\\':
				styler.ColourTo(i - 1, TC_DEFAULT);
				buffer[0] = '\\';
				count = 1;
				state = TS_KEYWORD;
				break;
				
			default:
				if (isOperator(ch) || isdigit(ch)) {
					styler.ColourTo(i - 1, TC_DEFAULT);
					state = TS_VALUE;
				}
			}
			break;

		case TS_COMMENT:
			// Style as comment when EOL (or EOF) is reached
			switch (ch)
			{
			case '\0':
			case '\n':
				state = TS_DEFAULT;
				styler.ColourTo(i - chEOL, TC_COMMENT);
				styler.ColourTo(i, TC_DEFAULT);
			}
			break;

		case TS_KEYWORD:
			// Read max. 32 chars into buffer until the equals sign (or EOF/EOL) is met.
			if (!isalpha(ch)) {
				buffer[count] = '\0';
				if (keywords0.InList(buffer))
					styler.ColourTo(i - 1, TC_KEYWORD0);
				else if (keywords1.InList(buffer))
					styler.ColourTo(i - 1, TC_KEYWORD1);
				else if (keywords2.InList(buffer))
					styler.ColourTo(i - 1, TC_KEYWORD2);
				else if (keywords3.InList(buffer))
					styler.ColourTo(i - 1, TC_KEYWORD3);
				else if (keywords4.InList(buffer))
					styler.ColourTo(i - 1, TC_KEYWORD4);
				else if (keywords5.InList(buffer))
					styler.ColourTo(i - 1, TC_FOLDER_OPEN);
				else if (keywords6.InList(buffer))
					styler.ColourTo(i - 1, TC_FOLDER_CLOSE);
				else
					styler.ColourTo(i - 1, TC_DEFAULT);
				--i;
				state = TS_DEFAULT;
			}
			else if (count < 128)
					buffer[count++] = ch;
			else
					state = TS_DEFAULT;
			break;

		case TS_VALUE:
			// Read values to highlight variables and bangs
			if (!isOperator(ch) && !isdigit(ch)) {
				styler.ColourTo(i - 1, TC_OPERATOR);
				--i;
				state = TS_DEFAULT;
			}
			break;

		case TS_LINEEND:
			// Apply default style when EOL (or EOF) is reached
			switch (ch)
			{
			case '\0':
			case '\n':
				state = TS_DEFAULT;
				styler.ColourTo(i, TC_DEFAULT);
			}
			break;
		}
	}
	styler.Flush();
}

void SCI_METHOD ConTeXt::Fold(unsigned int startPos, int length, int initStyle, IDocument* pAccess)
{
	
	Accessor styler(pAccess, nullptr);

	const WordList& keywords5 = m_WordLists[5]; //folder in code 2, open
	const WordList& keywords6 = m_WordLists[6]; //folder in code 2, close

	unsigned int endPos = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	char buffer[101] = "";

	for (unsigned int i = startPos; i < endPos; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (ch == '%')
		{
			i = styler.LineStart(lineCurrent + 1) - 1;
			++visibleChars;
			atEOL = TRUE;
			ch = '\n';
			chNext = styler.SafeGetCharAt(i + 1);
		}

		if (ch == '\\') {
			ParseTeXCommand(i, styler, buffer);
			if (keywords5.InList(buffer))
				levelCurrent++; 
			else if (keywords6.InList(buffer))
				levelCurrent--;
		}

		bool foldComment = TRUE;

		if (foldComment && atEOL && IsTeXCommentLine(lineCurrent, styler))
		{
			if (lineCurrent == 0 && IsTeXCommentLine(lineCurrent + 1, styler)
				)
				levelCurrent++;
			else if (lineCurrent != 0 && !IsTeXCommentLine(lineCurrent - 1, styler)
				&& IsTeXCommentLine(lineCurrent + 1, styler)
				)
				levelCurrent++;
			else if (lineCurrent != 0 && IsTeXCommentLine(lineCurrent - 1, styler) &&
				!IsTeXCommentLine(lineCurrent + 1, styler))
				levelCurrent--;
		}
		
		//---------------------------------------------------------------------------------------------

		if (atEOL) {
			int lev = levelPrev;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}

		if (isalnum(ch))
			visibleChars++;
	}

	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);

}

void* SCI_METHOD ConTeXt::PrivateCall(int operation, void* pointer) {
	return nullptr;
}

//
// Scintilla exports
//

int SCI_METHOD GetLexerCount()
{
	return 1;
}

void SCI_METHOD GetLexerName(unsigned int index, char* name, int buflength)
{
	strncpy(name, "ConTeXt", buflength);
	name[buflength - 1] = '\0';
}

void SCI_METHOD GetLexerStatusText(unsigned int index, WCHAR* desc, int buflength)
{
	//wcsncpy(desc, L"ConTeXt skin file", buflength);
	wcsncpy(desc, L"ConTeXt file", buflength);
	desc[buflength - 1] = L'\0';
}

LexerFactoryFunction SCI_METHOD GetLexerFactory(unsigned int index)
{
	return (index == 0) ? ConTeXt::LexerFactory : nullptr;
}

}	// namespace ConTeXt
