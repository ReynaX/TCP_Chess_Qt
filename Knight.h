#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece
{
public:
    Knight(int row, int col, Color color);

    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board);
};

#endif // KNIGHT_H
