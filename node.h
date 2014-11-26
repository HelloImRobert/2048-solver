#ifndef NODE_H
#define NODE_H

#include<stdlib.h>
#include<vector>

class Node
{
public:
    Node();
    Node(Node &inputnode);
    void copy_node(Node &inputnode);

    std::vector<std::vector<int> > fieldarr;

    double score_1;
    double score_2;

};

#endif // NODE_H
