#include "LogicController.h"

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Rook.h"
#include "Queen.h"

LogicController::LogicController() : m_turn(Color::WHITE), m_state(GameState::NONE){
    initBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    handleGameState();
}

void LogicController::initBoard(std::string fen){
    m_board.resize(8);
    for(auto& b : m_board)
        b.resize(8);

    int row = 0, col = 0;
    for(auto& f : fen){
        if(col >= 8)
            row += col / 8, col = 0;

        Piece* piece = nullptr;
        Color color = (f >= 65 && f <= 90) ? Color::WHITE : Color::BLACK;
        switch(f){
            case 'R': case 'r':
                piece = new Rook(row, col, color); break;
            case 'B': case 'b':
                piece = new Bishop(row, col, color); break;
            case 'N': case 'n':
                piece = new Knight(row, col, color); break;
            case 'Q': case 'q':
                piece = new Queen(row, col, color); break;
            case 'K': case 'k':
                piece = new King(row, col, color); break;
            case 'P': case 'p':
                piece = new Pawn(row, col, color); break;
            case '/':
                continue;
            default:
                col += static_cast<int>(f - 48);
                continue;
        }
        m_board[row][col++] = piece;
    }
    m_blackKing = dynamic_cast<King*>(m_board[0][4]);
    m_whiteKing = dynamic_cast<King*>(m_board[7][4]);
}

Move *LogicController::isSpecialMove(Pos from, Pos to){
    Piece* pieceToMove = m_board[from.row][from.col];
    Piece* toPiece = m_board[to.row][to.col];
    if(toPiece != nullptr && pieceToMove->getType() == Piece::Type::KING &&
            toPiece->getType() == Piece::Type::ROOK &&
            pieceToMove->getColor() == toPiece->getColor())
        return castlingMove(dynamic_cast<King*>(pieceToMove), dynamic_cast<Rook*>(toPiece));

    // TODO: enpassant and promotion
    return nullptr;
}

Move* LogicController::makeMove(Pos from, Pos to){
    Move* move = nullptr;
    if((move = isSpecialMove(from, to)) != nullptr){
        return move;
    }


    move = new Move(m_board[from.row][from.col], m_board[to.row][to.col], from, MoveType::NORMAL, m_board[from.row][from.col]->isFirstMove());
    m_board[from.row][from.col]->makeMove(to.row, to.col);
    m_board[to.row][to.col] = m_board[from.row][from.col];

    m_board[from.row][from.col] = nullptr;
    return move;
}

Move* LogicController::castlingMove(King* king, Rook* rook){
    Pos kingPos = king->getPos(), rookPos = rook->getPos();;
    int diff = kingPos.col - rookPos.col;
    if(diff > 0){
        // Castling king side
        Move* move = makeMove(kingPos, Pos(kingPos.row, kingPos.col - 2));
        Move* move2 = makeMove(rookPos, Pos(rookPos.row, king->getPos().col + 1));
        delete move; delete move2;
    }else{
        Move* move = makeMove(kingPos, Pos(kingPos.row, kingPos.col + 2));
        Move* move2 = makeMove(rookPos, Pos(rookPos.row, king->getPos().col - 1));
        delete move; delete move2;
    }
    return new Move(king, rook, kingPos, MoveType::CASTLING, true);
}

void LogicController::unmakeMove(Move *move){
    auto initialPos = move->getInitialPos();
    if(move->getMoveType() == MoveType::CASTLING){
        Piece* king = move->getMovedPiece(), *rook = move->getCapturedPiece();
        int diff = king->getCol() - rook->getCol(), toSquareCol = diff > 0 ? 7 : 0;
        m_board[king->getRow()][king->getCol()] = nullptr;
        m_board[rook->getRow()][rook->getCol()] = nullptr;

        m_board[initialPos.row][initialPos.col] = king;
        m_board[initialPos.row][toSquareCol] = rook;
        king->setPos(initialPos);
        rook->setPos(Pos(initialPos.row, toSquareCol));
        king->setFirstMove(true);
        rook->setFirstMove(true);
        return;
    }


    auto toPos = move->getMovedPiece()->getPos();
    move->getMovedPiece()->setPos(Pos(initialPos.row, initialPos.col));
    m_board[initialPos.row][initialPos.col] = m_board[toPos.row][toPos.col];
    m_board[toPos.row][toPos.col] = move->getCapturedPiece();

    if(m_board[toPos.row][toPos.col] != nullptr)
        m_board[toPos.row][toPos.col]->setPos(toPos);
    if(move->isFirstMove())
        m_board[initialPos.row][initialPos.col]->setFirstMove(true);
    delete move;
}

void LogicController::swapTurns(){
    m_turn = m_turn == Color::WHITE ? Color::BLACK : Color::WHITE;
    handleGameState();
}

void LogicController::handleGameState(){
    m_possibleMoves.clear();
    Pos kingPos = m_turn == Color::WHITE ? m_whiteKing->getPos() : m_blackKing->getPos();
    m_state = checkForChecks(kingPos);
}

void LogicController::generatePossibleMoves(){
    for(auto& row : m_board){
        for(auto& piece : row){
            if(piece == nullptr || m_turn != piece->getColor())
                continue;

            auto moves = piece->possibleMoves(m_board);
            for(auto& m : moves)
                m_possibleMoves.insert(std::make_pair(piece->getPos(), m));
        }
    }
}

GameState LogicController::checkForChecks(Pos kingPos){
    King* king = dynamic_cast<King*>(m_board[kingPos.row][kingPos.col]);
    GameState state = GameState::NONE;

    auto kingPossibleMoves = king->possibleMoves(m_board);
    auto coveredSquares = king->coveredSquares(m_board);
    int attackingPieces = coveredSquares[kingPos.row][kingPos.col];

    if(attackingPieces > 1 && kingPossibleMoves.empty())
        state = GameState::CHECKMATE;
    else if(attackingPieces > 1)
        state = GameState::CHECK;
    else{
        int possibleMovesCounter = 0;
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                Piece* piece = m_board[i][j];
                if(piece == nullptr || piece->getColor() != king->getColor())
                    continue;

                if(!isChecked(piece, king))
                    ++possibleMovesCounter;
            }
        }
        if(possibleMovesCounter == 0 && kingPossibleMoves.empty() && attackingPieces == 1)
            state = GameState::CHECKMATE;
        else if(possibleMovesCounter == 0 && kingPossibleMoves.empty() && attackingPieces == 0)
            state = GameState::STALEMATE;
        else if(attackingPieces != 0)
            state = GameState::CHECK;
        return state;
    }

    for(auto& move : kingPossibleMoves)
        m_possibleMoves.insert(std::make_pair(kingPos, move));
    return state;
}

bool LogicController::isChecked(Piece* piece, King* king){
    auto moves = piece->possibleMoves(m_board);
    if(moves.empty())
        return true;

    int legalMovesCount = 0;
    Pos piecePos = piece->getPos();
    // Check if removing the piece will put king in danger
    if(piece->getType() != Piece::Type::KING){
        // Make pseudo move
        auto ptr = m_board[piecePos.row][piecePos.col];
        m_board[piecePos.row][piecePos.col] = nullptr;

        if(king->isKingSafe(m_board)){
            m_board[piecePos.row][piecePos.col] = ptr;
            for(auto& move : moves){
                m_possibleMoves.insert(std::make_pair(piece->getPos(), move));
                ++legalMovesCount;
            }
            return legalMovesCount == 0;
        }
        m_board[piecePos.row][piecePos.col] = ptr;
    }

    for(Pos move : moves){
        Move* movePiece = makeMove(piecePos, move);
        if(king->isKingSafe(m_board)){
            ++legalMovesCount;
            m_possibleMoves.insert(std::make_pair(piecePos, move));
        }
        unmakeMove(movePiece);
    }
    return legalMovesCount == 0;
}
