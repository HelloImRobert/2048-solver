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
    ScoreNode::calc_scr_king_slot_scr();
    ScoreNode::calc_scr_monotony();

    // get scores
    ScoreNode::calc_score_1(inputnode);
    ScoreNode::calc_score_2(inputnode);

}

// FREE SLOTS SCORE
void ScoreNode::calc_scr_free_slots()
{

    this->scr_free_slots = pow(0.9, 15 - free_slots);

}


//KING SLOT POSITION
void ScoreNode::calc_scr_king_slot_scr()
{
    double diff;

    this->scr_king_slot_scr = 1.0;

    // --- postion of king slot ---

    //check colum
    //colum score = 1/(1+ dist-to-ideal-pos)
    if (king_slot_pos_j != 0)
        this->scr_king_slot_scr = pow(0.3, (double)king_slot_pos_j);

    if (king_slot_pos_j >= 1 ) //now adjust the score depending on the diff between king slot and its neighbor
    {
        diff = fieldarr[king_slot_pos_i][king_slot_pos_j] - fieldarr[king_slot_pos_i][king_slot_pos_j - 1];
        this->scr_king_slot_scr = this->scr_king_slot_scr + (1 - this->scr_king_slot_scr)*(1/(2*diff*diff)); //eg. if diff between king and neighbor only 1 then there's only half the penalty
    }

    //check row
    if (king_slot_pos_i != 3)
        this->scr_king_slot_scr = this->scr_king_slot_scr * (0.00001/(3.0 - king_slot_pos_i)); //never ever leave the bottom row (if you can't be sure to get back)


}

//MONOTONY SCORE
void ScoreNode::calc_scr_monotony()
{
    this->scr_monotony = 1.0; //fail silent
    double temp1;
    double temp2;
    double rowcounter1;
    double rowcounter2;
    int direction = 0;
    int best_direction = 0;


    //main monotony in one direction
    rowcounter1 = 1000000000.0;

    for(direction = 0; direction < 4 ; direction++)
    {
        temp1 = 0.0;

        for(int i = 0; i < 4; i++)
        {
            temp1 += get_rowcounter(i, direction);
        }

        if(temp1 < rowcounter1)
        {
            rowcounter1 = temp1;
            best_direction = direction;
        }
    }


    //get monotony for each row/colum
    //choose the lower score when checking for both directions
    //if mein direction up or down check monotony left-right but now each row/colum individually

    rowcounter2 = 0.0;

    if ((best_direction == 1) || (best_direction == 3))
    {
        for(int i = 0; i < 4; i++)
        {
            //up-down
            temp1 = get_rowcounter(i, 0);
            temp2 = get_rowcounter(i, 2);
            if ( temp1 < temp2)
                rowcounter2 += temp1;
            else
                rowcounter2 += temp2;

        }
    }
    else if ((best_direction == 0) || (best_direction == 2))
    {
        for(int i = 0; i < 4; i++)
        {
            //left right
            temp1 = get_rowcounter(i, 1);
            temp2 = get_rowcounter(i, 3);
            if ( temp1 < temp2)
                rowcounter2 += temp1;
            else
                rowcounter2 += temp2;
        }
    }


    //calc partial scores
    temp1 = pow(0.9, (double)rowcounter1);
    temp2 = pow(0.999, (double)rowcounter2);

    //ouput
    this->scr_monotony = temp1 * temp2;
}

void ScoreNode::calc_scr_chain_slots()
{

    //additional variables
    double diff;
    int direction_token;




    // --- chain ---
    // starting form the king slot the slot values should fall in a monotonous fashion following a zig-zag motion across the field

    double line_mult = 1.0;
    double chain_counter = 0.0;
    double weight_modifier = 1.0;
    bool endOfRow = false;


    int j = king_slot_pos_j;
    int i = king_slot_pos_i;
    direction_token = 1.0; //are we going right (= 1) or left? (= -1)

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
                //                      chain_counter = chain_counter + ((diff)*(diff) * line_mult);
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

                //                    chain_counter = chain_counter + ((diff)*(diff) * line_mult);
            }

        }

        line_mult = line_mult * 0.8;
        direction_token = direction_token * (-1);//next row in other direction
        endOfRow = false;
        i--;
    }

    //calc the score form the chain_counter
    this->scr_chain_slots = pow( 0.9, (double)chain_counter );

}

//HELPER
// calc row/colum monotony rowcounter
double ScoreNode::get_rowcounter(int index, int direction)
{

    double diff;
    double row_counter = 0.0;


    // --- monotony ---

    if(direction == 0) //monotonously decreasing scores to the top...
    {
        for(int i = 0; i < 3; i++)
        {
            diff = fieldarr[i + 1][index] - fieldarr[i][index];

            if(diff < 0) //has monotony been broken?
            {
                row_counter = row_counter + (-1)*diff;
            }
        }
    }
    else if(direction == 2)//...bottom
    {
        for(int i = 3; i >= 1; i--)
        {
            diff = fieldarr[i - 1][index] - fieldarr[i][index];

            if(diff < 0) //has monotony been broken?
            {
                row_counter = row_counter + (-1)*diff;
            }
        }
    }
    else if(direction == 1)//...right
    {
        //horizontal
        for(int j = 0; j < 3; j++)
        {
            diff = fieldarr[index][j + 1] - fieldarr[index][j];

            if(diff < 0) //has monotony been broken?
            {
                row_counter = row_counter + (-1)*diff;
            }
        }
    }
    else if(direction == 3)//...left
    {
        for(int j = 3; j >= 1; j--)
        {
            diff = fieldarr[index][j - 1] - fieldarr[index][j];

            if(diff < 0) //has monotony been broken?
            {
                row_counter = row_counter + (-1)*diff;
            }
        }
    }

    return row_counter;
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

    inputnode.score_1 = scr_free_slots;
}

void ScoreNode::calc_score_2(Node &inputnode)//score heuristic of scoretype 2: mainly monotony of the gamefield
{

    inputnode.score_2 = scr_free_slots * scr_monotony;
}
