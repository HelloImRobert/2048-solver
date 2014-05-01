#ifndef SCORENODE_H
#define SCORENODE_H

#include <aidata.h>

class ScoreNode
{
public:
    ScoreNode();

    void get_scores(Node &inputnode);

private:

    void calc_score_1(Node &inputnode);
    void calc_score_2(Node &inputnode);

    void calc_scr_free_slots(Node &inputnode);
    void calc_scr_chain_slots(Node &inputnode);

    void calc_free_slots(Node &inputnode);
    void calc_king_slot(Node &inputnode);

    int free_slots;
    int king_slot;
    int king_slot_pos_i;
    int king_slot_pos_j;

    double scr_free_slots;
    double scr_chain_slots;

};

#endif // SCORENODE_H
