#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(int row, int col, Color color);

    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board);
};

#endif // QUEEN_H
