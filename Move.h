#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"
#include "Globals.h"

class Move{
public:
    Move(Piece* pieceToMove, Piece* pieceCaptured, Pos initialPos, MoveType moveType, bool firstMove) :
        m_pieceMoved(pieceToMove), m_pieceCaptured(pieceCaptured), m_initialPos(initialPos),
        m_moveType(moveType), m_firstMove(firstMove){}

    Piece* getMovedPiece(){ return m_pieceMoved; }
    Piece* getCapturedPiece(){ return m_pieceCaptured; }
    Pos getInitialPos() { return m_initialPos; }
    MoveType getMoveType() { return m_moveType; }
    bool isFirstMove() { return m_firstMove; }
private:
    Piece* m_pieceMoved;
    Piece* m_pieceCaptured;
    Pos m_initialPos;
    MoveType m_moveType;
    bool m_firstMove;
};

#endif // MOVE_H
