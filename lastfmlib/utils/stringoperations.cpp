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

#include "stringoperations.h"
#include <cassert>

using namespace std;

namespace StringOperations {

void replace(std::string& aString, const std::string& toSearch, const std::string& toReplace)
{
    size_t startPos = 0;
    size_t foundPos;

    while (std::string::npos != (foundPos = aString.find(toSearch, startPos))) {
        aString.replace(foundPos, toSearch.length(), toReplace);
        startPos = foundPos + toReplace.size();
    }
}

void dos2unix(std::string& aString)
{
    replace(aString, "\r\n", "\n");
}

string urlEncode(const string& aString)
{
    stringstream result;

    for (char i : aString) {
        auto curChar = static_cast<int>(static_cast<unsigned char>(i));
        if ((curChar >= 48 && curChar <= 57) || (curChar >= 65 && curChar <= 90) || (curChar >= 97 && curChar <= 122) || i == '-' || i == '_' || i == '.' || i == '!' || i == '~' || i == '*' || i == '\'' || i == '(' || i == ')') {
            result << i;
        } else if (i == ' ') {
            result << '+';
        } else {
            result << '%' << hex << curChar;
        }
    }

    return result.str();
}

vector<string> tokenize(const string& str, const string& delimiter)
{
    vector<string> tokens;
    string tempString = str;
    size_t pos = 0;

    while ((pos = tempString.find(delimiter)) != string::npos) {
        tokens.push_back(tempString.substr(0, pos));
        tempString.erase(0, pos + delimiter.size());
    }
    tokens.push_back(tempString);

    return tokens;
}

void wideCharToUtf8(const wstring& wideString, string& utf8String)
{
    size_t stringLength = wcstombs(nullptr, wideString.c_str(), 0);
    utf8String.resize(stringLength + 1);

    size_t len = wcstombs(&utf8String[0], wideString.c_str(), stringLength + 1);
    if (len == static_cast<size_t>(-1)) {
        throw logic_error("Failed to convert wideString to UTF-8");
    }

    utf8String.resize(stringLength);
}

void utf8ToWideChar(const string& utf8String, wstring& wideString)
{
    size_t stringLength = mbstowcs(nullptr, utf8String.c_str(), 0);
    wideString.resize(stringLength + 1);

    size_t len = mbstowcs(&wideString[0], utf8String.c_str(), stringLength + 1);
    if (len == static_cast<size_t>(-1)) {
        throw logic_error("Failed to convert wideString to UTF-8");
    }

    wideString.resize(stringLength);
}

} // namespace StringOperations
