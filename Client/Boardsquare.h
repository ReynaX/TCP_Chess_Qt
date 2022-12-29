#ifndef BOARDSQUARE_H
#define BOARDSQUARE_H

#include <QPushButton>
#include <QObject>

#include "Globals.h"

class BoardSquare : public QPushButton
{
    Q_OBJECT
public:
    BoardSquare(Color color, int row, int col);

    int getRow();
    int getCol();

    void unselect();

    // Assign the icon of given type and color to this square
    void selectIcon(char type, Color color);
    // Remove icon from this square
    void removeIcon();
protected:
    QSize sizeHint() const;
private:
    int m_row;
    int m_col;
    Color m_color;
};

#endif // BOARDSQUARE_H
