//    Copyright (C) 2009 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

#include <sstream>
#include <vector>

namespace StringOperations {
void replace(std::string& aString, const std::string& toSearch, const std::string& toReplace);
void dos2unix(std::string& aString);
std::string urlEncode(const std::string& aString);
std::vector<std::string> tokenize(std::string_view str, std::string_view delimiter);
void wideCharToUtf8(const std::wstring& wideString, std::string& utf8String);
void utf8ToWideChar(const std::string& utf8String, std::wstring& wideString);
} // namespace StringOperations

#endif
