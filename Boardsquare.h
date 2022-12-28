#ifndef BOARDSQUARE_H
#define BOARDSQUARE_H

#include <QPushButton>
#include <QObject>

#include "Globals.h"
#include "Piece.h"

class BoardSquare : public QPushButton
{
    Q_OBJECT
public:
    BoardSquare(Color color, int row, int col);

    int getRow();
    int getCol();

    void unselect();

    void selectIcon(Piece::Type type, Color color);
    void removeIcon();
protected:
    QSize sizeHint() const;
private:
    int m_row;
    int m_col;
    Color m_color;
};

#endif // BOARDSQUARE_H
