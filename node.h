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
    /*
    double deathchance;
    int lvl;
    int resolve_direction;
    int parent_id;
    int child_id;
    */
};

#endif // NODE_H
