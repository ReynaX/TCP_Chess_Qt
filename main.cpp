#include <QApplication>

#include "Chessboard.h"
#include "LogicController.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessBoard w(new LogicController());
    w.show();
    return a.exec();
}
