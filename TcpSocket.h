#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

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
    std::string getPlayerName() const;
    void setPlayerName(std::string playerName);
    void setGameID(std::string gameID);
    std::string getGameID();
    void drawGame();
    void wonGame(std::string color);
    void readyRead();
signals:
    void moveSignal(std::string from, std::string to);
    void enableMovement();
    void disconnectedFromServer();
    void gameIDChanged();
    void activeGames(QList<std::string> games);
    void joined();
    void won();
    void lost();
private:
    std::string getProperty(QByteArray& data);

    void handleMove(QByteArray data);

    std::string m_playerName;
    std::string m_gameID;
};

#endif // TCPSOCKET_H
