#include "widget.h"
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include "chesspiece.h"
#include "barrier.h"
#include <QDebug>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //初始化棋盘
    this->setGeometry(100,100,780,600);
    redBarrier = new QLabel(this);
    blackBarrier = new QLabel(this);
    blackBarrier->setText("剩余障碍数：");
    blackBarrier->setGeometry(600,80,80,20);
    redBarrier->setText("剩余障碍数：");
    redBarrier->setGeometry(600,500,80,20);
    //初始化棋子
    chessRed = new ChessPiece(true, 7, 4);
    chessBlack = new ChessPiece(false, 1, 4);
    for(int i = 0; i < 16; i++)
    {
        if(i < 8)
        {
            barrier[i]._redFaction = true;
            barrier[i]._row = 0;
            barrier[i]._col = 0;
         }
        else
        {
            barrier[i]._redFaction = false;
            barrier[i]._row = 0;
            barrier[i]._col = 0;
        }
    }

}
//画棋盘
void Widget::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
 //   QFont font = painter.font();
    //画棋盘
    for(int i = 1; i < 9; i++)
    {
        painter.drawLine(QPoint(d,d*i),QPoint(8*d,d*i));
        painter.drawLine(QPoint(d*i,d),QPoint(i*d,d*8));
    }
    if(redTurn)
    {
        painter.setBrush(QBrush(QColor(Qt::red)));
        painter.drawEllipse(QPoint(600,300),20,20);
    }
    else
    {
        painter.setBrush(QBrush(QColor(Qt::black)));
        painter.drawEllipse(QPoint(600,300),20,20);
    }
    painter.drawText(680,95,QString::number(blackBarrierN));
    painter.drawText(680,515,QString::number(redBarrierN));
    if(result == 1)
        painter.drawText(QPoint(650, 300),"RDE WIN");
    else if (result == -1)
        painter.drawText(QPoint(650, 300),"BALCK WIN");

    //画障碍
    for(int i = 0; i < 16; i++)
    {
        if(barrier[i]._col == 0 && barrier[i]._row == 0 && i < 8)
        {
            if(selectId == 1)
                painter.setBrush(QBrush(QColor(Qt::gray)));
            else
                painter.setBrush(QBrush(QColor(Qt::yellow)));
            if(barrier[i]._across)
                painter.drawRect(600,525,barrier->length,barrier->width);
            else
                painter.drawRect(600,525,barrier->width,barrier->length);
        }
        else if(barrier[i]._col == 0 && barrier[i]._row == 0 && i < 16)
        {
            if(selectId == 3)
                painter.setBrush(QBrush(QColor(Qt::gray)));
            else
                painter.setBrush(QBrush(QColor(Qt::yellow)));
            if(barrier[i]._across)
                painter.drawRect(600,105,barrier->length,barrier->width);
            else
                painter.drawRect(600,105,barrier->width,barrier->length);

        }
        else
        {
            painter.setBrush(QBrush(QColor(Qt::yellow)));
            drawBarrier(painter, i);
        }
    }

    //画棋子
    if(selectId == 0)
        painter.setBrush(QBrush(QColor(Qt::gray)));
    else
        painter.setBrush(QBrush(QColor(Qt::red)));
    drawChessPiece(painter, chessRed);
    if(selectId == 2)
        painter.setBrush(QBrush(QColor(Qt::gray)));
    else
        painter.setBrush(QBrush(QColor(Qt::black)));
    drawChessPiece(painter, chessBlack);
}
//走棋子
void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint clickLocation = ev->pos();
    //判断点击此处有没有棋子或障碍
    if(ev->button() & Qt::LeftButton)
        mouseLeftClick(clickLocation);
    // 鼠标右键，切换障碍状态
    else if(ev->button() & Qt::RightButton)
        mouseLeftClick();
}
//鼠标点击左键
void Widget::mouseLeftClick(QPoint clickLocation)
{
    //选择棋子
    if(selectId == -1 && result == 0)
    {
        if(haveChess(clickLocation))
        {
            QPoint location;//棋盘位置
            location = coordinateChange1(clickLocation.rx(), clickLocation.ry());
            if(location.rx() == chessRed->_row && location.ry() == chessRed->_col && redTurn)
                selectId = 0;
            else if (location.rx() == chessBlack->_row && location.ry() == chessBlack->_col && !redTurn)
            {
                selectId = 2;
            }

        }
        else if (haveBarrier(clickLocation))
        {
            if(clickLocation.ry() < 200 && !redTurn)
                selectId = 3;
            else if (clickLocation.ry() > 200 && redTurn)
                selectId = 1;
        }
        else
            return;
        update();
    }
    //走棋
    else if(canMove(clickLocation.rx(), clickLocation.ry()) && result == 0)
    {
        if(haveChess(clickLocation) && (selectId == 0 ||selectId == 2))
        {
            QPoint location;//棋盘位置
            location = coordinateChange1(clickLocation.rx(), clickLocation.ry());
            if(selectId == 0)
            {
                chessRed->_row = location.rx();
                chessRed->_col = location.ry();
            }
            else if(selectId == 2)
            {
                chessBlack->_row = location.rx();
                chessBlack->_col = location.ry();
            }
            update();
        }
        else if(validBarrierL(clickLocation))
        {
            QPoint location;//棋盘位置
            location = coordinateChange3(clickLocation.rx(), clickLocation.ry());
            if(selectId == 1)
            {
                barrier[redBarrierN -1]._row = location.rx();
                barrier[redBarrierN -1]._col = location.ry();
                redBarrierN--;
            }
            else if (selectId == 3)
            {
                barrier[blackBarrierN + 7]._row = location.rx();
                barrier[blackBarrierN + 7]._col = location.ry();
                blackBarrierN--;
            }
            update();
        }
        else
            return;
        selectId = -1;
        if(chessRed->_row == 1)
            result = 1;
        else if(chessBlack->_row == 7)
            result = -1;
        redTurn = !redTurn;
    }
}
//鼠标点击右键
void Widget::mouseLeftClick()
{
    if(selectId == 1)
    {
        if(barrier[redBarrierN - 1]._across)
            barrier[redBarrierN - 1]._across = false;
        else
            barrier[redBarrierN - 1]._across = true;
        update();
    }
    else if(selectId == 3)
    {
        if(barrier[blackBarrierN + 7]._across)
            barrier[blackBarrierN + 7]._across = false;
        else
            barrier[blackBarrierN + 7]._across = true;
        update();
    }
}
//画棋子
void Widget::drawChessPiece(QPainter &painter, ChessPiece* chess)
{
    QPoint point;
    point = coordinateChange(chess->_row, chess->_col);
    painter.drawEllipse(point, chess->_radius, chess->_radius);
}
//画障碍
void Widget::drawBarrier(QPainter &painter, int id)
{
    QPoint point;
    point = coordinateChange2(barrier[id]._row, barrier[id]._col);
    if(barrier[id]._across)
        painter.drawRect(point.rx()-barrier[id].length/2, point.ry()-barrier[id].width/2, barrier[id].length, barrier[id].width);
    else
        painter.drawRect(point.rx()-barrier[id].width/2, point.ry()-barrier[id].length/2, barrier[id].width, barrier[id].length);

}
// 棋子坐标转化,返回像素坐标
QPoint Widget::coordinateChange(int row, int col)
{
    QPoint a;
    a.setX(col * d + d/2);
    a.setY(row * d + d/2);
    return a;
}
// 棋子坐标转化，返回棋子坐标
QPoint Widget::coordinateChange1(int row, int col)
{
    QPoint a;
    int b, c;
    a.setX(col/d);
    a.setY(row/d);
    b = (a.ry() * d + d/2 - row) * (a.ry() * d + d/2 - row);
    c = (a.ry() * d + d/2 - col) * (a.rx() * d + d/2 - col);
    if(b + c > chessRed->_radius * chessRed->_radius)
        return QPoint(10,10);
    return a;
}
// 障碍坐标转化，返回像素坐标
QPoint Widget::coordinateChange2(int row, int col)
{
    QPoint a;
    a.setX(col * d + d);
    a.setY(row * d + d);
    return a;
}
// 障碍坐标转化， 返回障碍坐标
QPoint Widget::coordinateChange3(int row, int col)
{
    QPoint a;
    a.setX((col - d/2)/d);
    a.setY((row - d/2)/d);
    return a;
}

//判断是否有棋子
bool Widget::haveChess(QPoint point)
{
    if(point.rx()/d > 0 && point.rx()/d < 8 && point.ry()/d > 0 && point.ry()/d < 8)
    {
        int a = (point.rx()%d - d/2) * (point.rx()%d - d/2);
        int b = (point.ry()%d - d/2) * (point.ry()%d - d/2);
        int c = chessRed->_radius * chessRed->_radius;
        if(a + b < c)
             return true;
    }
    return false;
}
//判断是否有障碍
bool Widget::haveBarrier(QPoint point)
{
    if(point.rx() > 600 && point.rx() < 720 && point.ry() > 105 && point.ry() < 115 && blackBarrierN > 0)
        return true;
    else if(point.rx() > 600 && point.rx() < 720 && point.ry() > 525 && point.ry() < 535 && redBarrierN > 0)
        return true;
    else
        return false;
}
//判断是否是有效障碍位置
bool Widget::validBarrierL(QPoint point)
{
    if((point.rx()+d/2)/d > 1 && (point.rx()+d/2)/d < 8 && (point.ry()+d/2)/d > 1 &&  (point.ry()+d/2)/d < 8)
    {
        int a = ((point.rx() + d/2)%d - d/2) * ((point.rx() + d/2)%d - d/2);
        int b = ((point.ry() + d/2)%d - d/2) * ((point.ry() + d/2)%d - d/2);
        int c = d*d/4;
        if(a + b < c)
            return true;
     }
     return false;
}
//判断棋子和障碍是否可以移动
bool Widget::canMove(int row, int col)
{
    if(selectId == 0 || selectId == 2)
    {
        if(chessCanMove(row, col))
            return true;
    }
    else if(selectId == 1 || selectId == 3)
    {
        if(barrierCanMove(row, col))
            return  true;
    }
    return false;
}
//判断棋子移动
bool Widget::chessCanMove(int row, int col)
{
    QPoint point = coordinateChange1(row, col);// 键盘坐标
    QPoint point1;
    bool c[4] = {false, false, false, false}; //上下左右
    bool span = false;
    int b ,a, d, e;
    if(selectId == 0)  // 红
    {
        point1 = QPoint(chessRed->_row, chessRed->_col);
        if(point.rx() == chessBlack->_row && point.ry() == chessBlack->_col)
            return  false;
        loop:
        a = chessRed->_row - point.rx();
        b = 10 * (chessRed->_col - point.ry());
        d = a + b;
        a = chessRed->_row - chessBlack->_row;
        b = 10 * (chessRed->_col - chessBlack->_col);
        e = a + b;
        for (int i = 0; i < 16; i++)
        {
            if(barrier[i]._row == (chessRed->_row - 1) && barrier[i]._col == (chessRed->_col - 1)){
                if(barrier[i]._across)  c[0] = true;
                else  c[2] = true;
            }
            else if (barrier[i]._row == (chessRed->_row - 1) && barrier[i]._col == chessRed->_col){
                if(barrier[i]._across)  c[0] = true;
                else  c[3] = true;
            }
            else if (barrier[i]._row == chessRed->_row && barrier[i]._col == (chessRed->_col - 1)){
                if(barrier[i]._across)  c[1] = true;
                else  c[2] = true;
            }
            else if (barrier[i]._row == chessRed->_row && barrier[i]._col == chessRed->_col) {
                if(barrier[i]._across)  c[1] = true;
                else  c[3] = true;
            }
        }
        if(e == 1 && (d == 2 || d == 11 || d == -9) && !c[0])
        {
            chessRed->_row = chessBlack->_row;
            chessRed->_col = chessBlack->_col;
            span = true;
            goto loop;
        }
        else if(e == -1 && (d == -2 || d == -11 || d == 9) && !c[1])
        {
            chessRed->_row = chessBlack->_row;
            chessRed->_col = chessBlack->_col;
            span = true;
            goto loop;
        }
        else if(e == 10 && (d == 9 || d == 11 || d == 20) && !c[2])
        {
            chessRed->_row = chessBlack->_row;
            chessRed->_col = chessBlack->_col;
            span = true;
            goto loop;
        }
        else if(e == -10 && (d == -9 || d == -11 || d == -20) && !c[3])
        {
            chessRed->_row = chessBlack->_row;
            chessRed->_col = chessBlack->_col;
            span = true;
            goto loop;
        }
        if((d == 1 && !c[0]) || (d == -1 && !c[1]) || (d == 10 && !c[2]) || (d == -10 && !c[3]))
            return true;
        else if (span)
        {
            chessRed->_row = point1.rx();
            chessRed->_col = point1.ry();
            return false;
        }
        else
            return false;
    }
    else  // 黑
    {
        point1 = QPoint(chessBlack->_row, chessBlack->_col);
        if(point.rx() == chessRed->_row && point.ry() == chessRed->_col)
            return  false;
        loop1:
        a = chessBlack->_row - point.rx();
        b = 10 * (chessBlack->_col - point.ry());
        d = a + b;
        a = chessBlack->_row - chessRed->_row;
        b = 10 * (chessBlack->_col - chessRed->_col);
        e = a + b;
        for (int i = 0; i < 16; i++)
        {
            if(barrier[i]._row == (chessBlack->_row - 1) && barrier[i]._col == (chessBlack->_col - 1)){
                if(barrier[i]._across)  c[0] = true;
                else  c[2] = true;
            }
            else if (barrier[i]._row == (chessBlack->_row - 1) && barrier[i]._col == chessBlack->_col){
                if(barrier[i]._across)  c[0] = true;
                else  c[3] = true;
            }
            else if (barrier[i]._row == chessBlack->_row && barrier[i]._col == (chessBlack->_col - 1)){
                if(barrier[i]._across)  c[1] = true;
                else  c[2] = true;
            }
            else if (barrier[i]._row == chessBlack->_row && barrier[i]._col == chessBlack->_col){
                if(barrier[i]._across)  c[1] = true;
                else  c[3] = true;
            }
        }
        if(e == 1 && (d == 2 || d == 11 || d == -9) && !c[0])
        {
            chessBlack->_row = chessRed->_row;
            chessBlack->_col = chessRed->_col;
            goto loop1;
        }
        else if(e == -1 && (d == -2 || d == -11 || d == 9) && !c[1])
        {
            chessBlack->_row = chessRed->_row;
            chessBlack->_col = chessRed->_col;
            goto loop1;
        }
        else if(e == 10 && (d == 9 || d == 11 || d == 20) && !c[2])
        {
            chessBlack->_row = chessRed->_row;
            chessBlack->_col = chessRed->_col;
            goto loop1;
        }
        else if(e == -10 && (d == -9 || d == -11 || d == -20) && !c[3])
        {
            chessBlack->_row = chessRed->_row;
            chessBlack->_col = chessRed->_col;
            goto loop1;
        }
        if((d == 1 && !c[0]) || (d == -1 && !c[1]) || (d == 10 && !c[2]) || (d == -10 && !c[3]))
            return true;
        else if (span)
        {
            chessBlack->_row = point1.rx();
            chessBlack->_col = point1.ry();
            return false;
        }
        else
            return false;
    }
}
//判断障碍移动
bool Widget::barrierCanMove(int row, int col)
{
    QPoint point = coordinateChange3(row, col);
    int a, b;
    for(int i =0; i < 16; i++)
    {
        if(selectId == 1 && i == (redBarrierN - 1)) //与自己相同
            continue;
        else if (selectId == 3 && i == (blackBarrierN + 7))
            continue;
        a = abs(barrier[i]._row - point.rx());
        b = 10 * abs(barrier[i]._col - point.ry());
        if(a + b == 0)
            return false;
        else if (selectId == 1 && a + b == 10 && barrier[i]._across && barrier[redBarrierN - 1]._across)
            return false;
        else if (selectId == 3 && a + b == 10 && barrier[i]._across && barrier[blackBarrierN + 7]._across)
            return false;
        else if (selectId == 1 && a + b == 1 && !barrier[i]._across && !barrier[redBarrierN - 1]._across)
            return false;
        else if (selectId == 3 && a + b == 1 && !barrier[i]._across && !barrier[blackBarrierN + 7]._across)
            return false;
    }
    return true;
}

Widget::~Widget()
{

}
