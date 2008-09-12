/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#ifndef UNICORN_TRACK_H
#define UNICORN_TRACK_H

#include "Album.h"
#include "Artist.h"
#include "lib/DllExportMacro.h"
#include "lib/core/WeightedStringList.h"
#include <QDateTime>
#include <QDomElement>
#include <QExplicitlySharedDataPointer>
#include <QString>
#include <QMap>
#include <QUrl>


struct TrackData : QSharedData
{
    TrackData();

    QString artist;
    QString album;
    QString title;
    int trackNumber;
    int playCount;
    int duration;
    short source;
    short rating;
    QString playerId;
    QString mbId; /// musicbrainz id
    QString fpId; /// fingerprint id
    QUrl url;
    QDateTime time; /// the time the track was started at

    //FIXME I hate this, but is used for radio trackauth etc.
    QMap<QString,QString> extras;
};


class TYPES_DLLEXPORT Track
{
public:
    enum Source
    {
        // DO NOT UNDER ANY CIRCUMSTANCES CHANGE THE ORDER OR VALUES OF THIS ENUM!
        // you will cause broken settings and b0rked scrobbler cache submissions

        Unknown = -1,
        LastFmRadio,
        Player,
        MediaDevice,
		NonPersonalisedBroadcast, // eg Shoutcast, BBC Radio 1, etc.
		PersonalisedRecommendation, // eg Pandora, but not Last.fm
    };

    enum Rating
    {
        // DO NOT UNDER ANY CIRCUMSTANCES CHANGE THE ORDER OR VALUES OF THIS ENUM!
        // you will cause broken settings and b0rked scrobbler cache submissions
		//NOTE sorted in precedence order

		NotScrobbled = 0x0001,
		Scrobbled	 = 0x0002,
        Loved		 = 0x0004,
        Skipped		 = 0x0008,
        Banned		 = 0x0018,	//Implied skip
    };
	

    enum ScrobblableStatus
    {
        OkToScrobble,
        NoTimeStamp,
        TooShort,
        ArtistNameMissing,
        TrackNameMissing,
        ExcludedDir,
        ArtistInvalid,
        FromTheFuture,
        FromTheDistantPast
    };

    Track();
    Track( const QDomElement& );

    /** this track and that track point to the same object, so they are the same
      * in fact. This doesn't do a deep data comparison. So even if all the 
      * fields are the same it will return false if they aren't in fact spawned
      * from the same initial Track object */
    bool operator==( const Track& that ) const
    {
        return this->d == that.d;
    }
    bool operator!=( const Track& that ) const
    {
        return !operator==( that );
    }

    /** not a great isEmpty check, but most services will complain if these two
      * are empty */
    bool isNull() const { return d->artist.isEmpty() && d->title.isEmpty(); }

    /** the standard representation of this object as an XML node */
    QDomElement toDomElement( class QDomDocument& ) const;

    Artist artist() const { return Artist( d->artist ); }
    Album album() const { return Album( artist(), d->album ); }
    QString title() const { return d->title; }
    int trackNumber() const { return d->trackNumber; }
    int playCount() const { return d->playCount; }
    int duration() const { return d->duration; }
    QString durationString() const;
    QString mbId() const { return d->mbId; }
    QUrl url() const { return d->url; }
    QDateTime timeStamp() const { return d->time; }
    QDateTime dateTime() const { return d->time; }
    Source source() const { return (Source)d->source; }

    /** scrobbler submission source string code */
    QString sourceString() const;
    QString playerId() const { return d->playerId; }
    QString fpId() const { return d->fpId; }

    bool isLoved() const { return d->rating & Loved; }
    bool isBanned() const { return d->rating & Banned; }
	bool isSkipped() const { return d->rating & Skipped; }
	bool isScrobbled() const { return isLoved() || d->rating & Scrobbled; }
	/** only one rating is possible, we have to figure out which from various flags applied */
	QString ratingCharacter() const;

    QString prettyTitle( const QChar& separator = QChar(8211) /*en dash*/ ) const;

    /** used to sort tracks into chronological order, used by scrobbling */
    static bool lessThan( const Track &t1, const Track &t2)
    {
        return t1.timeStamp() < t2.timeStamp();
    }

	/** See last.fm/api Track section */
    WsReply* share( const class User& recipient, const QString& message = "" );
	WsReply* love();
	WsReply* ban();
	WsReply* getTags() const; // for the logged in user
	WsReply* getTopTags() const;
	static WeightedStringList getTopTags( WsReply* );

	/** the url for this track's page at last.fm */
	QUrl www() const;
	
protected:
    friend class MutableTrack; //FIXME wtf? but compiler error otherwise
    QExplicitlySharedDataPointer<TrackData> d;
};


class TYPES_DLLEXPORT MutableTrack : public Track
{
public:
    MutableTrack()
    {}

    MutableTrack( const Track& that )
    {
        this->d = that.d;
    }

    void setArtist( QString artist ) { d->artist = artist.trimmed(); }
    void setAlbum( QString album ) { d->album = album.trimmed(); }
    void setTitle( QString title ) { d->title = title.trimmed(); }
    void setTrackNumber( int n ) { d->trackNumber = n; }
    void setPlayCount( int playCount ) { d->playCount = playCount; }
    void setDuration( int duration ) { d->duration = duration; }
    void setMbId( QString mbId ) { d->mbId = mbId; }
    void setUrl( QUrl url ) { d->url = url; }
    void setSource( Source s ) { d->source = s; }
    void setPlayerId( QString id ) { d->playerId = id; }
    void setFpId( QString id ) { d->fpId = id; }
    
	void upgradeRating( Rating r )
	{
		d->rating |= r;
	}
	
	/** Unloving is the only valid downgrade */
	void downgradeRating( Rating r )
	{
		Q_ASSERT_X( r == Loved, "track rating downgrade", "Should only downgrade love" );
		d->rating ^= r;
	}
	
    void stamp() { d->time = QDateTime::currentDateTime(); }

    void setExtra( QString key, QString value ) { d->extras[key] = value; }
};


inline 
TrackData::TrackData() 
             : trackNumber( 0 ),
               playCount( 0 ),
               duration( 0 ),
               source( Track::Unknown ),
               rating( 0 )
{}


#include <QDebug>
inline QDebug operator<<( QDebug d, const Track& t )
{
    return d << t.prettyTitle( '-' ) << t.album();
}


#include <QMetaType>
Q_DECLARE_METATYPE( Track );

#endif
