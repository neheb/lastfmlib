#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

#include "../utils/stringoperations.h"

using std::string;
using std::wstring;
using std::vector;

using namespace StringOperations;

TEST(StringOperationsTest, Dos2Unix)
{
    string testString = "abcde\r\nfgs\r\r\n";
    dos2unix(testString);
    EXPECT_EQ("abcde\nfgs\r\n", testString);

    testString = "\r\n";
    dos2unix(testString);
    EXPECT_EQ("\n", testString);

    testString = "teststring";
    dos2unix(testString);
    EXPECT_EQ("teststring", testString);
}

TEST(StringOperationsTest, Replace)
{
    string testString = "abcaabbabbab";
    replace(testString, "ab", "a");
    EXPECT_EQ("acaababa", testString);

    testString = "stringstringstring";
    replace(testString, "stringstring", "string");
    EXPECT_EQ("stringstring", testString);
}

TEST(StringOperationsTest, Tokenize)
{
    string testString = "A-B-C";
    vector<string> tokenized;
    tokenized = tokenize(testString, "-");
    EXPECT_EQ(3, tokenized.size());
    EXPECT_EQ("A", tokenized[0]);
    EXPECT_EQ("B", tokenized[1]);
    EXPECT_EQ("C", tokenized[2]);

    testString = "A_*_B_*_C";
    tokenized = tokenize(testString, "_*_");
    EXPECT_EQ(3, tokenized.size());
    EXPECT_EQ("A", tokenized[0]);
    EXPECT_EQ("B", tokenized[1]);
    EXPECT_EQ("C", tokenized[2]);

    testString = "A_*_B_*_C";
    tokenized = tokenize(testString, "_**_");
    EXPECT_EQ(1, tokenized.size());
    EXPECT_EQ("A_*_B_*_C", tokenized[0]);
}

TEST(StringOperationsTest, ConvertToUtf8)
{
    string utf8String;
    wideCharToUtf8(L"Teststring", utf8String);
    EXPECT_EQ("Teststring", utf8String);

    wideCharToUtf8(L"Trentemøller", utf8String);
    EXPECT_EQ("Trentemøller", utf8String);
}

TEST(StringOperationsTest, ConvertToWidechar)
{
    wstring wideString;
    utf8ToWideChar("Teststring", wideString);
    EXPECT_TRUE(wstring(L"Teststring") == wideString);

    utf8ToWideChar("Trentemøller", wideString);
    EXPECT_TRUE(wstring(L"Trentemøller") == wideString);
}

TEST(StringOperationsTest, UrlEncode)
{
    EXPECT_EQ("!%40%23%24%25%5e%26*()fsdkjh+", urlEncode("!@#$%^&*()fsdkjh "));
    EXPECT_EQ("Trentem%c3%b8ller", urlEncode("Trentemøller"));
}
