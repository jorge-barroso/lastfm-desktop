TARGET = scrobble
TEMPLATE = lib
QT = core network xml
CONFIG += core ws types

include( $$SRC_DIR/common/qmake/include.pro )

SOURCES = $$findSources( cpp )
HEADERS = $$findSources( h )

DEFINES += _SCROBBLE_DLLEXPORT