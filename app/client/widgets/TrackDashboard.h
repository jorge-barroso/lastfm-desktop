/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#ifndef TRACK_DASHBOARD_H
#define TRACK_DASHBOARD_H

#include <QWidget>
#include "lib/lastfm/types/Track.h"


/** @author <max@last.fm> */

class TrackDashboard : public QWidget
{
    Q_OBJECT

    Track m_track;
    class QNetworkAccessManager* nam;
    
public:
    TrackDashboard();

    void beginLoadingAnimation();
    void setTrack( const class Track& );
    void clear();

    Qt::Orientation orientation() const
    {
        return ui.info->geometry().y() == 0 ? Qt::Horizontal : Qt::Vertical;
    }

    virtual QSize sizeHint() const { return QSize( 100, 100 ); }
    
    struct //FIXME make not public
    {
        QWidget* actionbar;
        QWidget* papyrus;
        QWidget* info;
        class PrettyCoverWidget* cover;
        class QWebView* bio;
        class FadingScrollBar* scrollbar;
        class SpinnerLabel* spinner;
        class QListWidget *tags;
        class QListWidget *similarArtists;
    } ui;
    
public slots:
    void setPapyrusPosition( int );
    
private slots:
    void onArtistGotInfo( WsReply* );
    void onArtistGotTopTags( WsReply* );
    void onArtistImageDownloaded();
    void openExternally( const QUrl& );
    
private:
    virtual void paintEvent( QPaintEvent* );
    virtual void resizeEvent( QResizeEvent* );
    virtual bool event( QEvent* );
};

#endif
