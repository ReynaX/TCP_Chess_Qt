#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChessBoard; }
QT_END_NAMESPACE

class BoardSquare;
class LogicController;
class JoinGameDialog;
class Move;
class QLabel;
class QMediaPlayer;
class TcpSocket;
class ChessBoard : public QMainWindow
{
    Q_OBJECT
public:
    ChessBoard(LogicController* controller = nullptr, QWidget *parent = nullptr);
    ~ChessBoard();

    QVector<BoardSquare*> m_boardSquares;
public slots:
    void onChessBoardClicked();
    void onHostGameActionClicked();
    void onJoinGameActionClicked();
    void onDisconnectActionClicked();
     // Called when other player has moved. Method gets called when socket emits <code>moveSignal</code> signal.
    void onOtherPlayerMovement(std::string from, std::string to);
    // Called when this or other socket from the same game disconnects
    void disconnectedFromServer();
private:
    Ui::ChessBoard* ui;
    LogicController* m_logicController;
    BoardSquare* m_squareSelected;

    TcpSocket* m_socket;
    bool m_playerMove;

    void selectPossibleMoves();
    void unselectPossibleMoves();
    void removeIcon(Move* move, BoardSquare* squarePressed);
    void gameFinished();

    void setupMenu();

    void setupStatusbar(std::string gameIDLabel, std::string infoLabel, std::string moveLabel);

    void connectToServer();
    void clearBoard();

    void resetBoard();
    void garbageCollector();

    QLabel* m_gameIDLabel;
    QLabel* m_infoLabel;
    QLabel* m_moveLabel;
};
#endif // CHESSBOARD_H
