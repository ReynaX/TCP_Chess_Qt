#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>
#include "LogicController.h"

class QTcpSocket;
class ChessGame
{
public:
    ChessGame();
    ChessGame(std::string gameID, QTcpSocket* whitePlayer, LogicController* controller);
    ~ChessGame();

    void addPlayer(QTcpSocket* blackPlayer);
    std::string getGameID() const;
    QTcpSocket* getWhitePlayer() const;
    QTcpSocket* getBlackPlayer() const;
    QTcpSocket* getOtherSocket(QTcpSocket* socket) const;
    LogicController* getLogicController() const;
    bool isPlaying() const;

    void setWhitePlayer(QTcpSocket* whitePlayer);
    void setBlackPlayer(QTcpSocket* blackPlayer);
    bool isInGame(QTcpSocket* socket);
private:
    std::string m_gameID;

    QTcpSocket* m_whitePlayer;
    QTcpSocket* m_blackPlayer;

    bool m_isPlaying;

    LogicController* m_logicController;
};

#endif // CHESSGAME_H
