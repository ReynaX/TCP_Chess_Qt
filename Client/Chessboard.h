#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QMainWindow>
#include "Globals.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChessBoard; }
QT_END_NAMESPACE

class BoardSquare;
class JoinGameDialog;
class QLabel;
class TcpSocket;
class ChessBoard : public QMainWindow
{
    Q_OBJECT
public:
    ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();

    QVector<BoardSquare*> m_boardSquares;
public slots:
    /*
     * Check whether current client can move.
     * If there is no pressed button yet and currently clicked button contains a piece then
     * <code>selectPossibleMoves</code> method gets called.
     * If there already is a pressed button and currently clicked button is among possible moves for
     * pressed piece then the move is performed and server gets noticed.
     */
    void onChessBoardClicked();

    // Connect to the server and host the game.
    void onHostGameActionClicked();
    // Join the game if the game exists
    void onJoinGameActionClicked();
    // Disconnect from server
    void onDisconnectActionClicked();
     // Called when other player has moved. Method gets called when socket emits <code>moveSignal</code> signal.
//    void onOtherPlayerMovement(std::string from, std::string to);
    // Called when this or other socket from the same game disconnects
    void disconnectedFromServer();

    void setupBoard(std::string board);
private:
    Ui::ChessBoard* ui;
    BoardSquare* m_squareSelected;
    std::unordered_multimap<Pos, Pos, PosHash> m_possibleMoves;
    GameState m_gameState;
    Color m_color;

    TcpSocket* m_socket;
    bool m_playerMove;
    std::string m_board;

    void selectPossibleMoves();
    void unselectPossibleMoves();
    void gameFinished();

    void setupMenu();

    void setupStatusbar(std::string gameIDLabel, std::string infoLabel, std::string moveLabel);

    void connectToServer();
    void clearBoard();

    QLabel* m_gameIDLabel;
    QLabel* m_infoLabel;
    QLabel* m_moveLabel;
};
#endif // CHESSBOARD_H
