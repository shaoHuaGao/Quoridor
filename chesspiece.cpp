#include "chesspiece.h"

ChessPiece::ChessPiece()
{
}
ChessPiece::ChessPiece( bool faction)
{
     _redFaction = faction;
}

ChessPiece::ChessPiece(bool faction, int row, int col)
{
    _redFaction = faction; _row = row; _col = col;
}
