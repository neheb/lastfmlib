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
 * @file lastfmscrobbler.h
 * @brief Contains the LastFmScrobbler class
 * @author Dirk Vanden Boer
 */

#ifndef LAST_FM_SCROBBLER_H
#define LAST_FM_SCROBBLER_H

#include <condition_variable>
#include <mutex>
#include <thread>

#include "lastfmclient.h"
#include "submissioninfo.h"
#include "submissioninfocollection.h"

class LastFmScrobbler {
public:
    /** Constructor which will use the Last.fm client identifier and version of lastfmlib
     * \param user Last.fm user name
     * \param pass Last.fm password for user
     * \param hashedPass true if the password is hashed, false otherwise
     * \param synchronous if false all public methods will be executed in
     * a thread and return immediately (prevents long blocking methods in
     * case of network problems)
     */
    LastFmScrobbler(std::string user, const std::string& pass, bool hashedPass, bool synchronous);

    /** Constructor using your own client identifier (see http://www.last.fm/api/submissions#1.1)
     * \param clientIdentifier the Last.fm client identifier
     * \param clientVersion the Last.fm client version
     * \param user Last.fm user name
     * \param pass Last.fm password for user
     * \param hashedPass true if the password is hashed, false otherwise
     * \param synchronous if false all public methods will be executed in
     * a thread and return immediately (prevents long blocking methods in
     * case of network problems)
     */
    LastFmScrobbler(std::string clientIdentifier, std::string clientVersion, std::string user, const std::string& pass, bool hashedPass, bool synchronous);

    ~LastFmScrobbler();

    LastFmScrobbler(const LastFmScrobbler&) = delete;
    LastFmScrobbler& operator=(const LastFmScrobbler&) = delete;

    /** Authenticate with the Last.fm server, this must be called before
     * you can start comitting tracks
     */
    void authenticate();

    /** When commit only mode is set, now playinginfo is not updated on
     * Last.fm, tracks will only be commited once they finished playing
     * \param enabled set commitOnlyMode to true or false
     */
    void setCommitOnlyMode(bool enabled);

    /** Indicate that a new track has started playing, the previous track
     * will be submitted (if available) and the new track will be set as
     * Now Playing
     * \param info SubmissionInfo object containing information about
     * the new song
     */
    void startedPlaying(const SubmissionInfo& info);
    /** Indicate that the current track has stopped playing. The current
     * track will be submitted to Last.fm
     */
    void finishedPlaying();

    /** Indicate that playback of the current track has been (un)paused
     * \param paused true if track is being paused, false if being unpaused
     */
    void pausePlaying(bool paused);

    /** Set the proxy server to use for the connection to the Last.fm servers
     * \param server the address of the proxy server
     * \param port the port of the proxy server
     * \param username the username if the server needs authentication
     * \param password the password if the server needs authentication
     */
    void setProxy(const std::string& server, uint32_t port, const std::string& username = "", const std::string& password = "") const;

protected:
    explicit LastFmScrobbler(bool synchronous);
    std::shared_ptr<LastFmClient> m_pLastFmClient;
    /** \brief Last time a connection attempt was made */
    time_t m_LastConnectionAttempt {};
    /** \brief The time that the current track has been played, is set on pause */
    time_t m_TrackPlayTime { -1 };
    /** \brief The time that the current track was resumed after a pause */
    time_t m_TrackResumeTime {};
    /** \brief Thread handle of authentication thread (protected for testing) */
    std::thread m_AuthenticateThread;
    /** \brief Thread handle of sendinfo thread (protected for testing) */
    std::thread m_SendInfoThread;
    /** \brief Thread handle of finishPlaying thread (protected for testing) */
    std::thread m_FinishPlayingThread;

private:
    void authenticateIfNecessary();
    void authenticateNow();
    bool trackCanBeCommited(const SubmissionInfo& info);
    [[nodiscard]] bool canReconnect() const;
    void submitTrack(const SubmissionInfo& info);
    void setNowPlaying();

    void authenticateThread();
    void sendInfoThread();
    void finishPlayingThread();

    SubmissionInfo m_PreviousTrackInfo;
    SubmissionInfo m_CurrentTrackInfo;
    SubmissionInfoCollection m_BufferedTrackInfos;

    bool m_Authenticated {};
    int m_HardConnectionFailureCount {};
    std::condition_variable m_AuthenticatedCondition;
    std::mutex m_AuthenticatedMutex;
    std::mutex m_TrackInfosMutex;

    std::string m_Username;
    std::string m_Password;

    bool m_Synchronous;
    bool m_CommitOnly {};
};

#endif
