#include "TcpSocket.h"
#include <QHostAddress>

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent) {
    connect(this, SIGNAL(readyRead()), this, SLOT(readyRead()));
    this->connectToHost(QHostAddress("127.0.0.1"), 1234);
}

void TcpSocket::hostGame(){
    QByteArray outputData;
    outputData.append('h');
    qDebug() << "Sending data" << outputData;
    this->write(outputData);
}

void TcpSocket::joinGame(){
    QByteArray outputData;
    outputData.append('j');
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    qDebug() << "Sending data" << outputData;
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
    qDebug() << "Sending data" << outputData;
    this->write(outputData);
}

void TcpSocket::wonGame(std::string color){
    QByteArray outputData;
    outputData.append('c'); // code message for checkmate
    outputData.append(m_gameID.length());
    outputData.append(m_gameID.c_str(), m_gameID.length());
    outputData.append(color[0]);
    qDebug() << "Sending data" << outputData;
    this->write(outputData);
}

void TcpSocket::readyRead(){
    QByteArray inputData = this->readAll();
    qDebug() << "Data received:" << inputData;
    while(!inputData.isEmpty()){
        char messagecode = inputData[0];
        inputData.remove(0,  1);
        if(messagecode == 0){
            emit failedToJoin();
        }else if(messagecode == 'm'){
            handleMove(inputData);
        }else if(messagecode == 'n'){
            emit enableMovement();
        }else if(messagecode == 'j'){
             std::string gameID = getProperty(inputData);
             m_gameID = gameID;
             emit gameIDChanged(); emit joined();
        }else if(messagecode == 'd'){
            disconnectFromHost();
            emit disconnectedFromServer();
        }else if(messagecode == 'l'){
            emit lost();
        }else if(messagecode == 'w'){
            emit won();
        }else if(messagecode == 'p'){
            auto possibleMoves = parsePossibleMoves(inputData);
            emit possibleMovesSignal(possibleMoves);
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

void TcpSocket::handleMove(QByteArray& data){
    std::string board = QString(data.left(64)).toStdString();
    data.remove(0, 64);
    emit boardChanged(board);
}

std::unordered_multimap<Pos, Pos, PosHash> TcpSocket::parsePossibleMoves(QByteArray& data){
    std::unordered_multimap<Pos, Pos, PosHash> moves;
    while(!data.isEmpty() && data.at(0) < 10){
        std::string from = getProperty(data);
        std::string to = getProperty(data);

        Pos fromPos = Pos(std::stoi(from) / 8, std::stoi(from) % 8);
        Pos toPos = Pos(std::stoi(to) / 8, std::stoi(to) % 8);
        moves.insert(std::make_pair(fromPos, toPos));
    }
    return moves;
}

