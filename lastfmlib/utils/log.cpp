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

//#include "log.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ENABLE_LOGGING
#include <syslog.h>
#endif

#include <iostream>
#include <sstream>
#include <string>

namespace Log {

static constexpr std::string_view red = "\033[31m";
static constexpr std::string_view green = "\033[32m";
static constexpr std::string_view yellow = "\033[33m";
static constexpr std::string_view purple = "\033[35m";
static constexpr std::string_view standard = "\033[39m";

static inline void outputInfo([[maybe_unused]] const std::string& message)
{
#ifdef ENABLE_LOGGING
    syslog(LOG_INFO, "%s", message.c_str());
#endif

#ifdef ENABLE_DEBUG
    std::cout << green << "INFO:  " << message << standard << std::endl;
#endif
}

template <typename T1>
void info(const T1& t1)
{
    std::stringstream ss;
    ss << t1;

    outputInfo(ss.str());
}

template <typename T1, typename T2>
void info(const T1& t1, const T2& t2)
{
    std::stringstream ss;
    ss << t1 << " " << t2;

    outputInfo(ss.str());
}

template <typename T1, typename T2, typename T3>
void info(const T1& t1, const T2& t2, const T3& t3)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3;

    outputInfo(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4>
void info(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4;

    outputInfo(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void info(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5;

    outputInfo(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void info(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6;

    outputInfo(ss.str());
}

static inline void outputWarn(const std::string& message)
{
#ifdef ENABLE_LOGGING
    syslog(LOG_WARNING, "%s", message.c_str());
#endif

    std::cout << yellow << "WARN:  " << message << standard << std::endl;
}

template <typename T1>
void warn(const T1& t1)
{
    std::stringstream ss;
    ss << t1;

    outputWarn(ss.str());
}

template <typename T1, typename T2>
void warn(const T1& t1, const T2& t2)
{
    std::stringstream ss;
    ss << t1 << " " << t2;

    outputWarn(ss.str());
}

template <typename T1, typename T2, typename T3>
void warn(const T1& t1, const T2& t2, const T3& t3)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3;

    outputWarn(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4>
void warn(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4;

    outputWarn(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void warn(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5;

    outputWarn(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void warn(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6;

    outputWarn(ss.str());
}

static inline void outputCritical(const std::string& message)
{
#ifdef ENABLE_LOGGING
    syslog(LOG_CRIT, "%s", message.c_str());
#endif

    std::cerr << purple << "CRIT:  " << message << standard << std::endl;
}

template <typename T1>
void critical(const T1& t1)
{
    std::stringstream ss;
    ss << t1;

    outputCritical(ss.str());
}

template <typename T1, typename T2>
void critical(const T1& t1, const T2& t2)
{
    std::stringstream ss;
    ss << t1 << " " << t2;

    outputCritical(ss.str());
}

template <typename T1, typename T2, typename T3>
void critical(const T1& t1, const T2& t2, const T3& t3)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3;

    outputCritical(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4>
void critical(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4;

    outputCritical(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void critical(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5;

    outputCritical(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void critical(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6;

    outputCritical(ss.str());
}

static inline void outputError(const std::string& message)
{
#ifdef ENABLE_LOGGING
    syslog(LOG_ERR, "%s", message.c_str());
#endif

    std::cerr << red << "ERROR: " << message << standard << std::endl;
}

template <typename T1>
void error(const T1& t1)
{
    std::stringstream ss;
    ss << t1;

    outputError(ss.str());
}

template <typename T1, typename T2>
void error(const T1& t1, const T2& t2)
{
    std::stringstream ss;
    ss << t1 << " " << t2;

    outputError(ss.str());
}

template <typename T1, typename T2, typename T3>
void error(const T1& t1, const T2& t2, const T3& t3)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3;

    outputError(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4>
void error(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4;

    outputError(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void error(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5;

    outputError(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void error(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6;

    outputError(ss.str());
}

static inline void outputDebug([[maybe_unused]] const std::string& message)
{
#ifdef ENABLE_LOGGING
    syslog(LOG_DEBUG, "%s", message.c_str());
#endif

#ifdef ENABLE_DEBUG
    std::cout << "DEBUG: " << message << std::endl;
#endif
}

template <typename T1>
void debug(const T1& t1)
{
    std::stringstream ss;
    ss << t1;

    outputDebug(ss.str());
}

template <typename T1, typename T2>
void debug(const T1& t1, const T2& t2)
{
    std::stringstream ss;
    ss << t1 << " " << t2;

    outputDebug(ss.str());
}

template <typename T1, typename T2, typename T3>
void debug(const T1& t1, const T2& t2, const T3& t3)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3;

    outputDebug(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4>
void debug(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4;

    outputDebug(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void debug(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5;

    outputDebug(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
void debug(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6;

    outputDebug(ss.str());
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void debug(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7)
{
    std::stringstream ss;
    ss << t1 << " " << t2 << " " << t3 << " " << t4 << " " << t5 << " " << t6 << " " << t7;

    outputDebug(ss.str());
}

} // namespace Log
