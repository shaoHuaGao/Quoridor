#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include "chesspiece.h"
#include "barrier.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = NULL);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent* ev);
    ~Widget();
    void drawChessPiece(QPainter& painter ,ChessPiece* chess);
    void drawBarrier(QPainter& painter, int id);
    QPoint coordinateChange(int row, int col);
    QPoint coordinateChange1(int row, int col);
    QPoint coordinateChange2(int row, int col);
    QPoint coordinateChange3(int row, int col);
    bool haveChess(QPoint point);
    bool haveBarrier(QPoint point);
    bool validBarrierL(QPoint point);
    bool canMove(int row, int col);
    bool chessCanMove(int row, int col);
    bool barrierCanMove(int row, int col);
    virtual void mouseLeftClick(QPoint clickLocation);
    void mouseLeftClick();
    bool redTurn = true;
    ChessPiece *chessRed, *chessBlack;
    Barrier barrier[16];
    int redBarrierN = 8;
    int blackBarrierN = 8;
    int result = 0;
private:
    QLabel *redBarrier, *blackBarrier;


    int d = 70;   // 棋盘边长
    int selectId = -1;



};

#endif // WIDGET_H
