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

#ifndef ConTeXt_LEXER_H_
#define ConTeXt_LEXER_H_
#define MAX_MENU_SCRIPTS 50

#include "Scintilla.h"
#include "ILexer.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"
#include "StyleContext.h"
#include "CharacterSet.h"

namespace ConTeXt {

class ConTeXt final :
	public ILexer
{
public:
	ConTeXt() {}
	virtual ~ConTeXt() {}

	static ILexer* LexerFactory();

	inline bool isOperator(const char ch);
	int ParseTeXCommand(unsigned int pos, Accessor &styler, char *command);
	int classifyFoldPointTeXPaired(const char* s);
	int classifyFoldPointTeXUnpaired(const char* s);
	bool IsTeXCommentLine(int line, Accessor &styler);

	// ILexer
	void SCI_METHOD Release() override;
	int SCI_METHOD Version() const override;
	const char* SCI_METHOD PropertyNames() override;
	int SCI_METHOD PropertyType(const char* name) override;
	const char* SCI_METHOD DescribeProperty(const char* name) override;
	int SCI_METHOD PropertySet(const char* key, const char* val) override;
	const char* SCI_METHOD DescribeWordListSets() override;
	int SCI_METHOD WordListSet(int n, const char* wl) override;
	void SCI_METHOD Lex(unsigned int startPos, int length, int initStyle, IDocument* pAccess) override;
	void SCI_METHOD Fold(unsigned int startPos, int length, int initStyle, IDocument* pAccess) override;
	void* SCI_METHOD PrivateCall(int operation, void* pointer) override;	

private:
	enum TextState
	{
		TS_DEFAULT,
		TS_COMMENT,
		//TS_SECTION,
		TS_KEYWORD,
		//TS_OPTION,
		TS_VALUE,
		//TS_BANG,
		//TS_VARIABLE,
		TS_LINEEND
	};
	
	enum TextColor
	{
		TC_DEFAULT,
		TC_COMMENT,
		//TC_SECTION,
		TC_KEYWORD0,
		TC_KEYWORD1,
		TC_KEYWORD2,
		TC_KEYWORD3,
		TC_KEYWORD4,
		TC_OPERATOR,
		TC_NUMBER,
		TC_FOLDER_OPEN,
		TC_FOLDER_CLOSE
		//TC_EQUALS,
		//TC_INVALID,
		//TC_VALID,
		//TC_BANG,
		//TC_INTVARIABLE,
		//TC_EXTVARIABLE
	};

	WordList m_WordLists[7];
};

}	// namespace ConTeXt

#endif
