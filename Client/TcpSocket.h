#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include "Globals.h"

class JoinGameDialog;
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QObject* parent = nullptr);

public slots:
    void hostGame();
    void joinGame();
    void disconnect();
    void move(std::string from, std::string to);

    void setGameID(std::string gameID);
    std::string getGameID();
    void drawGame();
    void wonGame(std::string color);
    //
    void readyRead();
signals:
    // Signals responsible for notifying Chess board of changes coming from the server
    void moveSignal(std::string from, std::string to);
    void enableMovement();
    void disconnectedFromServer();
    void gameIDChanged();
    void joined();
    void failedToJoin();
    void won();
    void lost();
    void boardChanged(std::string board);
    void possibleMovesSignal(std::unordered_multimap<Pos, Pos, PosHash> moves);
private:
    std::string getProperty(QByteArray& data);

    void handleMove(QByteArray& data);
    std::unordered_multimap<Pos, Pos, PosHash> parsePossibleMoves(QByteArray& data);
    std::string m_gameID;
};

#endif // TCPSOCKET_H
