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

#include "lastfmscrobbler.h"

#include "utils/log.h"

#include <algorithm>
#include <csignal>
#include <iostream>
#include <stdexcept>

using namespace std;

static const time_t MIN_SECONDS_TO_SUBMIT = 240;
static const time_t MIN_TRACK_LENGTH_TO_SUBMIT = 30;
static const time_t MIN_SECS_BETWEEN_CONNECT = 60;
static const time_t MAX_SECS_BETWEEN_CONNECT = 7200;

LastFmScrobbler::LastFmScrobbler(string user, const string& pass, bool hashedPass, bool synchronous)
: m_pLastFmClient(std::make_shared<LastFmClient>())
, m_Username(std::move(user))
, m_Password(pass)
, m_Synchronous(synchronous)
{
    if (!hashedPass) {
        m_Password = LastFmClient::generatePasswordHash(pass);
    }
}

LastFmScrobbler::LastFmScrobbler(std::string clientIdentifier, std::string clientVersion, string user, const string& pass, bool hashedPass, bool synchronous)
: m_pLastFmClient(std::make_shared<LastFmClient>(std::move(clientIdentifier), std::move(clientVersion)))
, m_Username(std::move(user))
, m_Password(pass)
, m_Synchronous(synchronous)
{
    if (!hashedPass) {
        m_Password = LastFmClient::generatePasswordHash(pass);
    }
}

LastFmScrobbler::LastFmScrobbler(bool synchronous)
: m_Synchronous(synchronous)
{
}

LastFmScrobbler::~LastFmScrobbler()
{
    if (m_FinishPlayingThread.joinable())
        m_FinishPlayingThread.join();
    if (m_SendInfoThread.joinable())
        m_SendInfoThread.join();
    if (m_AuthenticateThread.joinable())
        m_AuthenticateThread.join();
}

void LastFmScrobbler::authenticate()
{
    authenticateIfNecessary();
}

void LastFmScrobbler::setCommitOnlyMode(bool enabled)
{
    m_CommitOnly = enabled;
}

void LastFmScrobbler::startedPlaying(const SubmissionInfo& info)
{
    authenticateIfNecessary();

    Log::info("startedPlaying " + info.getTrack());
    m_PreviousTrackInfo = m_CurrentTrackInfo;
    m_CurrentTrackInfo = info;

    if (m_CurrentTrackInfo.getTimeStarted() < 0) {
        m_CurrentTrackInfo.setTimeStarted(time(nullptr));
    }

    if (m_Synchronous) {
        submitTrack(m_PreviousTrackInfo);
        if (!m_CommitOnly) {
            setNowPlaying();
        }
    } else {
        if (m_SendInfoThread.joinable())
            m_SendInfoThread.join();
        m_SendInfoThread = std::thread([this] { sendInfoThread(); });
    }
}

void LastFmScrobbler::pausePlaying(bool paused)
{
    time_t curTime = time(nullptr);
    if (paused) {
        m_TrackPlayTime += curTime - m_CurrentTrackInfo.getTimeStarted();
    } else {
        m_TrackResumeTime = curTime;
    }
}

void LastFmScrobbler::finishedPlaying()
{
    authenticateIfNecessary();
    if (m_Synchronous) {
        submitTrack(m_CurrentTrackInfo);
    } else {
        if (m_FinishPlayingThread.joinable())
            m_FinishPlayingThread.join();
        m_FinishPlayingThread = std::thread([this] { finishPlayingThread(); });
    }
}

void LastFmScrobbler::setProxy(const std::string& server, uint32_t port, const std::string& username, const std::string& password)
{
    m_pLastFmClient->setProxy(server, port, username, password);
}

bool LastFmScrobbler::trackCanBeCommited(const SubmissionInfo& info)
{
    time_t curTime = time(nullptr);
    m_TrackPlayTime += curTime - m_TrackResumeTime;

    bool trackTooShort = info.getTrackLength() < MIN_TRACK_LENGTH_TO_SUBMIT;
    bool trackPlayedLongEnough = m_TrackPlayTime >= MIN_SECONDS_TO_SUBMIT
        || m_TrackPlayTime >= (info.getTrackLength() / 2);

    if (trackTooShort) {
        Log::info("Track \"" + info.getTrack() + "\" can't be committed: length is too short");
    } else if (!trackPlayedLongEnough) {
        Log::info("Track \"" + info.getTrack() + "\" can't be committed: not played long enough");
    } else {
        Log::info("Track \"" + info.getTrack() + "\" can be committed: conditions OK");
    }

    return (!trackTooShort) && trackPlayedLongEnough;
}

void LastFmScrobbler::authenticateIfNecessary()
{
    if (!m_Authenticated && !m_AuthenticateThread.joinable() && canReconnect()) {
        if (m_Synchronous) {
            authenticateNow();
        } else {
            if (m_AuthenticateThread.joinable())
                m_AuthenticateThread.join();
            m_AuthenticateThread = std::thread([this] { authenticateThread(); });
        }
    }
}

void LastFmScrobbler::authenticateNow()
{
    try {
        m_pLastFmClient->handshake(m_Username, m_Password);
        Log::info("Authentication successfull for user: " + m_Username);
        m_HardConnectionFailureCount = 0;
        m_Authenticated = true;
    } catch (ConnectionError&) {
        ++m_HardConnectionFailureCount;
        m_LastConnectionAttempt = time(nullptr);
    } catch (logic_error& e) {
        Log::error(e.what());
    }
}

bool LastFmScrobbler::canReconnect() const
{
    time_t curTime = time(nullptr);
    time_t timeSinceLastConnectionAttempt = curTime - m_LastConnectionAttempt;
    time_t connectionDelay = max(MAX_SECS_BETWEEN_CONNECT, m_HardConnectionFailureCount * MIN_SECS_BETWEEN_CONNECT);

    return timeSinceLastConnectionAttempt > connectionDelay;
}

void LastFmScrobbler::authenticateThread()
{
    Log::info("Authenticate thread started");

    authenticateNow();

    {
        auto lock = std::scoped_lock(m_AuthenticatedMutex);
        m_AuthenticatedCondition.notify_all();
    }

    Log::info("Authenticate thread finished");
}

void LastFmScrobbler::sendInfoThread()
{
    Log::debug("sendInfo thread started");

    {
        auto lock = std::unique_lock(m_AuthenticatedMutex);
        if (!m_Authenticated) {
            if (!m_AuthenticatedCondition.wait_for(lock, 4ms, [this] { return m_Authenticated; })) {
                Log::info("send info terminated because no connection");
                submitTrack(m_PreviousTrackInfo);
                return;
            }
        }
    }

    submitTrack(m_PreviousTrackInfo);
    if (!m_CommitOnly) {
        setNowPlaying();
    }

    Log::debug("sendInfo thread finished");
}

void LastFmScrobbler::finishPlayingThread()
{
    Log::debug("finishPlaying thread started");

    {
        auto lock = std::scoped_lock(m_AuthenticatedMutex);
        if (!m_Authenticated) {
            // Program is probalby cleaning up, dont't try to start authentication
            return;
        }
    }

    submitTrack(m_PreviousTrackInfo);

    Log::debug("finishPlaying thread finished");
}

void LastFmScrobbler::setNowPlaying()
{
    if (!m_Authenticated) {
        Log::info("Can't set Now Playing status: not authenticated");
        return;
    }

    try {
        m_pLastFmClient->nowPlaying(m_CurrentTrackInfo);
        Log::info("Now playing info submitted: " + m_CurrentTrackInfo.getArtist() + " - " + m_CurrentTrackInfo.getTrack());
    } catch (BadSessionError&) {
        Log::info("Session has become invalid: starting new handshake");
        authenticateNow();
        setNowPlaying();
    } catch (ConnectionError&) {
        m_Authenticated = false;
    } catch (logic_error& e) {
        Log::error(e.what());
    }
}

void LastFmScrobbler::submitTrack(const SubmissionInfo& info)
{
    if (info.getTrackLength() < 0 || !trackCanBeCommited(info)) {
        Log::info("Won't submit");
        m_TrackPlayTime = 0;
        m_TrackResumeTime = m_CurrentTrackInfo.getTimeStarted();
        return;
    }

    {
        auto lock = std::scoped_lock(m_TrackInfosMutex);
        m_BufferedTrackInfos.addInfo(info);
    }

    SubmissionInfoCollection tracksToSubmit;
    {
        auto lock = std::scoped_lock(m_TrackInfosMutex);
        tracksToSubmit = m_BufferedTrackInfos;
    }

    try {
        if (m_Authenticated) {
            m_pLastFmClient->submit(tracksToSubmit);
            Log::info("Buffered tracks submitted");
            m_BufferedTrackInfos.clear();
        } else {
            Log::info("Track info buffered: not connected");
        }
    } catch (BadSessionError&) {
        Log::info("Session has become invalid: starting new handshake");
        authenticateNow();
        submitTrack(info);
    } catch (ConnectionError&) {
        m_Authenticated = false;
    } catch (logic_error& e) {
        Log::error(e.what());
    }

    m_TrackPlayTime = 0;
    m_TrackResumeTime = m_CurrentTrackInfo.getTimeStarted();
}
