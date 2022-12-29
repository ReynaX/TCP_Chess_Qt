#ifndef LOGICCONTROLLER_H
#define LOGICCONTROLLER_H

#include <unordered_map>

#include "Move.h"
#include "Piece.h"

class King;
class Rook;
class LogicController
{
public:
    LogicController();

    Move* makeMove(Pos from, Pos to);
    void unmakeMove(Move* move);
    void swapTurns();
    std::string getBoard();
    const std::unordered_multimap<Pos, Pos, PosHash>& getPossibleMoves();
    GameState getState();
private:

    std::vector<std::vector<Piece*>> m_board;
    void initBoard(std::string fen);

    void handleGameState();
    GameState checkForChecks(Pos kingPos);
    bool isChecked(Piece* piece, King* king);
    void generatePossibleMoves();

    Move* isSpecialMove(Pos from, Pos to);
    Move* castlingMove(King* king, Rook* rook);
    Color m_turn;
    GameState m_state;

    King* m_whiteKing;
    King* m_blackKing;
    std::unordered_multimap<Pos, Pos, PosHash> m_possibleMoves;

    friend class ChessBoard;
};
#endif // LOGICCONTROLLER_H
