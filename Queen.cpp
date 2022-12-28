#include "Queen.h"


Queen::Queen(int row, int col, Color color) : Piece(row, col, color, Piece::Type::QUEEN){
    m_sign = color == Color::WHITE ? 'Q' : 'q';
}

std::vector<Pos> Queen::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;
    // Check diagonals
    std::vector<std::pair<int, int>> offsets{ {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for(auto& offset : offsets){
        int row = m_pos.row + offset.first, col = m_pos.col + offset.second;
        while(row >= 0 && row < 8 && col >= 0 && col < 8){
            if(board[row][col] != nullptr){
                if(m_color != board[row][col]->getColor())
                    moves.push_back({ row, col });
                break;
            }
            moves.push_back({ row, col });
            row += offset.first;
            col += offset.second;
        }
    }

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
                    moves.push_back({ row,  m_pos.col });
                break;
            }
            moves.push_back({ row, m_pos.col });
            row += (1 * i);
        }
    }

    return moves;
}
