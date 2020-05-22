#ifndef MCTS_H
#define MCTS_H
#include "widget.h"
#include<QVector>
#include "Treenode.h"
#include "type.h"

class MCTS : public Widget
{
    Q_OBJECT
public:
    friend class State;
    MCTS();
    virtual void mouseLeftClick(QPoint clickLocation);

    TreeNode tree;
    TreeNode initChildren(TreeNode* father, mover move);
    int cycleN;
    TreeNode* maxUcbNode(TreeNode* _currentNode);
    void updateUct(TreeNode* _treeNode, int _size);
   // QVector<TreeNode> expand(TreeNode* _currentNode);
    void append(TreeNode* treeStartNode, int _size, TreeNode* _currentNode);
    int roolOut(TreeNode* _currentNode);
    void backProgram(TreeNode* _currentNode);
public slots:
    void computerMove();
private:
    int value;
};

#endif // PRUNING_H
