#include <QApplication>

#include "Chessboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessBoard w;
    w.show();
    return a.exec();
}
