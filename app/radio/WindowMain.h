#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <lastfm/RadioStation>

#include "lib/unicorn/UnicornMainWindow.h"

namespace Ui { class WindowMain; }

class WindowMain : public unicorn::MainWindow
{
    Q_OBJECT

public:
    explicit WindowMain( class Actions& actions );
    ~WindowMain();

signals:
    void aboutToHide();

private slots:
    void onStartClicked();
    void onPlayClicked( bool checked );
    void onRadioTick( qint64 tick );
    void onTrackSpooled( const Track& track );
    void onTuningIn( const RadioStation& station );
    void onError( int error , const QVariant& errorData );
    void onStopped();

    void onGotRecentStations();
    void onGotEvents();

    void onLoveClicked( bool loved );
    void onLoveTriggered();
    void onBanClicked();
    void onBanFinished();
    void onSkipClicked();
    void onInfoClicked();

    void onBackClicked();
    void onNowPlayingClicked();

    void onStationEditTextChanged( const QString& text );

    void onEditClicked();
    void onFilterClicked();

    void onActionsChanged();

    void onSwitch();

    void onSpace();

private:
    void addWinThumbBarButtons( QList<QAction*>& );

private:
    Ui::WindowMain* ui;

    class Actions* m_actions;
};

#endif // WINDOWMAIN_H
