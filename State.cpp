#include "State.h"
#include "chesspiece.h"
#include "barrier.h"

State::State()
{}
State::State(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn)
{
    chess[0] = _chess[0]; chess[1] = _chess[1];
    for (int i = 0; i < 16; i++)
    {
        barrier[i] = _barrier[i];
    }
    redTurn = _redTurn;
    nextMoves = this->getLegalMoves(chess, barrier,redTurn);
    moveNumber = nextMoves.size();
}

QVector<mover> State::getLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn)
{
    QVector<mover> moves;
    QVector<mover> moves1;
    if(_redTurn)
    {
        moves = chessLegalMoves(_chess, _barrier, _redTurn);
        moves1 = barrierLegalMoves(_chess, _barrier, _redTurn);
        moves.append(moves1);
    }
    else
    {
        moves = chessLegalMoves(_chess, _barrier, _redTurn);
        moves1 = barrierLegalMoves(_chess, _barrier, _redTurn);
        moves.append(moves1);
    }
    return moves;
}
QVector<mover> State::chessLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn)
{
    QVector<mover> moves;
    mover move;
    int i, j;
    if(_redTurn)
    {
        //记录棋子步数
        for (i = 1; i < 8; i++)
        {
            for (j = 1; j < 8; j++)
            {
                if(chessCanMove(_chess, _barrier, QPoint(i, j), _redTurn))
                {
                    move.redMove = true;
                    move.chessMove = true;
                    move.barrierChange = false;
                    move.point = QPoint(i, j);
                    moves.append(move);
                }
            }
        }

    }
    else
    {
        for (i = 1; i < 8; i++)
        {
            for (j = 1; j < 8; j++)
            {
                if(chessCanMove(_chess, _barrier, QPoint(i, j), _redTurn))
                {
                    move.redMove = false;
                    move.chessMove = true;
                    move.barrierChange = false;
                    move.point = QPoint(i, j);
                    moves.append(move);
                }
            }
        }
    }
    return moves;
}

QVector<mover> State::barrierLegalMoves(ChessPiece _chess[2], Barrier _barrier[16], bool _redTurn)
{
    QVector<mover> moves;
    mover move;
    int i, j;
    int barrierN = -1;
    //查看是否还剩障碍
    //循环查看
    if(_redTurn)
    {
        for (i = 7; i > -1; i--)
        {
            if(_barrier[i]._col == 0)
            {
                barrierN = i;
                break;
            }
        }
        if(barrierN > -1)
        {
            for(i = 1; i < 7; i++)
            {
                for (j = 1; j < 7; j ++)
                {
                    if(barrierCanMove(_chess, _barrier, QPoint(i, j), barrierN))
                    {
                        move.redMove = true;
                        move.chessMove = false;
                        move.barrierChange = false;
                        move.point = QPoint(i, j);
                        moves.append(move);
                    }
                    _barrier[barrierN]._across = false;
                    if(barrierCanMove(_chess, _barrier, QPoint(i, j), barrierN))
                    {
                        move.redMove = true;
                        move.chessMove = false;
                        move.barrierChange = true;
                        move.point = QPoint(i, j);
                        moves.append(move);
                    }
                    _barrier[barrierN]._across = true;
                }
            }
        }
    }
    else
    {
        for (i = 15; i > 7; i--)
        {
            if(_barrier[i]._col == 0)
            {
                barrierN = i;
                break;
            }
        }
        if(barrierN > 7)
        {
            for(i = 1; i < 7; i++)
            {
                for (j = 1; j < 7; j ++)
                {
                    if(barrierCanMove(_chess, _barrier, QPoint(i, j), barrierN))
                    {
                        move.redMove = false;
                        move.chessMove = false;
                        move.barrierChange = false;
                        move.point = QPoint(i, j);
                        moves.append(move);
                    }
                    _barrier[barrierN]._across = false;
                    if(barrierCanMove(_chess, _barrier, QPoint(i, j), barrierN))
                    {
                        move.redMove = false;
                        move.chessMove = false;
                        move.barrierChange = true;
                        move.point = QPoint(i, j);
                        moves.append(move);
                    }
                    _barrier[barrierN]._across = true;
                }
            }
        }
    }
    return moves;
}
bool State::chessCanMove(ChessPiece _chess[2], Barrier _barrier[16], QPoint point, bool _redTurn)
{
    QPoint point1;
    bool c[4] = {false, false, false, false}; //上下左右
    bool span = false;
    int b ,a, d, e;
    if(_redTurn)
    {
        point1 = QPoint(_chess[0]._row, _chess[0]._col);
        if(point.rx() == _chess[1]._row && point.ry() == _chess[1]._col)
            return  false;
        loop:
        a = _chess[0]._row - point.rx();
        b = 10 * (_chess[0]._col - point.ry());
        d = a + b;
        a = _chess[0]._row - _chess[1]._row;
        b = 10 * (_chess[0]._col - _chess[1]._col);
        e = a + b;
        for (int i = 0; i < 16; i++)
        {
            if(_barrier[i]._row == (_chess[0]._row - 1) && _barrier[i]._col == (_chess[0]._col - 1)){
                if(_barrier[i]._across)  c[0] = true;
                else  c[2] = true;
            }
            else if (_barrier[i]._row == (_chess[0]._row - 1) && _barrier[i]._col == _chess[0]._col){
                if(_barrier[i]._across)  c[0] = true;
                else  c[3] = true;
            }
            else if (_barrier[i]._row == _chess[0]._row && _barrier[i]._col == (_chess[0]._col - 1)){
                if(_barrier[i]._across)  c[1] = true;
                else  c[2] = true;
            }
            else if (_barrier[i]._row == _chess[0]._row && _barrier[i]._col == _chess[0]._col) {
                if(_barrier[i]._across)  c[1] = true;
                else  c[3] = true;
            }
        }
        if(e == 1 && (d == 2 || d == 11 || d == -9) && !c[0])
        {
            _chess[0]._row = _chess[1]._row;
            _chess[0]._col = _chess[1]._col;
            span = true;
            goto loop;
        }
        else if(e == -1 && (d == -2 || d == -11 || d == 9) && !c[1])
        {
            _chess[0]._row = _chess[1]._row;
            _chess[0]._col = _chess[1]._col;
            span = true;
            goto loop;
        }
        else if(e == 10 && (d == 9 || d == 11 || d == 20) && !c[2])
        {
            _chess[0]._row = _chess[1]._row;
            _chess[0]._col = _chess[1]._col;
            span = true;
            goto loop;
        }
        else if(e == -10 && (d == -9 || d == -11 || d == -20) && !c[3])
        {
            _chess[0]._row = _chess[1]._row;
            _chess[0]._col = _chess[1]._col;
            span = true;
            goto loop;
        }
        if((d == 1 && !c[0]) || (d == -1 && !c[1]) || (d == 10 && !c[2]) || (d == -10 && !c[3]))
            return true;
        else if (span)
        {
            _chess[0]._row = point1.rx();
            _chess[0]._col = point1.ry();
            return false;
        }
        else
            return false;
    }
    else
    {
        point1 = QPoint(_chess[1]._row, _chess[1]._col);
        if(point.rx() == _chess[0]._row && point.ry() == _chess[0]._col)
            return  false;
        loop1:
        a = _chess[1]._row - point.rx();
        b = 10 * (_chess[1]._col - point.ry());
        d = a + b;
        a = _chess[1]._row - _chess[0]._row;
        b = 10 * (_chess[1]._col - _chess[0]._col);
        e = a + b;
        for (int i = 0; i < 16; i++)
        {
            if(_barrier[i]._row == (_chess[1]._row - 1) && _barrier[i]._col == (_chess[1]._col - 1)){
                if(_barrier[i]._across)  c[0] = true;
                else  c[2] = true;
            }
            else if (_barrier[i]._row == (_chess[1]._row - 1) && _barrier[i]._col == _chess[1]._col){
                if(_barrier[i]._across)  c[0] = true;
                else  c[3] = true;
            }
            else if (_barrier[i]._row == _chess[1]._row && _barrier[i]._col == (_chess[1]._col - 1)){
                if(_barrier[i]._across)  c[1] = true;
                else  c[2] = true;
            }
            else if (_barrier[i]._row == _chess[1]._row && _barrier[i]._col == _chess[1]._col){
                if(_barrier[i]._across)  c[1] = true;
                else  c[3] = true;
            }
        }
        if(e == 1 && (d == 2 || d == 11 || d == -9) && !c[0])
        {
            _chess[1]._row = _chess[0]._row;
            _chess[1]._col = _chess[0]._col;
             span = true;
            goto loop1;
        }
        else if(e == -1 && (d == -2 || d == -11 || d == 9) && !c[1])
        {
            _chess[1]._row = _chess[0]._row;
            _chess[1]._col = _chess[0]._col;
             span = true;
            goto loop1;
        }
        else if(e == 10 && (d == 9 || d == 11 || d == 20) && !c[2])
        {
            _chess[1]._row = _chess[0]._row;
            _chess[1]._col = _chess[0]._col;
             span = true;
            goto loop1;
        }
        else if(e == -10 && (d == -9 || d == -11 || d == -20) && !c[3])
        {
            _chess[1]._row = _chess[0]._row;
            _chess[1]._col = _chess[0]._col;
             span = true;
            goto loop1;
        }
        if((d == 1 && !c[0]) || (d == -1 && !c[1]) || (d == 10 && !c[2]) || (d == -10 && !c[3]))
            return true;
        else if (span)
        {
            _chess[1]._row = point1.rx();
            _chess[1]._col = point1.ry();
            return false;
        }
        else
            return false;
    }
}
bool State::barrierCanMove(ChessPiece _chess[2], Barrier _barrier[16], QPoint point, int lastNumber)
{
    int a, b;
    for(int i = 0; i < 16; i++)
    {
        if(i == lastNumber)
            continue;
        a = abs(_barrier[i]._row - point.rx());
        b = 10 * abs(_barrier[i]._col - point.ry());
        if(a + b == 0)
            return false;
        else if(a + b == 10 && _barrier[i]._across && _barrier[lastNumber]._across)
            return false;
        else if(a + b == 1 && !_barrier[i]._across && !_barrier[lastNumber]._across)
            return false;
    }
    return true;
}

bool State::isGameOver()
{
    if(chess[0]._row == 1 || chess[1]._row == 7)
        return true;
    else
        return false;
}
