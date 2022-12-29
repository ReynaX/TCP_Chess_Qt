#include "Bishop.h"


Bishop::Bishop(int row, int col, Color color) : Piece(row, col, color, Piece::Type::BISHOP) {
    m_sign = color == Color::WHITE ? 'B' : 'b';
}

std::vector<Pos> Bishop::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;
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
    return moves;
}

