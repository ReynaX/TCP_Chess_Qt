#include "Chessboard.h"
#include "ui_Chessboard.h"

#include <QLabel>
#include <QMessageBox>

#include "Boardsquare.h"
#include "joingamedialog.h"
#include "TcpSocket.h"
//#include "SoundPlayer.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChessBoard), m_squareSelected(nullptr), m_socket(nullptr){
    ui->setupUi(this);

    for(int i = 0; i < 64; ++i){
        int row = i / 8, col = i % 8;
        Color color = ((i + i / 8) % 2 == 0) ? Color::WHITE : Color::BLACK;

        BoardSquare* square = new BoardSquare(color, row, col);
//        connect(square, &QPushButton::pressed, this, &ChessBoard::onChessBoardClicked);
        ui->mainLayout->addWidget(square, i / 8, i % 8, 1, 1);
        m_boardSquares.push_back(square);

    }

    setWindowTitle("Chess");

    setupMenu();
    setupBoard("rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR");

    m_playerMove = false;
    m_moveLabel = new QLabel(this);
    m_infoLabel = new QLabel("Not connected to the server", this);
    m_gameIDLabel = new QLabel("Game ID: ", this);

    ui->statusbar->addWidget(m_gameIDLabel, 10);
    ui->statusbar->addWidget(m_moveLabel, 5);
    ui->statusbar->addPermanentWidget(m_infoLabel);
}

ChessBoard::~ChessBoard(){
    delete ui;

    for(auto& square: m_boardSquares)
        delete square;
    m_boardSquares.clear();
}

void ChessBoard::selectPossibleMoves(){
    unselectPossibleMoves();
    int row = m_squareSelected->getRow(), col = m_squareSelected->getCol();

    Pos pos(row, col);
    for(auto move: m_possibleMoves){
        if(move.first == pos){
            int r = move.second.row, c = move.second.col;
            m_boardSquares[r * 8 + c % 8]->setStyleSheet("background-color: rgb(255, 248, 220);");
        }
    }
}

void ChessBoard::unselectPossibleMoves(){
    for(auto& square: m_boardSquares)
        square->unselect();
}

//void ChessBoard::onChessBoardClicked(){
//    BoardSquare* squarePressed = qobject_cast<BoardSquare*>(sender());

//    if(!m_playerMove)
//        return;

//    if(m_logicController->m_state == GameState::STALEMATE || m_logicController->m_state == GameState::STALEMATE)
//        return;

//    if(m_squareSelected == nullptr){
//        Piece* pieceClicked = m_logicController->m_board[squarePressed->getRow()][squarePressed->getCol()];
//        if(pieceClicked != nullptr && pieceClicked->getColor() == m_logicController->m_turn){
//            m_squareSelected = squarePressed;
//            selectPossibleMoves();
//        }
//    }else{
//        Pos fromPos(m_squareSelected->getRow(), m_squareSelected->getCol());
//        Pos toPos(squarePressed->getRow(), squarePressed->getCol());
//        Piece* pieceWaitingToMove = m_logicController->m_board[fromPos.row][fromPos.col];
//        auto moves = pieceWaitingToMove->possibleMoves(m_logicController->m_board);

//        // Iterate over possible moves and check if move is legal
//        auto& possibleMoves = m_logicController->m_possibleMoves;
//        for(auto it = possibleMoves.begin(); it != possibleMoves.end(); ++it){
//            if(it->first == fromPos && it->second == toPos){
//                Move* move = m_logicController->makeMove(fromPos, toPos);
//                m_logicController->swapTurns();
//                removeIcon(move, squarePressed);
//                m_squareSelected->removeIcon();

//                SoundPlayer::playSound(move, m_logicController->m_state);
//                m_socket->move(fromPos.toString(), toPos.toString());

//                delete move;
//                m_playerMove = false;
//                m_moveLabel->setText("Not your move");
//                break;
//            }
//        }
//        unselectPossibleMoves();
//        m_squareSelected = nullptr;

//        if(m_logicController->m_state != GameState::NONE && m_logicController->m_state != GameState::CHECK){
//            gameFinished();
//        }
//    }
//}

void ChessBoard::onHostGameActionClicked(){
    qDebug() << "Host clicked";
    if(m_socket != nullptr)
        m_socket->disconnect();
    connectToServer();
    if(m_socket != nullptr){
        m_socket->hostGame();
        setupBoard("rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR");
        m_playerMove = false;
        m_infoLabel->setText("Waiting for other player");
    }
}

void ChessBoard::onJoinGameActionClicked(){
    qDebug() << "Join clicked";
    if(m_socket != nullptr)
        m_socket->disconnect();

    connectToServer();
    if(m_socket != nullptr){
        JoinGameDialog dialog;

        int res = dialog.exec();
        setupBoard("rnbqkbnrpppppppp--------------------------------PPPPPPPPRNBQKBNR");
        if(res == QDialog::Accepted && m_socket != nullptr){
            QString gameID = dialog.getGameID();
            if(gameID.toStdString() == m_socket->getGameID()){
                setupStatusbar("Game ID: ", "", "Failed to join the game");
                return;
            }
            m_socket->setGameID(gameID.toStdString());
            m_socket->joinGame();
            connect(m_socket, &TcpSocket::joined, this, [this](){
                m_playerMove = false;
                setupStatusbar("Game ID: " + m_socket->getGameID(), "Playing...", "Not your move");
            });
        }
    }
}

void ChessBoard::onDisconnectActionClicked(){
    qDebug() << "Disconnect clicked";
    m_socket->disconnect();
    m_socket = nullptr;
    clearBoard();
    m_playerMove = false;
    m_infoLabel->setText("Disconnected from the server");
    m_moveLabel->clear();
    m_gameIDLabel->clear();
}

//void ChessBoard::onOtherPlayerMovement(std::string from, std::string to){
//    int fromPos = std::stoi(from), toPos = std::stoi(to);
//    qDebug() << "Other player received a move";
//    Move* move = m_logicController->makeMove(Pos(fromPos / 8, fromPos % 8), Pos(toPos / 8, toPos % 8));
//    m_logicController->swapTurns();

//    m_boardSquares[fromPos]->removeIcon();
//    removeIcon(move, m_boardSquares[toPos]);

//    SoundPlayer::playSound(move, m_logicController->m_state);
//    m_playerMove = true;
//    m_moveLabel->setText("Your move");
//    delete move;
//}

void ChessBoard::disconnectedFromServer(){
    if(m_socket != nullptr){
        m_socket->disconnectFromHost();
        delete m_socket;
    }
    clearBoard();
    m_moveLabel->clear();
    m_gameIDLabel->clear();
    m_infoLabel->setText("Disconnected from server");
    m_socket = nullptr;
}


void ChessBoard::gameFinished(){
    std::string currentPlayerMessage, otherPlayerMessage;

    if(m_gameState == GameState::STALEMATE){
        currentPlayerMessage = "Draw!";
        otherPlayerMessage = "Draw!";
        m_socket->drawGame();
    }else{
        if(m_playerMove){
            if(m_color == Color::WHITE)
                m_socket->wonGame("black");
            else
                m_socket->wonGame("white");
        }else{
            if(m_color == Color::WHITE)
                m_socket->wonGame("white");
            else
                m_socket->wonGame("black");
        }
    }
}

void ChessBoard::setupMenu(){
    QMenu* menu = new QMenu(this);
    menu->setTitle("Game");
    QAction* hostGameAction = new QAction("Host game", menu);
    connect(hostGameAction, SIGNAL(triggered()), this, SLOT(onHostGameActionClicked()));

    QAction* joinGameAction = new QAction("Join game", menu);
    connect(joinGameAction, SIGNAL(triggered()), this, SLOT(onJoinGameActionClicked()));

    QAction* disconnectAction = new QAction("Disconnect", menu);
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(onDisconnectActionClicked()));

    menu->addAction(hostGameAction);
    menu->addAction(joinGameAction);
    menu->addAction(disconnectAction);
    ui->menubar->addMenu(menu);
}

void ChessBoard::setupStatusbar(std::string gameIDLabel, std::string infoLabel, std::string moveLabel){
    m_gameIDLabel->setText(QString::fromStdString(gameIDLabel));
    m_infoLabel->setText(QString::fromStdString(infoLabel));
    m_moveLabel->setText(QString::fromStdString(moveLabel));
}

void ChessBoard::connectToServer(){
    m_socket = new TcpSocket();
    if(!(m_socket->state() == QAbstractSocket::ConnectedState || m_socket->state() == QAbstractSocket::ConnectingState)){
        QMessageBox box;
        box.setWindowTitle("Server error");
        box.setText("Failed to connect to a server!");
        box.exec();
        delete m_socket;
        m_socket = nullptr;
    }
    else{
        m_infoLabel->setText("Connected to a server");
//        connect(m_socket, &TcpSocket::moveSignal, this, &ChessBoard::onOtherPlayerMovement);
        connect(m_socket, &TcpSocket::enableMovement, this, [this](){ m_playerMove = true;
            setupStatusbar("Game ID: " + m_socket->getGameID(), "Playing...", "Your move");});
        connect(m_socket, &TcpSocket::disconnectedFromServer, this, &ChessBoard::disconnectedFromServer);
        connect(m_socket, &TcpSocket::gameIDChanged, this, [this](){
            m_gameIDLabel->setText("Game ID: " + QString::fromStdString(m_socket->getGameID()));});
        connect(m_socket, &TcpSocket::won, this, [this](){
            m_infoLabel->setText("Congratulations, you won!"); m_playerMove = false; });
        connect(m_socket, &TcpSocket::lost, this, [this](){
            m_infoLabel->setText("Game over, you lost!"); m_playerMove = false; });
    }
}

void ChessBoard::clearBoard(){
    for(auto& square: m_boardSquares)
        square->removeIcon();
}

void ChessBoard::setupBoard(std::string board){
    m_board = board;
    for(int i = 0; i < 64; ++i){
        if(m_board[i] == '-'){
            m_boardSquares[i]->removeIcon();
        }else{
            Color color = (m_board[i] >= 65 && m_board[i] <= 90) ? Color::WHITE : Color::BLACK;
            m_boardSquares[i]->selectIcon(m_board[i], color);
        }
    }
}

//void ChessBoard::garbageCollector(){
//    delete m_logicController;
//    for(auto& square: m_boardSquares)
//        delete square;
//    m_boardSquares.clear();
//}
