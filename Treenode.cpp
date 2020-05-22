#include "Treenode.h"

TreeNode::TreeNode(State state)
{
    this->chess[0] = state.chess[0];
    this->chess[1] = state.chess[1];
    for(int i = 0; i < 16; i++)
        this->barrier[i] = state.barrier[i];
    this->redTurn = state.redTurn;
    this->nextMoves = state.nextMoves;
    this->moveNumber = state.moveNumber;
    this->children.resize(this->moveNumber);
}
