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
 * @file nowplayinginfo.h
 * @brief Contains the NowPlayingInfo class
 * @author Dirk Vanden Boer
 */

#ifndef NOW_PLAYING_INFO_H
#define NOW_PLAYING_INFO_H

#include <iostream>

/** The NowPlayingInfo class contains all the necessary information to
 *  set the Now Playing info on Last.Fm. Artist and Track are required
 *  fields that must be set.
 */
class NowPlayingInfo {
public:
    /** \brief Default constructor */
    NowPlayingInfo() = default;
    /** \brief Constructor that sets artist and track */
    NowPlayingInfo(std::string artist, std::string track);
    /** \brief Constructor that sets artist and track (unicode) */
    NowPlayingInfo(const std::wstring& artist, const std::wstring& track);

    /** \brief returns the postdata needed to submit the info to Last.fm, used by LastFmClient */
    [[nodiscard]] std::string getPostData() const;

    /** \brief sets the artist of the track */
    void setArtist(std::string artist);
    /** \brief sets the artist of the track (unicode) */
    void setArtist(const std::wstring& artist);

    /** \brief sets the title of the track */
    void setTrack(std::string track);
    /** \brief sets the title of the track (unicode) */
    void setTrack(const std::wstring& track);
    /** \brief sets the album of the track */

    void setAlbum(std::string album);
    /** \brief sets the album of the track (unicode) */
    void setAlbum(const std::wstring& album);
    /** \brief sets the track length (in seconds) */

    void setTrackLength(int lengthInSecs);
    /** \brief sets the track number */
    void setTrackNr(int trackNr);
    /** \brief sets the Music Brainz Id */
    void setMusicBrainzId(std::string musicBrainzId);
    /** \brief sets the Music Brainz Id (unicode) */
    void setMusicBrainzId(const std::wstring& musicBrainzId);

    /** \brief returns the artist */
    [[nodiscard]] const std::string& getArtist() const;
    /** \brief returns the track title */
    [[nodiscard]] const std::string& getTrack() const;
    /** \brief returns the album */
    [[nodiscard]] const std::string& getAlbum() const;
    /** \brief returns the track length (in seconds) */
    [[nodiscard]] int getTrackLength() const;
    /** \brief returns the track number */
    [[nodiscard]] int getTrackNr() const;
    /** \brief returns Music Brainz Id */
    [[nodiscard]] const std::string& getMusicBrainzId() const;

private:
    std::string m_Artist; /**< \brief the artist */
    std::string m_Track; /**< \brief the track title */
    std::string m_Album; /**< \brief the album */

    int m_TrackLengthInSecs { -1 }; /**< \brief the track length (in seconds) */
    int m_TrackNr { -1 }; /**< \brief the track number */
    std::string m_MusicBrainzId; /**< \brief the Music Brainz Id */
};

#endif
