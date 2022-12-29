#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
public:
    King(int row, int col, Color color);

    std::vector<Pos> possibleMoves(const std::vector<std::vector<Piece*>>& board);

    std::vector<Pos> attackingMoves();

    std::vector<std::vector<int>> coveredSquares(const std::vector<std::vector<Piece*>>& board);

    bool isKingSafe(const std::vector<std::vector<Piece*>>& board);

};

#endif // KING_H
