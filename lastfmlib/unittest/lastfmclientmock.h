#ifndef LAST_FM_CLIENT_MOCK_H
#define LAST_FM_CLIENT_MOCK_H

#include "lastfmlib/lastfmclient.h"
#include "lastfmlib/submissioninfo.h"
#include "lastfmlib/submissioninfocollection.h"
#include <string>

class NowPlayingInfo;
class SubmissionInfo;
class SubmissionInfoCollection;

class LastFmClientMock : public LastFmClient {
public:
    void handshake(const std::string& user, const std::string& pass) override;
    void nowPlaying(const NowPlayingInfo& info) override;
    void submit(const SubmissionInfo& info) override;
    void submit(const SubmissionInfoCollection& infoCollection) override;

    bool m_HandShakeThrowConnectionError {};
    bool m_HandShakeThrowException {};
    bool m_BadSessionError {};
    bool m_NowPlayingCalled {};
    bool m_SubmitCalled {};
    bool m_SubmitCollectionCalled {};
    bool m_HandshakeCalled {};

    NowPlayingInfo m_LastRecPlayingInfo;
    SubmissionInfo m_LastRecSubmitInfo;
    SubmissionInfoCollection m_LastRecSubmitInfoCollection;
};

#endif
