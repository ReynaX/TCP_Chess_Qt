#include "chessgame.h"
#include <QHostAddress>
#include <QTcpSocket>

ChessGame::ChessGame(){
}

ChessGame::ChessGame(std::string gameID, QTcpSocket *whitePlayer)
    : m_gameID(gameID), m_whitePlayer(whitePlayer), m_blackPlayer(nullptr), m_isPlaying(false){
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

QTcpSocket *ChessGame::getOtherSocket(QTcpSocket *socket){
    if(socket->peerAddress() == m_whitePlayer->peerAddress() && socket->peerPort() == m_whitePlayer->peerPort())
        return m_blackPlayer;
    return m_whitePlayer;
}

bool ChessGame::isPlaying(){
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
    return (socket->peerAddress() == m_whitePlayer->peerAddress() && socket->peerPort() == m_whitePlayer->peerPort()) ||
            (socket->peerAddress() == m_blackPlayer->peerAddress() && socket->peerPort() == m_blackPlayer->peerPort());
}



