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

/**
 * @file lastfmclient.h
 * @brief Contains the LastFmClient class
 * @author Dirk Vanden Boer
 */

#ifndef LAST_FM_CLIENT_H
#define LAST_FM_CLIENT_H

#include "lastfmexceptions.h"
#include "urlclient.h"

class NowPlayingInfo;
class SubmissionInfo;
class SubmissionInfoCollection;

/** The LastFmClient class provides access to Last.Fm to submit tracks
 *  and set Now Playing info.
 */
class LastFmClient {
public:
    /** Default constructor which will use the Last.fm client identifier
     * and version of lastfmlib
     */
    LastFmClient() = default;

    /** Constructor
     * \param clientIdentifier an std::string containing the Last.fm client identifier
     * \param clientVersion an std::string containing the Last.fm client version
     */
    LastFmClient(std::string clientIdentifier, std::string clientVersion);

    virtual ~LastFmClient() = default;

    /** Attempt to authenticate with the Last.fm server
     * \param user an std::string containing the username
     * \param pass an std::string containing a hashed password (use static generatePasswordHash method)
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when authentication with Last.fm server fails
     */
    virtual void handshake(const std::string& user, const std::string& pass);

    /** Set a track as Now Playing on Last.fm
     * \param info a NowPlaying oject containing the current track information
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when setting Now Playing info fails
     */
    virtual void nowPlaying(const NowPlayingInfo& info);

    /** Submit a played track to the Last.fm server
     * \param info a SubmissionInfo oject containing information about the played track
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when submitting the Track info fails
     */
    virtual void submit(const SubmissionInfo& info);

    /** Submit a collection of played tracks to the Last.fm server (max. 50)
     * \param infoCollection a SubmissionInfoCollection oject containing played tracks
     * \exception ConnectionError when connection to Last.fm server fails
     * \exception std::logic_error when submitting the Track info collection fails
     */
    virtual void submit(const SubmissionInfoCollection& infoCollection);

    /** Generates an md5 hash of the supplied password which can also be used
     * to login and is safer to store
     * \param password the password to generate a hash for
     * \return a string containing the hashed password
     */
    static std::string generatePasswordHash(const std::string& password);

    /** Set the proxy server to use for the connection to the Last.fm servers
     * \param server the address of the proxy server
     * \param port the port of the proxy server
     * \param username the username if the server needs authentication
     * \param password the password if the server needs authentication
     */
    void setProxy(const std::string& server, uint32_t port, const std::string& username = "", const std::string& password = "");

private:
    [[nodiscard]] std::string createRequestString(const std::string& user, const std::string& pass) const;
    [[nodiscard]] std::string createNowPlayingString(const NowPlayingInfo& info) const;
    [[nodiscard]] std::string createSubmissionString(const SubmissionInfo& info) const;
    [[nodiscard]] std::string createSubmissionString(const SubmissionInfoCollection& infoCollection) const;
    void throwOnInvalidSession() const;
    void submit(const std::string& postData) const;

    UrlClient m_UrlClient;
    std::string m_ClientIdentifier { "lfc" };
    std::string m_ClientVersion { "1.0" };
    std::string m_SessionId;
    std::string m_NowPlayingUrl;
    std::string m_SubmissionUrl;
};

#endif
