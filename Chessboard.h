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
//    void onChessBoardClicked();
    void onHostGameActionClicked();
    void onJoinGameActionClicked();
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

//    void garbageCollector();

    QLabel* m_gameIDLabel;
    QLabel* m_infoLabel;
    QLabel* m_moveLabel;
};
#endif // CHESSBOARD_H
