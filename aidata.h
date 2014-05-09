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
    bool isDone;
}Scores_struct;


typedef struct
{
    double value;
    int pos_i;
    int pos_j;
    int dropval;
    bool is_used;
} Worst_Scores_struct;




// Scorefield container class
class Scorefield
{
public:
    Scorefield();

    void calc_final_scores(Node &inputnode, int how_many);

    // i, j, dropvalue, direction, <scores>
    std::vector< std::vector< std::vector< std::vector< Scores_struct > > > > fieldarr;

    Worst_Scores_struct worst_scores [4][2]; //[worst, 2nd, 3rd worst...] [type]

    double deathchance;

private:

    bool is_redundant (int i, int j, int n, int scoretype, int score_it);

    bool are_all_neg(int i, int j, int n);

    double get_best_score(int i, int j , int n, int type);

    void calc_worst_scores(Node &inputnode , int how_many);

    double get_deathchance( Node &inputnode );

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

    // (recursive) score checker. inputnode = current state of the scorefield, depth = how deep should he look, branches = how many of the worst possible outcomes should he check (0 = all)
    Scores_struct check_score_deep(Node inputnode, int depth);

    int calc_score_1(const Node &inputnode);

    double calc_score_2(const Node &inputnode);

    bool check_move(Node &inputnode, int direction);

    bool check_gameover(const Node &inputnode);

    int check_empty (const Node &inputnode); //# of empty slots in inputnode


    void copy_node(Node &inputnode, Node &outputnode);

    bool move_left(Node &inputnode);

    bool move_right(Node &inputnode);

    bool move_up(Node &inputnode);

    bool move_down(Node &inputnode);

    void finalize_Scorefield (Scorefield &myScorefield);

};

#endif // AIDATA_H
