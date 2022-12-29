#ifndef PIECE_H
#define PIECE_H

#include <QVector>

#include "Globals.h"

class Piece
{
public:
    enum class Type { ROOK, BISHOP, KNIGHT, KING, QUEEN, PAWN};
    Piece(int row, int col, Color color, Type type) : m_pos(row, col), m_color(color), m_type(type), m_sign('-'), m_firstMove(true){}

    int getRow(){ return m_pos.row; }
    void setRow(int row){ m_pos.row = row; }

    int getCol(){ return m_pos.col; }
    void setCol(int col){ m_pos.col = col; }

    Pos getPos(){ return m_pos; }
    void setPos(Pos pos){ m_pos = pos; }

    Color getColor(){ return m_color; }
    void setColor(Color color){ m_color = color; }

    Type getType() { return m_type; }
    void setType(Type type) { m_type = type; }
    char getSign(){ return m_sign; }
    virtual std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board) = 0;

    virtual void makeMove(int row, int col){
        m_pos.row = row;
        m_pos.col = col;
        m_firstMove = false;
    }

    bool isFirstMove(){ return m_firstMove; }
    void setFirstMove(bool firstMove) { m_firstMove = firstMove; }
protected:
    Pos m_pos;
    Color m_color;
    Type m_type;

    char m_sign;

    bool m_firstMove;
};

#endif // PIECE_H
