QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Bishop.cpp \
        King.cpp \
        Knight.cpp \
        LogicController.cpp \
        Pawn.cpp \
        Queen.cpp \
        Rook.cpp \
        chessgame.cpp \
        main.cpp \
        tcpserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Bishop.h \
    Globals.h \
    King.h \
    Knight.h \
    LogicController.h \
    Move.h \
    Pawn.h \
    Piece.h \
    Queen.h \
    Rook.h \
    chessgame.h \
    tcpserver.h

DISTFILES += \
    Server.pro.user
