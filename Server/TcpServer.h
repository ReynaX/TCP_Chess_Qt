#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QObject>
#include <unordered_map>

#include "ChessGame.h"

class TcpServer : QTcpServer
{
    Q_OBJECT
public:
    static int NEXT_ID;

    TcpServer(QObject* parent = nullptr);
    void startServer();
public slots:
    /* Read data from a socket.
     * Data is sent in specific order that follows given notation.
     * XY1D1Y2D2...
     * Where:
     * - X is a type of operation to perform consisting a first letter of operation(host, join, move, disconnect)
     * - Yi is a length of data to read
     * - Di is data itself
     */
    void readyRead();
    // Responsible for disconnecting both players in the game
    void disconnectBothPlayers(std::string gameID);
    /*
     *  Responsible for removing socket that disconnected from server.
     *  Also disconnects and deletes the other player from the same game.
     */
    void disconnect();
    // Accept new connection and assign signals to newly connected socket
    void acceptConnection();

private:
    // Map designed to store active games in gameID -> game structure
    std::unordered_map<int, ChessGame*> m_activeGames;

    /*
     * Read next part of data received from a socket.
     * Segment contains length of data to read and data itself.
     */
    std::string getProperty(QByteArray& data);

    // Responsible for hosting new game
    void hostNewGame(QTcpSocket* socket);
    // Responsible for assigning player to a game and sending enableMovement signal to the host
    void joinGame(QByteArray& data, QTcpSocket* socket);
    // Responsible for sending message information to other socket
    void move(QByteArray& data, QTcpSocket* socket);
    // Enable the host's movement
    void notifyOtherPlayer(QTcpSocket* socket);
    void gameDrawn(QByteArray& data);
    void gameWon(QByteArray& data, QTcpSocket* socket);

    void parsePossibleMoves(QByteArray& data, LogicController* controller);
    QByteArray parsePossibleMoves(LogicController* controller);

    void checkIfGameOver(std::string gameID, LogicController* controller, QTcpSocket* socket);
};

#endif // TCPSERVER_H
