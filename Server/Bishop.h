#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece
{
public:
    Bishop(int row, int col, Color color);
    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board) override;
};

#endif // BISHOP_H
