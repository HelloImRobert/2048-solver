#ifndef AIDATA_H
#define AIDATA_H


#include <stdlib.h>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <QVector>

#include <scorenode.h>
#include <node.h>


#define STACKSIZE 10

typedef struct
{
    double score_1;
    double score_2;
    double deathchance;
}Scores_struct;


typedef struct
{
    int value;
    Scores_struct up_two;
    Scores_struct up_four;
    Scores_struct right_two;
    Scores_struct right_four;
    Scores_struct down_two;
    Scores_struct down_four;
    Scores_struct left_two;
    Scores_struct left_four;
}Scorefieldvalues_struct;

/*
// Node container class with init
class Node
{
public:
    Node();
    Node(Node &inputnode);
    void copy_node(Node &inputnode);

    std::vector<std::vector<int> > fieldarr;

    double score_1;
    double score_2;

//    double deathchance;
//    int lvl;
//    int resolve_direction;
//    int parent_id;
//    int child_id;

};

*/

// Scorefield container class
class Scorefield
{
public:
    Scorefield(Node &inputnode);
    std::vector<std::vector<Scorefieldvalues_struct> > fieldarr;
    int free_slots;
};


class AIdata
{
public:
    AIdata();

    int think(std::vector<std::vector<int> > &gamefield, int depth);


private:

    Node Solverstack[5000];

    ScoreNode ScoreCalc;//create scoring object

    int stack_read;

    int stack_write;




    Scores_struct check_score(Node inputnode, int depth);

    Scores_struct check_score_deep(Node inputnode, int depth);

    int calc_score_1(const Node &inputnode);

    double calc_score_2(const Node &inputnode);

    void copy_to_stack(const Node &, int);

    bool check_move(Node &inputnode, int direction);

    bool check_gameover(const Node &inputnode);

    int check_empty (const Node &inputnode); //# of empty slots in inputnode


    void copy_node(Node &inputnode, Node &outputnode);

    bool move_left(Node &inputnode);

    bool move_right(Node &inputnode);

    bool move_up(Node &inputnode);

    bool move_down(Node &inputnode);

};

#endif // AIDATA_H
