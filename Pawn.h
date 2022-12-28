#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(int row, int col, Color color);

    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board) override;

    std::vector<Pos> attackingMoves(const std::vector<std::vector<Piece*>>& board);
};

#endif // PAWN_H
