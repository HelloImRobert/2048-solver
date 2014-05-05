#include "node.h"

Node::Node() //init empty Node
{
    this->fieldarr.resize(4);
    this->fieldarr[0].resize(4);
    this->fieldarr[1].resize(4);
    this->fieldarr[2].resize(4);
    this->fieldarr[3].resize(4);

    this->score_1 = 0;
    this->score_2 = 0;

}

Node::Node(Node &inputnode) //init Node as copy of given node
{
    this->fieldarr.resize(4);
    this->fieldarr[0].resize(4);
    this->fieldarr[1].resize(4);
    this->fieldarr[2].resize(4);
    this->fieldarr[3].resize(4);

    this->copy_node(inputnode);
}

void Node::copy_node(Node &inputnode)
{
    for (int i = 0 ; i < 4; i++) //copy fieldarr
    {
        for (int j = 0 ; j < 4; j++)
        {
            this->fieldarr[i][j] = inputnode.fieldarr[i][j];
        }
    }

    this->score_1 = inputnode.score_1;
    this->score_2 = inputnode.score_2;

}
