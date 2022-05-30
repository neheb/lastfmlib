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

#include "submissioninfo.h"
#include "utils/stringoperations.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {
std::string sourceToString(TrackSource source, const std::string& recommendationKey)
{
    switch (source) {
    case UserChosen:
        return "P";
    case NonPersonalizedBroadCast:
        return "R";
    case PersonalizedRecommendation:
        return "E";
    case Lastfm:
        return "L" + recommendationKey;
    }
    return "U";
}

std::string ratingToString(TrackRating rating)
{
    switch (rating) {
    case Love:
        return "L";
    case Ban:
        return "B";
    case Skip:
        return "S";
    }
    return "";
}
} // namespace

SubmissionInfo::SubmissionInfo()
: m_TimeStarted(0)
, m_Source(UserChosen)
, m_Rating(NoRating)
{
}

SubmissionInfo::SubmissionInfo(const string& artist, const string& track, time_t timeStarted)
: NowPlayingInfo(artist, track)
, m_TimeStarted(timeStarted)
, m_Source(UserChosen)
, m_Rating(NoRating)
{
}

SubmissionInfo::SubmissionInfo(const wstring& artist, const wstring& track, time_t timeStarted)
: NowPlayingInfo(artist, track)
, m_TimeStarted(timeStarted)
, m_Source(UserChosen)
, m_Rating(NoRating)
{
}

string SubmissionInfo::getPostData(int index) const
{
    if (m_Source == UserChosen && getTrackLength() < 0) {
        throw logic_error("Tracklength is required when submitting user chosen track");
    }

    stringstream ss;
    ss << "&a[" << index << "]=" << StringOperations::urlEncode(getArtist())
       << "&t[" << index << "]=" << StringOperations::urlEncode(getTrack())
       << "&i[" << index << "]=" << m_TimeStarted
       << "&o[" << index << "]=" << sourceToString(m_Source, m_RecommendationKey)
       << "&r[" << index << "]=" << ratingToString(m_Rating)
       << "&l[" << index << "]=" << (getTrackLength() > 0 ? StringOperations::getPostData(getTrackLength()) : "")
       << "&b[" << index << "]=" << StringOperations::urlEncode(getAlbum())
       << "&n[" << index << "]=" << (getTrackNr() > 0 ? StringOperations::getPostData(getTrackNr()) : "")
       << "&m[" << index << "]=" << StringOperations::urlEncode(getMusicBrainzId());

    return ss.str();
}

time_t SubmissionInfo::getTimeStarted() const
{
    return m_TimeStarted;
}

void SubmissionInfo::setSource(TrackSource source, const std::string& recommendationKey)
{
    m_Source = source;
    m_RecommendationKey = recommendationKey;
}

void SubmissionInfo::setRating(TrackRating rating)
{
    m_Rating = rating;
}

void SubmissionInfo::setTimeStarted(time_t timeStarted)
{
    m_TimeStarted = timeStarted;
}
