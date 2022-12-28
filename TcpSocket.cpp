#include "TcpSocket.h"
#include <QHostAddress>

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent) {
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    this->connectToHost(QHostAddress("127.0.0.1"), 1234);
    m_playerName = "aha";
}

void TcpSocket::hostGame(){
    QByteArray outputData;
    outputData.append('h');
    this->write(outputData);
}

void TcpSocket::joinGame(){
    QByteArray outputData;
    outputData.append('j');
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    this->write(outputData);
}

void TcpSocket::disconnect(){
   QByteArray outputData;
   outputData.append('d');
   outputData.append(m_gameID.length());
   outputData.append(m_gameID.c_str(), m_gameID.length());
   this->write(outputData);
}

void TcpSocket::move(std::string from, std::string to){
    QByteArray outputData;
    outputData.append('m');
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    outputData.append(from.length());
    outputData.append(from.c_str(), from.length());

    outputData.append(to.length());
    outputData.append(to.c_str(), to.length());
    qDebug() << "Sending data" << outputData;
    this->write(outputData);
}

std::string TcpSocket::getPlayerName() const{
    return m_playerName;
}

void TcpSocket::setPlayerName(std::string playerName){
    m_playerName = playerName;
}

void TcpSocket::setGameID(std::string gameID){
    m_gameID = gameID;
}

std::string TcpSocket::getGameID(){
    return m_gameID;
}

void TcpSocket::drawGame(){
    QByteArray outputData;
    outputData.append('r'); // code message for draw
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    this->write(outputData);
}

void TcpSocket::wonGame(std::string color){
    QByteArray outputData;
    outputData.append('c'); // code message for checkmate
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    outputData.append(color[0]);
    this->write(outputData);
}

void TcpSocket::readyRead(){
    QByteArray inputData = this->readAll();
    qDebug() << "Data received:" << inputData;
    while(!inputData.isEmpty()){
        char messagecode = inputData[0];
        inputData.remove(0,  1);
        if(messagecode == 0){
            qDebug() << "Error";
        }else if(messagecode == 'e'){
            qDebug() << "Server got the message";
        }else if(messagecode == 'm'){
            handleMove(inputData);
        }else if(messagecode == 'n'){
            emit enableMovement();
        }else if(messagecode == 'j'){
             std::string gameID = getProperty(inputData);
             m_gameID = gameID;
             qDebug() << "Game ID" << m_gameID.c_str();
             emit gameIDChanged(); emit joined();
        }else if(messagecode == 'd'){
            qDebug() << "Emitting";
            disconnectFromHost();
            emit disconnectedFromServer();
        }else if(messagecode == 'l'){
            emit lost();
        }else if(messagecode == 'w'){
            emit won();
        }
    }
}

std::string TcpSocket::getProperty(QByteArray& data){
    int length = data[0];
    data.remove(0, 1);
    std::string property = data.left(length).toStdString();
    data.remove(0, length);
    return property;
}

void TcpSocket::handleMove(QByteArray data){
    std::string from = getProperty(data);
    std::string to = getProperty(data);
    qDebug() << "Emitting move signal" << from.c_str() << to.c_str();
    emit moveSignal(from, to);
}

