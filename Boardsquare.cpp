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

void BoardSquare::selectIcon(Piece::Type type, Color color){
    QString c = color == Color::WHITE ? "white" : "black";
    QString t;
    switch(type){
        case Piece::Type::BISHOP: t = "bishop"; break;
        case Piece::Type::QUEEN: t = "queen"; break;
        case Piece::Type::PAWN: t = "pawn"; break;
        case Piece::Type::KNIGHT: t = "knight"; break;
        case Piece::Type::ROOK: t = "rook"; break;
        case Piece::Type::KING: t = "king"; break;
    }

    QString url = ":/icons/Resources/" + c + "_" + t + "_icon.png";
    QPixmap pix(url);
    setIcon(QIcon(pix));
    setIconSize(pix.rect().size());
}

void BoardSquare::removeIcon(){
    setIcon(QIcon());
}

