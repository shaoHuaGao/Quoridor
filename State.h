#ifndef NODE_H
#define NODE_H
#include <QPoint>
#include "chesspiece.h"
#include "barrier.h"
#include <QPoint>
#include <QVector>
#include "type.h"

class State
{
public:

    State();
    State(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn);
    ChessPiece chess[2];       //棋子
    Barrier barrier[16];       //障碍位置
    int moveNumber;            //可移动步数
    QVector<mover> nextMoves;   //下一步走法
    mover lastMove;             //上一步走法
    bool redTurn;

    bool chessCanMove(ChessPiece _chess[2], Barrier _barrier[16], QPoint point, bool _redTurn);
    bool barrierCanMove(ChessPiece _chess[2], Barrier _barrier[16], QPoint point, int lastNumber);
    QVector<mover> getLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn);
    QVector<mover> chessLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn);
    QVector<mover> barrierLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn);
    bool isGameOver();



};

#endif // NODE_H
