QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++14 c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Bishop.cpp \
    Boardsquare.cpp \
    Chessboard.cpp \
    King.cpp \
    Knight.cpp \
    LogicController.cpp \
    Pawn.cpp \
    Queen.cpp \
    Rook.cpp \
    TcpSocket.cpp \
    joingamedialog.cpp \
    main.cpp

HEADERS += \
    Bishop.h \
    Boardsquare.h \
    Chessboard.h \
    Globals.h \
    King.h \
    Knight.h \
    LogicController.h \
    Move.h \
    Pawn.h \
    Piece.h \
    Queen.h \
    Rook.h \
    SoundPlayer.h \
    TcpSocket.h \
    joingamedialog.h

FORMS += \
    Chessboard.ui \
    joingamedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
