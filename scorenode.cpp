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


    //init all helper values
    ScoreNode::calc_free_slots();
    ScoreNode::calc_king_slot();

    //calc all partial scores
    ScoreNode::calc_scr_free_slots();
    ScoreNode::calc_scr_chain_slots();

    // get scores
    ScoreNode::calc_score_1(inputnode);
    ScoreNode::calc_score_2(inputnode);

}

// FREE SLOTS SCORE
void ScoreNode::calc_scr_free_slots()
{
    double exponent;
    int needed_free;


    needed_free = 16 - king_slot; //# of slots to keep free (dependent on the highest number). any number below that will get an increasingly strong penalty on the score

    if(needed_free < 4)
        needed_free = 4;

    if (free_slots < 1)
    {
        this->scr_free_slots = 0.001; // prevent a score of zero
    }
    else
    {
        exponent = (-1) * ((float)free_slots / (float)needed_free); // calculate score
        this->scr_free_slots = pow((1.0 - pow(20.0, exponent)), 2.0);
    }

}


//MONOTONY SCORE
void ScoreNode::calc_scr_chain_slots()
{
    //this score has two parts: 1st the position of the so called king slot (the field with the highest value)
    //2nd the monotony of decrease of the field values starting at the king slot and covering the field in a zig-zag motion to the very top

    //partial scores
    double king_slot_scr = 1.0;
    double scr_chain = 1.0;

    //additional variables
    int diff;




    // --- postion of king slot ---

    //check colum
    //colum score = 1/(1+ dist-to-ideal-pos)
    king_slot_scr = (1.0 /(king_slot_pos_j + 1.0));

    if (king_slot_pos_j >= 1 ) //now adjust the score depending on the diff between king slot and its neighbor
    {
        diff = fieldarr[king_slot_pos_i][king_slot_pos_j] - fieldarr[king_slot_pos_i][king_slot_pos_j - 1];
        king_slot_scr = king_slot_scr + (1 - king_slot_scr)*(1/(2*diff*diff)); //eg. if diff between king and neighbor only 1 then there's only half the penalty
    }

    //check row
    if (king_slot_pos_i != 3)
        king_slot_scr = king_slot_scr * (0.01/(3.0 - king_slot_pos_i)); //never ever leave the bottom row (if you can't be sure to get back)



    // --- monotony ---

    // starting form the king slot the slot values should fall in a monotonous fashion following a zig-zag motion across the field

    double line_mult = 1.0;
    double chain_counter = 0.0;
    double weight_modifier = 1.0;
    bool endOfRow = false;


    int j = king_slot_pos_j;
    int i = king_slot_pos_i;
    int direction_token = 1; //are we going right (= 1) or left? (= -1)

    //for every row higher or equal of king slot row
    while(i >= 0)
    {
        //do the row
        while(endOfRow == false)
        {

                diff = (fieldarr[i][j] - fieldarr[i][(j + direction_token)]) * direction_token; //diff between current slot and neighbor

                if(diff < 0)
                {
                    // if difference not in line with the wanted zig-zag monotony
                    // then add the score step between the neighbors to the score and multiply it with a factor that depends on the current line (for lines closer to the bottom row monotony is more important)
                    weight_modifier = fieldarr[i][j + direction_token];
                    weight_modifier = weight_modifier*weight_modifier;
                    weight_modifier = weight_modifier/(weight_modifier + 9.0);
                    chain_counter = chain_counter + (-1)*((diff) * line_mult * weight_modifier);
                }


            //increment up or down
            j = j + direction_token;

            //check if end of loop
            if(direction_token > 0)
            {
                if(j >= 4)
                    endOfRow = true;
            }
            else
            {
                if(j <= 0)
                    endOfRow = true;
            }
        }


        //now the transition to the next row

        if(i > 0)//no transition if in top row
        {

                diff = (fieldarr[i][j] - fieldarr[i-1][j]); //diff between current slot and neighbor above

                if(diff < 0)
                {
                    // if difference not in line with the wanted zig-zag monotony
                    // then add the score step between the neighbors to the score and multiply it with a factor that depends on the current line (for lines closer to the bottom row monotony is more important)
                    weight_modifier = fieldarr[i-1][j];
                    weight_modifier = weight_modifier*weight_modifier;
                    weight_modifier = weight_modifier/(weight_modifier + 9.0);
                    chain_counter = chain_counter + (-1)*((diff) * line_mult * weight_modifier);
                }

        }

        line_mult = line_mult * 0.8;
        direction_token = direction_token * (-1);//next row in other direction
        endOfRow = false;
        i--;
    }

    //calc the score form the chain_counter
    scr_chain = 1.0/(1.0 + chain_counter);



    //output
    this->scr_chain_slots = king_slot_scr * scr_chain;
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

void ScoreNode::calc_score_2(Node &inputnode)//score heuristic of scoretype 2: mainly monotony of the gamefield
{
    double score1;
    double score2;

    score1 = scr_free_slots;

    score2 = scr_chain_slots;

    inputnode.score_2 = score1 * score2;
}
