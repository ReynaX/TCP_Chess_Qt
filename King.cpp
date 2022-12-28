#include "King.h"

#include <vector>

#include "Pawn.h"

King::King(int row, int col, Color color) : Piece(row, col, color, Piece::Type::KING){
    m_sign = color == Color::WHITE ? 'K' : 'k';
}

std::vector<Pos> King::possibleMoves(const std::vector<std::vector<Piece*>>& board){
    std::vector<Pos> moves;

    auto covered = coveredSquares(board);

    for(int i = -1; i <= 1; ++i){
        for(int j = -1; j <= 1; ++j){
            // King cannot move to its square
            if(i == m_pos.row && j == m_pos.col)
                continue;

            int row = m_pos.row + i, col = m_pos.col + j;
            if(row >= 0 && row < 8 && col >= 0 && col < 8){
                if((board[row][col] == nullptr || m_color != board[row][col]->getColor()) && covered[row][col] == 0)
                    moves.push_back({ row, col });
            }
        }
    }

    if(covered[m_pos.row][m_pos.col] > 0)
        return moves;
    // TODO: castling check
    if(m_firstMove && (m_pos.row == 0 || m_pos.row == 7)){
        Piece* rook = board[m_pos.row][0];
        if(rook != nullptr && m_color == rook->getColor() &&
                rook->getType() == Piece::Type::ROOK && rook->isFirstMove()){
            int col = m_pos.col - 1;
            while(col > 0 && board[m_pos.row][col] == nullptr && covered[m_pos.row][col] == 0) --col;
            if(col == 0) moves.push_back(Pos(m_pos.row, 0));
        }

        rook = board[m_pos.row][7];
        if(rook != nullptr && m_color == rook->getColor() &&
                rook->getType() == Piece::Type::ROOK && rook->isFirstMove()){
            int col = m_pos.col + 1;
            while(col < 7 && board[m_pos.row][col] == nullptr && covered[m_pos.row][col] == 0) ++col;
            if(col == 7) moves.push_back(Pos(m_pos.row, 7));
        }
    }
    return moves;
}

std::vector<Pos> King::attackingMoves(){
    std::vector<Pos> moves;
    for(int i = -1; i <= 1; ++i){
        for(int j = -1; j <= 1; ++j){
            if(i == 0 && j == 0)
                continue;
            if(m_pos.row + i >= 0 && m_pos.row + i < 8 &&
                    m_pos.col + j >= 0 && m_pos.col + j < 8){
                moves.push_back({ m_pos.row + i, m_pos.col + j });
            }
        }
    }
    return moves;
}

std::vector<std::vector<int>> King::coveredSquares(const std::vector<std::vector<Piece*>>& board){
    std::vector<std::vector<int>> covered(8, std::vector<int>(8));

    auto b = board;
    b[m_pos.row][m_pos.col] = nullptr;

    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            Piece* piece = board[i][j];
            if(piece == nullptr || m_color == piece->getColor())
                continue;

            // Swap color of current piece
            piece->setColor(piece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE);

            std::vector<Pos> moves;
            if(piece->getType() == Piece::Type::PAWN)
                moves = dynamic_cast<Pawn*>(piece)->attackingMoves(b);
            else if(piece->getType() == Piece::Type::KING)
                moves = dynamic_cast<King*>(piece)->attackingMoves();
            else moves = piece->possibleMoves(b);

            for(Pos move : moves)
                ++covered[move.row][move.col];

            piece->setColor(piece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE);
        }
    }
    b[m_pos.row][m_pos.col] = this;
    return covered;
}

bool King::isKingSafe(const std::vector<std::vector<Piece*>>& board){
    // Check if king is being attacked horizontally
    std::vector<int> offsets{ -1, 1 };
    for(auto& offset : offsets){
        int col = m_pos.col + offset;
        while(col >= 0 && col < 8){
            Piece* piece = board[m_pos.row][col];
            col += offset;
            if(piece == nullptr)
                continue;
            // Check if opposite color piece can attack the king horizontally
            if(m_color != piece->getColor() && (piece->getType() == Piece::Type::ROOK || piece->getType() == Piece::Type::QUEEN))
                return false;
            break;
        }
    }
    // Check if king is being attacked vertically
    offsets = { -1, 1 };
    for(auto& offset : offsets){
        int row = m_pos.row + offset;
        while(row >= 0 && row < 8){
            Piece* piece = board[row][m_pos.col];
            row += offset;
            if(piece == nullptr)
                continue;
            // Check if opposite color piece can attack the king vertically
            if(m_color != piece->getColor() && (piece->getType() == Piece::Type::ROOK || piece->getType() == Piece::Type::QUEEN))
                return false;
            break;
        }
    }

    // Check if king is being attacked diagonally
    std::vector<std::pair<int, int>> diagOffsets{{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for(auto& diag: diagOffsets){
        int row = m_pos.row + diag.first , col = m_pos.col + diag.second;
        while(row >= 0 && row < 8 && col >= 0 && col < 8){
            if(board[row][col] != nullptr){
                Piece* piece = board[row][col];
                if(m_color != piece->getColor() && (piece->getType() == Piece::Type::BISHOP || piece->getType() == Piece::Type::QUEEN))
                    return false;
                break;
            }
            row += diag.first;
            col += diag.second;
        }
    }

    // Check for knight attacks
    for(int r = -1; r <= 1; r += 2){
        for(int c = -1; c <= 1; c += 2){
            if(m_pos.row + r * 2 >= 0 && m_pos.row + r * 2 < 8 && m_pos.col + c >= 0 && m_pos.col + c < 8){
                Piece* piece = board[m_pos.row + r * 2][m_pos.col+ c];
                if(piece != nullptr && piece->getColor() != m_color && piece->getType() == Piece::Type::KNIGHT)
                    return false;
            }

            if(m_pos.row + r >= 0 && m_pos.row + r < 8 && m_pos.col + c * 2 >= 0 && m_pos.col + c * 2 < 8){
                Piece* piece = board[m_pos.row + r][m_pos.col + c * 2];
                if(piece != nullptr && piece->getColor() != m_color && piece->getType() == Piece::Type::KNIGHT)
                    return false;
            }
        }
    }

    // Check for pawn attacks
    int pawnAttackDirection = m_color == Color::WHITE ? -1 : 1;
    if(m_pos.row + pawnAttackDirection >= 0 && m_pos.row + pawnAttackDirection < 8){
        if(m_pos.col - 1 >= 0){
            Piece* piece = board[m_pos.row + pawnAttackDirection][m_pos.col - 1];
            if(piece != nullptr && piece->getColor() != m_color && piece->getType() == Piece::Type::PAWN)
                return false;
        }

        if(m_pos.col + 1 < 8){
            Piece* piece = board[m_pos.row + pawnAttackDirection][m_pos.col + 1];
            if(piece != nullptr && piece->getColor() != m_color && piece->getType() == Piece::Type::PAWN)
                return false;
        }
    }
    // TODO: Check from king
    return true;
}
