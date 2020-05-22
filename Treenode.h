#ifndef TREENODE_H
#define TREENODE_H
#include "State.h"
#include <QVector>


class TreeNode :public State
{
public:
    TreeNode(){}
    TreeNode(State state);
    int N = 0;            //访问次数
    int Q = 0;                      //模拟收益
    float c = 0.1;
    float UCB = 100000;
    bool isLeafNode = true;
    TreeNode* fatherNode;               //父节点
    QVector<TreeNode*> children;    //子节点
    void Update();
    void expand();

private:


};



#endif // TREENODE_H
