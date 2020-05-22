#include "MCTS.h"
#include "widget.h"
#include "type.h"
#include "barrier.h"
#include "Treenode.h"
#include "State.h"
#include <QDebug>
#include <QTimer>

MCTS::MCTS()
{

}
void MCTS::mouseLeftClick(QPoint clickLocation)
{
   if(this->redTurn)
   {
       Widget::mouseLeftClick(clickLocation);
       if(chessRed->_row == 1)
           result = 1;
       else if(chessBlack->_row == 7)
           result = -1;

       if(!this->redTurn && result == 0)
       {
           QTimer::singleShot(100, this, SLOT(computerMove()));
        //   computerMove();
       }
         //  QTimer::singleShot(100, this, computerMove());
          // computerMove();
   }
}
void MCTS::computerMove()
{
    //创建根节点
    ChessPiece chess1[2];
    chess1[0] = *chessRed;
    chess1[1] = *chessBlack;
    State state(chess1, barrier, redTurn);
    TreeNode rootNode(state);
    rootNode.fatherNode = NULL;
    rootNode.isLeafNode = false;
    QVector<TreeNode> treeNode;     // 存储树
    treeNode.reserve(40000);
    treeNode.push_back(rootNode);
    treeNode.resize(treeNode.size() + rootNode.moveNumber);
    append(&treeNode[0], treeNode.size(), &treeNode[0]);
    TreeNode* currentNode;            // 定义当前指针
    cycleN = 2000;
    while(cycleN)
    {
        //指针指向根节点
        currentNode = &treeNode[0];
        //更新uct
        updateUct(&treeNode[0], treeNode.size());
        //选择uct最大的节点
        currentNode = maxUcbNode(currentNode);
        //判断当前节点是否为叶节点
        while(!currentNode->isLeafNode)
        {
            //currentNode指向最大uct的叶节点
            currentNode = maxUcbNode(currentNode);
        }
        //判断当前节点被访问次数是否为零
        if(currentNode->N != 0)
        {
            if(currentNode->moveNumber > 0)
            {
                 //当前节点不是叶节点
//                if(currentNode == &treeNode[4])
//                    cycleN = cycleN;
                currentNode->isLeafNode = false;
                //把当前节点下的子节点添加到树
                treeNode.resize(treeNode.size() + currentNode->moveNumber);
                append(&treeNode[0], treeNode.size(), currentNode);
                //当前节点指向其中一个
                currentNode = currentNode->children[0];
            }
        }
        //模拟
         value = roolOut(currentNode);
        //返回
        backProgram(currentNode);
        cycleN--;
    }
    currentNode = &treeNode[0];
    updateUct(&treeNode[0], treeNode.size());
    currentNode = maxUcbNode(currentNode);
    *chessRed = currentNode->chess[0];
    *chessBlack = currentNode->chess[1];
    for(int i = 0; i < 16; i++)
    {
        barrier[i] = currentNode->barrier[i];
    }
    if(!currentNode->lastMove.chessMove)
        blackBarrierN--;
    if(chessRed->_row == 1)
        result = 1;
    else if(chessBlack->_row == 7)
        result = -1;
    update();
    redTurn = !redTurn;
}
int MCTS::roolOut(TreeNode *_currentNode)
{
    int data;
    int loopN = 500;
    TreeNode currentNode = *_currentNode;
    //从当前状态随机选一个下一步
    while(loopN)
    {
        if(currentNode.moveNumber == 0)
            break;
        data = rand()%currentNode.moveNumber;      
        currentNode = initChildren(&currentNode, currentNode.nextMoves[data]);
        loopN--;
        if(currentNode.chess[1]._row == 7)
            return 10;
        else if (currentNode.chess[0]._row == 1)
            return -10;
    }
    return 0;
}
void MCTS::backProgram(TreeNode *_currentNode)
{
    while(1)
    {
        _currentNode->N++;
        _currentNode->Q += value;
        if(!_currentNode->fatherNode)
            break;
        _currentNode = _currentNode->fatherNode;
    }
}

void MCTS::append(TreeNode* treeStartNode, int _size, TreeNode* _currentNode)
{
    int data;
    for (int i = 0; i < _currentNode->moveNumber; i++)
    {
        data = _size - _currentNode->moveNumber + i;
        treeStartNode[data] = initChildren(_currentNode,_currentNode->nextMoves[i]);
     //   treeStartNode[data].fatherNode = _currentNode;
        _currentNode->children[i] = &treeStartNode[data];
    }
}
void MCTS::updateUct(TreeNode* _treeNode, int _size)
{
    for(int i = 1; i < _size; i++)
    {
        if(_treeNode[i].N == 0)
            _treeNode[i].UCB = 15;
        else
        {
             _treeNode[i].UCB = _treeNode[i].Q/_treeNode[i].N +
              _treeNode[i].c * sqrtf(logf(_treeNode[i].fatherNode->N)/_treeNode[i].N);
        }

    }
}

TreeNode* MCTS::maxUcbNode(TreeNode* _currentNode)
{
    float maxUcb = -10000;
    TreeNode* node = _currentNode->children[0];
    for (int i = 0; i < _currentNode->children.size(); i++)
    {
        if(_currentNode->children[i]->UCB > maxUcb)
        {
            node =_currentNode->children[i];
            maxUcb = _currentNode->children[i]->UCB;
        }
    }
    return node;
}

TreeNode MCTS::initChildren(TreeNode* father, mover move)
{

    State state(father->chess, father->barrier,!father->redTurn);
    TreeNode children(state);
    children.fatherNode = father;
    children.lastMove = move;
    int barrierN = -1, i;
    //判断哪方棋子走动
    //判断走动的是棋子还是障碍
    //是棋子直接改变位置
    //判断哪个障碍
    //是障碍改变障碍位置并判断是否改变状态
    if(move.redMove)
    {
        if(move.chessMove)
        {
            children.chess[0]._row = move.point.rx();
            children.chess[0]._col = move.point.ry();
        }
        else
        {
            for (i = 7; i > -1; i--)
            {
                if(father->barrier[i]._col == 0)
                {
                    barrierN = i;
                    break;
                }

            }
            children.barrier[barrierN]._row = move.point.rx();
            children.barrier[barrierN]._col = move.point.ry();
            if(move.barrierChange)
                children.barrier[barrierN]._across = false;

        }
    }
    else
    {
        if(move.chessMove)
        {
            children.chess[1]._row = move.point.rx();
            children.chess[1]._col = move.point.ry();
        }
        else
        {
            for(i = 15; i > 7; i--)
            {
                if(father->barrier[i]._col == 0)
                {
                    barrierN = i;
                    break;
                }
            }
            children.barrier[barrierN]._row = move.point.rx();
            children.barrier[barrierN]._col = move.point.ry();
            if(move.barrierChange)
                children.barrier[barrierN]._across = false;
        }
    }
    return children;
}


