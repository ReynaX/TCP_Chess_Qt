#include "Pawn.h"

Pawn::Pawn(int row, int col, Color color) : Piece(row, col, color, Piece::Type::PAWN){
    m_sign = color == Color::WHITE ? 'P' : 'p';
}

std::vector<Pos> Pawn::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;

    int forwardMoves = m_firstMove ? 2 : 1;
    int direction = m_color == Color::WHITE ? -1 : 1;

    // Forward moves
    for(int i = 1; i <= forwardMoves; ++i){
        int row = m_pos.row + (i * direction);
        if(row >= 0 && row < 8){
            if(board[row][m_pos.col] != nullptr)
                break;
            moves.push_back({ row, m_pos.col });
        }
    }

    int rowForward = m_pos.row + direction;
    if(rowForward < 0 || rowForward >= 8)
        return moves;

    int col = m_pos.col - 1;
    if(m_pos.col - 1 >= 0 && board[rowForward][col] != nullptr && m_color != board[rowForward][col]->getColor())
        moves.push_back({ rowForward, col });
    col = m_pos.col + 1;
    if(m_pos.col + 1 < 8 && board[rowForward][col] != nullptr && m_color != board[rowForward][col]->getColor())
        moves.push_back({ rowForward, col });

    return moves;
}

std::vector<Pos> Pawn::attackingMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;

    int direction = m_color == Color::WHITE ? 1 : -1;
    int rowForward = m_pos.row + direction;
    if(rowForward >= 0 && rowForward < 8){
        if(m_pos.col - 1 >= 0)
            moves.push_back({ rowForward, m_pos.col - 1 });
        if(m_pos.col + 1 < 8)
            moves.push_back({ rowForward, m_pos.col + 1 });
    }
    return moves;
}
