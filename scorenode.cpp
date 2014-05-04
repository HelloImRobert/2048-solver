#include "scorenode.h"

ScoreNode::ScoreNode() //init empty Node
{

}



void ScoreNode::get_scores(Node &inputnode)
{
    for (int i = 0 ; i < 4; i++) //copy fieldarr
    {
        for (int j = 0 ; j < 4; j++)
        {
            this->fieldarr[i][j] = inputnode.fieldarr[i][j];
        }
    }


    //    this->child_id = inputnode.child_id;
    //    this->deathchance = inputnode.deathchance;
    //    this->lvl = inputnode.lvl;
    //    this->parent_id = inputnode.parent_id;
    //    this->resolve_direction = inputnode.resolve_direction;



    //init all helper values
    ScoreNode::calc_free_slots();
    ScoreNode::calc_king_slot();



    // get scores
    ScoreNode::calc_score_1(inputnode);
    ScoreNode::calc_score_2(inputnode);

}

// MAIN SCORE CALC
void ScoreNode::calc_scr_free_slots()
{
    double exponent;
    int needed_free;


    needed_free = 16 - king_slot; //# of slots to keep free (dependent on the highest number). any number below that will get an increasingly strong penalty on the score

    if (needed_free < 1)
    {
        this->scr_free_slots = 0.001; // prevent a score of zero
    }
    else
    {
        exponent = (-1) * (free_slots / needed_free); // calculate score
        this->scr_free_slots = pow((1 - pow(20, exponent)), 2);
    }

}


//MAIN SCORE CALC
void ScoreNode::calc_scr_chain_slots()
{
    double king_slot_scr = 1.0;
    double scr_chain = 1.0;
    bool endOfChain = false;

    int i = king_slot_pos_i;
    int j = king_slot_pos_j;

    // postion of king slot
    //check row
    if (king_slot_pos_i == 3)
        king_slot_scr = 1.0;
    else
        king_slot_scr = 0.01/(3.0 - king_slot_pos_i); //never ever leave the bottom row (if you can't be sure to get back)

    //check colum
    // cloum score = 1/(1+ dist-to-ideal-pos)
    king_slot_scr = king_slot_scr * (1.0 /(king_slot_pos_j + 1.0));


    scr_chain_slots = king_slot_scr * scr_chain;
}

// HELPER
// calc amount of free(= 0) slots
void ScoreNode::calc_free_slots()
{
    free_slots = 0;

    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++)
        {
            if(fieldarr[i][j] == 0)
                free_slots++;
        }
    }
}


// HELPER
// calc value and position of slot with highest value in the game
void ScoreNode::calc_king_slot()
{
    king_slot = 0;
    king_slot_pos_i = 0;
    king_slot_pos_j = 0;

    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 3 ; j >= 0 ; j--)
        {
            if(fieldarr[i][j] >= king_slot) //the lowest left most slot with the highest value is king
            {
                king_slot = fieldarr[i][j];
                king_slot_pos_i = i;
                king_slot_pos_j = j;
            }
        }
    }
}


void ScoreNode::calc_score_1(Node &inputnode) // second version score heuristic of scoretype 1: # of free slots in the gamefield - the more the better
{
    int returnval = 0;

    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++)
        {
            if(inputnode.fieldarr[i][j] == 0)
                returnval++;
        }
    }

    inputnode.score_1 = returnval;
}

void ScoreNode::calc_score_2(Node &inputnode)//score heuristic of scoretype 2: # monotony of the gamefield * small steps between beighbors
{
    double score1;
    double score2;

    score1 = scr_free_slots;

    score2 = scr_chain_slots;

    inputnode.score_2 = score1 * score2;
}
