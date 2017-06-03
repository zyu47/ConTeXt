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

#ifndef ConTeXt_STDAFX_H_
#define ConTeXt_STDAFX_H_

#include <Windows.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <memory>
#include <atlstr.h>

typedef  std::basic_string<TCHAR>	tstring;
typedef size_t idx_t;

#define IDX_MAX SIZE_MAX
#define MaxKeyLen 50
#define MaxValueLen 1024
#define MaxBuffLen 1024
#define MaxLineLen 2049
#define MaxFuncs 99
#define MaxIndent 80
#define KeyNotFoundMsgLen 40

#define FuncsGroupNo 3

#endif
