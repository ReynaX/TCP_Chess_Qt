#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece
{
public:
    Rook(int row, int col, Color color);

    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board);
};

#endif // ROOK_H
