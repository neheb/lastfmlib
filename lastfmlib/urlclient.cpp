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

#include "urlclient.h"

#include <cassert>
#include <curl/curl.h>
#include <stdexcept>

using namespace std;

size_t receiveData(char* data, size_t size, size_t nmemb, string* pBuffer);

UrlClient::UrlClient()
{
#ifdef WIN32
    CURLcode rc = curl_global_init(CURL_GLOBAL_WIN32 | CURL_GLOBAL_ALL);
#else
    CURLcode rc = curl_global_init(CURL_GLOBAL_ALL);
#endif

    if (CURLE_OK != rc) {
        throw std::logic_error("Failed to initialize libcurl");
    }
}

UrlClient::~UrlClient()
{
    curl_global_cleanup();
}

void UrlClient::setProxy(const std::string& server, uint32_t port, const std::string& username, const std::string& password)
{
    if (server.empty()) {
        m_ProxyServer.clear();
        m_ProxyUserPass.clear();
        return;
    }

    m_ProxyServer = server + ':' + std::to_string(port);

    if (!username.empty() && !password.empty()) {
        m_ProxyUserPass = username + ':' + password;
    } else {
        m_ProxyUserPass.clear();
    }
}

void UrlClient::get(const string& url, string& response) const
{
    CURL* curlHandle = curl_easy_init();
    assert(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveData);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    if (!m_ProxyServer.empty()) {
        curl_easy_setopt(curlHandle, CURLOPT_PROXY, m_ProxyServer.c_str());
    }

    if (!m_ProxyUserPass.empty()) {
        curl_easy_setopt(curlHandle, CURLOPT_PROXYUSERPWD, m_ProxyUserPass.c_str());
    }

    CURLcode rc = curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);

    if (CURLE_OK != rc) {
        throw std::logic_error("Failed to get " + url + ": " + curl_easy_strerror(rc));
    }
}

void UrlClient::post(const string& url, const string& data, string& response)
{
    CURL* curlHandle = curl_easy_init();
    assert(curlHandle);

    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, receiveData);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlHandle, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(curlHandle, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curlHandle, CURLOPT_NOSIGNAL, 1);

    CURLcode rc = curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);

    if (CURLE_OK != rc) {
        throw std::logic_error("Failed to post " + url + ": " + curl_easy_strerror(rc));
    }
}

size_t receiveData(char* data, size_t size, size_t nmemb, string* pBuffer)
{
    auto dataSize = size * nmemb;
    pBuffer->append(data, dataSize);

    return dataSize;
}
