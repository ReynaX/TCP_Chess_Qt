#include "Knight.h"


Knight::Knight(int row, int col, Color color) : Piece(row, col, color, Piece::Type::KNIGHT){
    m_sign = color == Color::WHITE ? 'N' : 'n';
}

std::vector<Pos> Knight::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;
    std::vector<std::pair<int, int>> offsets{ {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2 }, {-1, 2}, {1, -2}, {1, 2} };

    for(auto& offset : offsets){
        int row = m_pos.row + offset.first, col = m_pos.col + offset.second;
        if(row >= 0 && row < 8 && col >= 0 && col < 8){
            if(board[row][col] == nullptr || m_color != board[row][col]->getColor())
                moves.push_back({ row, col });
        }
    }
    return moves;
}
