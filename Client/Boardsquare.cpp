#include "Boardsquare.h"

BoardSquare::BoardSquare(Color color, int row, int col)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_row = row;
    m_col = col;
    m_color = color;

    if(color == Color::WHITE)
        this->setStyleSheet("background-color: rgba(144, 130, 109, 255); border: 0px;");
    else this->setStyleSheet("background-color: rgba(108, 81, 60, 255); border: 0px;");
}

QSize BoardSquare::sizeHint() const{
    QSize size = QPushButton::sizeHint();


    size.rheight() = std::min(52, size.width() + 50);
    size.rwidth() = size.rheight();
    return size;
}

int BoardSquare::getRow(){
    return m_row;
}

int BoardSquare::getCol(){
    return m_col;
}

void BoardSquare::unselect(){
    if(m_color == Color::WHITE)
        this->setStyleSheet("background-color: rgba(144, 130, 109, 255); border: 0px;");
    else this->setStyleSheet("background-color: rgba(108, 81, 60, 255); border: 0px;");
}

void BoardSquare::selectIcon(char type, Color color){
    QString c = color == Color::WHITE ? "white" : "black";
    QString t;
    switch(type){
        case 'B': case 'b': t = "bishop"; break;
        case 'Q': case 'q': t = "queen"; break;
        case 'P': case 'p': t = "pawn"; break;
        case 'N': case 'n': t = "knight"; break;
        case 'R': case 'r': t = "rook"; break;
        case 'K': case 'k': t = "king"; break;
    }

    QString url = ":/icons/Resources/" + c + "_" + t + "_icon.png";
    QPixmap pix(url);
    setIcon(QIcon(pix));
    setIconSize(pix.rect().size());
}

void BoardSquare::removeIcon(){
    setIcon(QIcon());
}

