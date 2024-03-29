#include "ChessGame.h"
#include <QHostAddress>
#include <QTcpSocket>

ChessGame::ChessGame(){
}

ChessGame::~ChessGame(){
    delete m_logicController;
}

ChessGame::ChessGame(std::string gameID, QTcpSocket *whitePlayer, LogicController* controller)
    : m_gameID(gameID), m_whitePlayer(whitePlayer), m_blackPlayer(nullptr), m_isPlaying(false), m_logicController(controller){
}

void ChessGame::addPlayer(QTcpSocket* blackPlayer){
    if(m_whitePlayer != nullptr){
        m_blackPlayer = blackPlayer;
        m_isPlaying = true;
    }
}

std::string ChessGame::getGameID() const{
    return m_gameID;
}

QTcpSocket *ChessGame::getWhitePlayer() const{
    return m_whitePlayer;
}

QTcpSocket *ChessGame::getBlackPlayer() const{
    return m_blackPlayer;
}

QTcpSocket *ChessGame::getOtherSocket(QTcpSocket *socket) const{
    if(socket->peerAddress() == m_whitePlayer->peerAddress() && socket->peerPort() == m_whitePlayer->peerPort())
        return m_blackPlayer;
    return m_whitePlayer;
}

LogicController* ChessGame::getLogicController() const{
    return m_logicController;
}

bool ChessGame::isPlaying() const{
    return m_isPlaying;
}

void ChessGame::setWhitePlayer(QTcpSocket* whitePlayer){
    m_whitePlayer = whitePlayer;
}

void ChessGame::setBlackPlayer(QTcpSocket *blackPlayer){
    m_isPlaying = true;
    m_blackPlayer = blackPlayer;
}

bool ChessGame::isInGame(QTcpSocket *socket){
    return (m_whitePlayer != nullptr && socket->peerAddress() == m_whitePlayer->peerAddress() && socket->peerPort() == m_whitePlayer->peerPort()) ||
            (m_blackPlayer != nullptr && socket->peerAddress() == m_blackPlayer->peerAddress() && socket->peerPort() == m_blackPlayer->peerPort());
}



