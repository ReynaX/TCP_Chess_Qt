#include "tcpserver.h"

#include <QTcpSocket>

#include "chessgame.h"

int TcpServer::NEXT_ID = 0;

TcpServer::TcpServer(QObject* parent) : QTcpServer(parent){
}

void TcpServer::startServer(){
    connect(this, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    if(!this->listen(QHostAddress("127.0.0.1"), 1234)){
        qDebug() << "Failed to start ther server";
        return;
    }
    qDebug() << "Server listening on port 1234";
}

void TcpServer::acceptConnection(){
    QTcpSocket* socket = this->nextPendingConnection();
    if(socket == nullptr){
        qDebug() << "Failed to connect to server";
        return;
    }
    qDebug() << socket->socketDescriptor() << "connected to the server";
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
}

void TcpServer::readyRead(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    QByteArray inputData = socket->readAll();
    qDebug() << inputData.toStdString().c_str();
    while(!inputData.isEmpty()){
        char action = inputData[0];
        inputData.remove(0, 1);


        if(action == 'h')
            hostNewGame(socket);
        else if(action == 'j')
            joinGame(inputData, socket);
        else if(action == 'm')
            move(inputData, socket);
        else if(action == 'd'){
            std::string gameID = getProperty(inputData);
            disconnectBothPlayers(gameID);
            qDebug() << "Disconnected";
        }else if(action == 'c'){
            gameWon(inputData, socket);
        }else if(action == 'r'){
            gameDrawn(inputData);
        }
    }
}

void TcpServer::disconnectBothPlayers(std::string gameID){
    QByteArray outputData;
    outputData.append('d');
    int lGameID = std::stoi(gameID);

    if(m_activeGames.find(lGameID) != m_activeGames.end()){
        ChessGame* game = m_activeGames[lGameID];

        game->getWhitePlayer()->write(outputData);
        game->getBlackPlayer()->write(outputData);

        m_activeGames.erase(lGameID);
    }
}

void TcpServer::disconnect(){
    QTcpSocket* socket = (QTcpSocket*)sender();

    QByteArray outputData;
    outputData.append('d');
    for(auto& game: m_activeGames){
        if(game.second->isInGame(socket)){
            auto whitePlayer = game.second->getWhitePlayer();
            auto blackPlayer = game.second->getBlackPlayer();
            whitePlayer->write(outputData);
            blackPlayer->write(outputData);

            m_activeGames.erase(game.first);
            break;
        }
    }
}

std::string TcpServer::getProperty(QByteArray& data){
    int length = data[0];
    data.remove(0, 1);
    std::string property = data.left(length).toStdString();
    data.remove(0, length);
    return property;
}

void TcpServer::hostNewGame(QTcpSocket* socket){
    qDebug() << "Host new game";
    std::string gameID = std::to_string(++NEXT_ID);

    ChessGame* newGame = new ChessGame(gameID, socket, new LogicController());
    m_activeGames.insert(std::make_pair(NEXT_ID, newGame));

    QByteArray outputData;
    outputData.append('j'); // code message for hosting
    outputData.append(gameID.length());
    outputData.append(gameID.c_str(), gameID.length());
    qDebug() << "Writing " << outputData;
    socket->write(outputData);
}

void TcpServer::joinGame(QByteArray& data, QTcpSocket *socket){
    qDebug() << "Join to game";
    std::string gameID = getProperty(data);
    qDebug() << "GameID:" << gameID.c_str();
    int lGameID = std::stoi(gameID);
    // Check if game exists and is playing
    if(m_activeGames.find(lGameID) != m_activeGames.end() && !m_activeGames.find(lGameID)->second->isPlaying()){
        qDebug() << "Player joining the game";
        ChessGame* game = m_activeGames[lGameID];

        game->addPlayer(socket);
        notifyOtherPlayer(game->getOtherSocket(socket));
        QByteArray outputData;
        outputData.append('j'); // code message for joining
        outputData.append(gameID.length());
        outputData.append(gameID.c_str(), gameID.length());
        socket->write(outputData);
    }else{
        qDebug() << "Player failed to join the game";
        QByteArray outputData;
        int x = 0;
        outputData.append(x);
        socket->write(outputData);
    }
}

void TcpServer::move(QByteArray& data, QTcpSocket *socket){
    std::string gameID = getProperty(data);
    qDebug() << "Move" << data << gameID.c_str();
    int lGameID = std::stoi(gameID);
    // Notify other player of movement
    if(m_activeGames.find(lGameID) != m_activeGames.end()){
        std::string from = getProperty(data);
        std::string to = getProperty(data);

        QByteArray outputData;
        outputData.append('m'); // code message for move
        outputData.append(from.length());
        outputData.append(from.c_str(), from.length());
        outputData.append(to.length());
        outputData.append(to.c_str(), to.length());
        qDebug() << "Move" << from.c_str() << to.c_str();
        m_activeGames[lGameID]->getOtherSocket(socket)->write(outputData);
    }
}

void TcpServer::notifyOtherPlayer(QTcpSocket* socket){
    QByteArray outputData;
    outputData.append('n'); // code message for notify
    socket->write(outputData);
}

void TcpServer::gameDrawn(QByteArray& data){
    qDebug() << "Draw";
    std::string gameID = getProperty(data);
    int lGameID = std::stoi(gameID);

    if(m_activeGames.find(lGameID) != m_activeGames.end()){
        ChessGame* game = m_activeGames[lGameID];
        QByteArray outputData;
        outputData.append('r');
        game->getWhitePlayer()->write(outputData);
        game->getBlackPlayer()->write(outputData);
    }
}

void TcpServer::gameWon(QByteArray& data, QTcpSocket *socket){
    qDebug() << "Won";
    std::string gameID = getProperty(data);
    int lGameID = std::stoi(gameID);

    if(m_activeGames.find(lGameID) != m_activeGames.end()){
        ChessGame* game = m_activeGames[lGameID];
        socket->write("w");
        game->getOtherSocket(socket)->write("l");
    }
}
