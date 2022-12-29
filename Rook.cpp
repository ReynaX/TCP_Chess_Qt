#include "Rook.h"

Rook::Rook(int row, int col, Color color) : Piece(row, col, color, Piece::Type::ROOK){
    m_sign = color == Color::WHITE ? 'R' : 'r';
}

std::vector<Pos> Rook::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;
    // Check horizontal
    for(int i = -1; i <= 1; i += 2){
        int col = m_pos.col + (1 * i);
        while(col >= 0 && col < 8){
            // Other piece has been ancountered
            if(board[m_pos.row][col] != nullptr){
                if(m_color != board[m_pos.row][col]->getColor())
                    moves.push_back({ m_pos.row, col });
                break;
            }
            moves.push_back({ m_pos.row, col });
            col += (1 * i);
        }
    }

    // Check vertical
    for(int i = -1; i <= 1; i += 2){
        int row = m_pos.row + (1 * i);
        while(row >= 0 && row < 8){
            // Other piece has been ancountered
            if(board[row][m_pos.col] != nullptr){
                if(m_color != board[row][m_pos.col]->getColor())
                    moves.push_back({ row, m_pos.col });
                break;
            }
            moves.push_back({ row, m_pos.col });
            row += (1 * i);
        }
    }
    return moves;
}
