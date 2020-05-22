#ifndef CHESSPIECE_H
#define CHESSPIECE_H


class ChessPiece
{

public:
    ChessPiece();
    ChessPiece(bool faction);
    ChessPiece(bool faction, int row, int col);
    int _row;
    int _col;
    int _radius = 30;
    bool _redFaction;
private:

};

#endif // CHESSPIECE_H
