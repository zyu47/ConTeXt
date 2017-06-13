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
#include "Lexer.h"

namespace ConTeXt {

ILexer* ConTeXt::LexerFactory()
{
	return new ConTeXt();
}

inline bool ConTeXt::isOperator(char ch) {
	if (ch >= 0x80 || isalnum(ch)) {
		return false;
	}

	if (ch == '*' || ch == '/' || ch == '-' ||
		ch == '+' || ch == '(' || ch == ')' ||
		ch == '=' || ch == '{' || ch == '}' ||
		ch == '~' || ch == '[' || ch == ']' ||
		ch == ';' || ch == '<' || ch == '>' ||
		//ch == ',' ||
		ch == '.' ||
		ch == '^' || ch == ':' || ch == '#' ||
		ch == '&' || ch == '|'
		//|| ch == '!'
		)
		return true;
	return false;
}


int ConTeXt::ParseTeXCommand(unsigned int pos, Accessor &styler, char *command)
{
	int length = 0;
	char ch = styler.SafeGetCharAt(pos + 1);

	if (ch == ',' || ch == ':' || ch == ';' || ch == '%') {
		command[0] = ch;
		command[1] = 0;
		return 1;
	}

	// find end
	while (isalpha(ch) && !IsADigit(ch) && ch != '_' && ch != '.' && length<100) {
		command[length] = ch;
		length++;
		ch = styler.SafeGetCharAt(pos + length + 1);
	}

	command[length] = '\0';
	if (!length) return 0;
	return length + 1;
}

int ConTeXt::classifyFoldPointTeXPaired(const char* s) {
	int lev = 0;
	if (!(isdigit(s[0]) || (s[0] == '.'))) {
		if (strcmp(s, "autostarttext") == 0 || strncmp(s, "begin", 5) == 0 || strcmp(s, "begstrut") == 0 ||
			strcmp(s, "dostarttagged") == 0 || strcmp(s, "globalpushreferenceprefix") == 0 ||
			strncmp(s, "push", 4) == 0 || strcmp(s, "savecolor") == 0 || strcmp(s, "setbuffer") == 0 ||			
			strncmp(s, "start", 5) == 0 || strcmp(s, "unprotect") == 0 //|| strncmp(s, "Start", 5) == 0 ||
			//strcmp(s, "documentclass") == 0 || strncmp(s, "if", 2) == 0
			)
			lev = 1;
		if (strcmp(s, "autostoptext") == 0 || strcmp(s, "dostoptagged") == 0 ||strncmp(s, "end", 3) == 0 ||
			strcmp(s, "globalpopreferenceprefix") == 0 || strncmp(s, "pop", 3) == 0 ||
			strcmp(s, "protect") == 0 || strcmp(s, "restorecolor") == 0 ||
			strncmp(s, "stop", 4) == 0 //|| strncmp(s, "Stop", 4) == 0 ||
			)
			lev = -1;
	}
	return lev;
}

int ConTeXt::classifyFoldPointTeXUnpaired(const char* s) {
	int lev = 0;
	/*
	if (!(isdigit(s[0]) || (s[0] == '.'))) {
		if (//strcmp(s, "part") == 0 ||
			strcmp(s, "chapter") == 0 ||
			strcmp(s, "section") == 0 ||
			strcmp(s, "subsection") == 0 ||
			strcmp(s, "subsubsection") == 0 ||
			//strcmp(s, "CJKfamily") == 0 ||
			//strcmp(s, "appendix") == 0 ||
			//strcmp(s, "Topic") == 0 || strcmp(s, "topic") == 0 ||
			strcmp(s, "subject") == 0 || strcmp(s, "subsubject") == 0 ||
			//strcmp(s, "def") == 0 || strcmp(s, "gdef") == 0 || strcmp(s, "edef") == 0 ||
			//strcmp(s, "xdef") == 0 ||
			strcmp(s, "framed") == 0 //||
			//strcmp(s, "frame") == 0 ||
			//strcmp(s, "foilhead") == 0 || strcmp(s, "overlays") == 0 || strcmp(s, "slide") == 0
			) {
			lev = 1;
		}
	}*/
	return lev;
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
	/*
	for (int i = 0; i != 30; ++i) {
		const char* p = keywords0.words[i];
		//wchar_t buffer[256];
		//wsprintfW(buffer, L"%d", a);
		//OutputDebugString(buffer);
		wchar_t wtext[20];
		mbstowcs(wtext, p, strlen(p) + 1);//Plus null
										  //mbstowcs(wtext, s, strlen(s) + 1);
		LPWSTR ptr = wtext;
		OutputDebugString(ptr);
	}
	*/
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

			//case '[':
			//	state = TS_SECTION;
			//	break;

			case '%':
				styler.ColourTo(i-1, TC_DEFAULT);
				state = TS_COMMENT;
				break;
				/*
			case '\t':
			case ' ':
				break;
				*/
			case '\\':
				styler.ColourTo(i - 1, TC_DEFAULT);
				buffer[0] = '\\';
				count = 1;
				state = TS_KEYWORD;
				break;
				
			default:
				/*
				if (isalpha(ch) || ch == '\\')
				{
					count = 0;
					digits = 0;
					buffer[count++] = tolower(ch);
					state = TS_KEYWORD;
				}
				else
				{
					state = TS_VALUE;
				}
				*/
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

		/*
		case TS_SECTION:
			// Style as section when EOL (or EOF) is reached unless section name has a space
			switch (ch)
			{
			case '\0':
			case '\n':
				state = TS_DEFAULT;
				styler.ColourTo(i - chEOL, TC_SECTION);
				styler.ColourTo(i, TC_DEFAULT);
			}
			break;
		*/

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
/*
		case TS_OPTION:
			// Read value into buffer and check if it's valid for cases like StringAlign=RIGHT
			switch (ch)
			{
			case '#':
				count = 0;
				styler.ColourTo(i - 1, TC_DEFAULT);
				state = TS_VARIABLE;
				break;

			case '\0':
				// Read the last character if at EOF
				buffer[count++] = tolower(styler[i++]);

			case '\r':
			case '\n':
				while (isspacechar(buffer[count - 1]))
				{
					--count;
				}

				buffer[count] = '\0';
				count = 0;
				state = TS_DEFAULT;

				if (options.InList(buffer))
				{
					styler.ColourTo(i - chEOL, TC_VALID);
				}
				else
				{
					styler.ColourTo(i - chEOL, TC_INVALID);
				}
				styler.ColourTo(i, TC_DEFAULT);
				break;

			default:
				if (count < _countof(buffer))
				{
					buffer[count++] = tolower(ch);
				}
				else
				{
					state = TS_LINEEND;
				}
			}
			break;
			*/
			
		case TS_VALUE:
			// Read values to highlight variables and bangs
			if (!isOperator(ch) && !isdigit(ch)) {
				styler.ColourTo(i - 1, TC_OPERATOR);
				--i;
				state = TS_DEFAULT;
			}
			break;
/*
		case TS_BANG:
			// Highlight bangs
			switch (ch)
			{
			case '\0':
				buffer[count++] = tolower(styler[i++]);

			case '\n':
			case ' ':
			case '[':
			case ']':
				buffer[count] = '\0';
				count = 0;
				state = (ch == '\n') ? TS_DEFAULT : TS_VALUE;

				// Skip ConTeXt before comparing the bang
				if (bangs.InList(&buffer[(strncmp(buffer, "ConTeXt", 9) == 0) ? 9 : 0]))
				{
					styler.ColourTo(i - chEOL, TC_BANG);
				}
				styler.ColourTo(i, TC_DEFAULT);
				break;

			case '\r':
				break;

			case '#':
				count = 0;
				styler.ColourTo(i - 1, TC_DEFAULT);
				state = TS_VARIABLE;
				break;

			default:
				if (count < _countof(buffer))
				{
					buffer[count++] = tolower(ch);
				}
				else
				{
					state = TS_VALUE;
				}
			}
			break;
			*/
			/*
		case TS_VARIABLE:
			// Highlight variables
			switch (ch)
			{
			case '\n':
				state = TS_DEFAULT;
				styler.ColourTo(i, TC_DEFAULT);
				break;

			case '\0':
			case '#':
				if (count)
				{
					buffer[count] = '\0';

					if (variables.InList(buffer))
					{
						styler.ColourTo(i, TC_INTVARIABLE);
					}
					else
					{
						if (buffer[0] == '*' && buffer[count - 1] == '*')
						{
							// Escaped variable, don't highlight
							styler.ColourTo(i, TC_DEFAULT);
						}
						else
						{
							styler.ColourTo(i, TC_EXTVARIABLE);
						}
					}

					count = 0;
				}
				state = TS_VALUE;
				break;

			case ' ':
			case '[':
			case ']':
				state = TS_VALUE;
				break;

			default:
				if (count < _countof(buffer))
				{
					buffer[count++] = tolower(ch);
				}
				else
				{
					state = TS_VALUE;
				}
			}
			break;
			*/
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

	//bool foldCompact = TRUE;//styler.GetPropertyInt("fold.compact", 1) != 0;
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
			levelCurrent += classifyFoldPointTeXPaired(buffer) + classifyFoldPointTeXUnpaired(buffer);
		}

		if (levelCurrent > SC_FOLDLEVELBASE && ((ch == '\r' || ch == '\n') && (chNext == '\\'))) {
			ParseTeXCommand(i + 1, styler, buffer);
			levelCurrent -= classifyFoldPointTeXUnpaired(buffer);
		}
		/*
		char chNext2;
		char chNext3;
		char chNext4;
		char chNext5;
		chNext2 = styler.SafeGetCharAt(i + 2);
		chNext3 = styler.SafeGetCharAt(i + 3);
		chNext4 = styler.SafeGetCharAt(i + 4);
		chNext5 = styler.SafeGetCharAt(i + 5);

		bool atEOfold = (ch == '%') &&
			(chNext == '%') && (chNext2 == '}') &&
			(chNext3 == '}') && (chNext4 == '-') && (chNext5 == '-');

		bool atBOfold = (ch == '%') &&
			(chNext == '%') && (chNext2 == '-') &&
			(chNext3 == '-') && (chNext4 == '{') && (chNext5 == '{');

		if (atBOfold) {
			levelCurrent += 1;
		}

		if (atEOfold) {
			levelCurrent -= 1;
		}
		
		if (ch == '\\' && chNext == '[') {
			levelCurrent += 1;
		}

		if (ch == '\\' && chNext == ']') {
			levelCurrent -= 1;
		}
		*/
		
		bool foldComment = TRUE;// styler.GetPropertyInt("fold.comment") != 0;

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
			//if (visibleChars == 0 && foldCompact)
			//	lev |= SC_FOLDLEVELWHITEFLAG;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}

		//if (!isspacechar(ch))
		if (isalnum(ch))
			visibleChars++;
	}

	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);

	//const WordList& keywords5 = m_WordLists[5]; //folder in code 2, open
	//const WordList& keywords6 = m_WordLists[6]; //folder in code 2, close

	//length += startPos;
	//int line = styler.GetLine(startPos);

	//int count = 0;
	//char buffer[128];
	////bool startKeyword = 0;
	//int lev = SC_FOLDLEVELBASE;

	//for (unsigned int i = startPos, isize = (unsigned int)length; i < isize; ++i)
	//{
	//	char ch = (i == length - 1) ? '\0' : styler[i];

	//	if (count == 0)
	//	{
	//		if (ch == '\n' || ch == '\0') {
	//			styler.SetLevel(line, lev);
	//			++line;
	//		}
	//		else if(ch == '\\' && (i == startPos || styler[i - 1] == '\n'))
	//			buffer[count++] = '\\';
	//	}
	//	else
	//	{
	//		if (!isalpha(styler[i])) {
	//			buffer[count] = '\0';
	//			if (keywords5.InList(buffer))
	//			{
	//				lev |= SC_FOLDLEVELHEADERFLAG;
	//				styler.SetLevel(line, lev);

	//				lev |= SC_FOLDLEVELBASE;
	//				++lev;
	//				++line;
	//				i = styler.LineStart(line) - 1;
	//			}
	//			else if (keywords6.InList(buffer)) {
	//				--lev;
	//				styler.SetLevel(line, lev);
	//				++line;
	//				i = styler.LineStart(line) - 1;
	//			}
	//			count = 0;
	//		}
	//		else
	//			buffer[count++] = ch;
	//	}
	//}

	//styler.Flush();
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
	wcsncpy(desc, L"ConTeXt skin file", buflength);
	desc[buflength - 1] = L'\0';
}

LexerFactoryFunction SCI_METHOD GetLexerFactory(unsigned int index)
{
	return (index == 0) ? ConTeXt::LexerFactory : nullptr;
}

}	// namespace ConTeXt
