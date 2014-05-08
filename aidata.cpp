#include "aidata.h"


Scorefield::Scorefield()
{

    this->fieldarr.resize(4);
    for (int i = 0; i < 4; i++)
    {
        this->fieldarr[i].resize(4);
        for (int j = 0; j < 4; j++)
        {
            this->fieldarr[i][j].resize(2);
            for (int n = 0 ; n < 2 ; n++)
            {
                this->fieldarr[i][j][n].resize(4);
                for(int k = 0; k < 4; k++)
                {
                    this->fieldarr[i][j][n][k].score_1 = 0.0;
                    this->fieldarr[i][j][n][k].score_2 = 0.0;
                    this->fieldarr[i][j][n][k].deathchance = 0.0;
                    this->fieldarr[i][j][n][k].isDone = false;
                }//for
            }//for
        }//for
    }//for
}



void Scorefield::finalize(Node &inputnode)// go through all scores and calculate deathchances and alternative scores if move wasn't possible
{
    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j < 4 ; j++)
        {
            if( inputnode.fieldarr[i][j] == 0 )//slot was empty...
            {
                for(int n = 0; n < 2 ; n++) //for each possible drop
                {

                    if (are_all_neg(i,j,n)) //if no move was possible then death
                    {
                        this->fieldarr[i][j][n][0].deathchance = 1.0;
                        this->fieldarr[i][j][n][1].deathchance = 1.0;
                        this->fieldarr[i][j][n][2].deathchance = 1.0;
                        this->fieldarr[i][j][n][3].deathchance = 1.0;
                    }
                    else
                    {
                        for(int k = 0; k < 4; k++)
                        {

                            if( this->fieldarr[i][j][n][k].score_1 < 0)//..but move in this direction (with this drop) possible
                            {
                                //find best alternative scores + deathchances

                                this->fieldarr[i][j][n][k].score_1 = get_best_score(i,j,n,1);
                                this->fieldarr[i][j][n][k].score_2 = get_best_score(i,j,n,2);
                                this->fieldarr[i][j][n][k].deathchance = get_best_score(i,j,n,3);
                            }
                        }
                    }
                }
            }
        }
    }
}



double Scorefield::get_best_score(int i, int j , int n, int type) //get best score of demanded type and dropvalue of all directions for current slot
{
    double best_score;

    if(type == 1)//score 1
    {
        best_score = 0.0;
        for(int k = 0; k < 4; k++)
        {
            if(this->fieldarr[i][j][n][k].score_1 > best_score)
            {
                best_score = this->fieldarr[i][j][n][k].score_1;
            }
        }
    }
    else if(type == 2)//score 2
    {
        best_score = 0.0;
        for(int k = 0; k < 4; k++)
        {
            if(this->fieldarr[i][j][n][k].score_2 > best_score)
            {
                best_score = this->fieldarr[i][j][n][k].score_2;
            }
        }
    }
    else if(type == 3)//deathchance
    {
        best_score = 1.0;// !!deathchance lower is better
        for(int k = 0; k < 4; k++)
        {
            if((this->fieldarr[i][j][n][k].deathchance < best_score) && (this->fieldarr[i][j][n][k].deathchance >= 0))
            {
                best_score = this->fieldarr[i][j][n][k].deathchance;
            }
        }
    }

    return best_score;
}



bool Scorefield::are_all_neg(int i, int j, int n)
{
    for(int k = 0; k < 4 ; k++) //checks if any score at this slot is valid (move possible)
    {
        if(this->fieldarr[i][j][n][k].score_1 >= 0)
        {
            return false;
        }
    }

    return true;
}



double Scorefield::get_worst_score(int direction, int type, Node &inputnode) //returns the worst-case score of demanded type for the given direction
{
    double worst_score;

    if (type == 2)
    {
        worst_score = 16.0;

        for(int i = 0; i < 4 ; i++)
        {
            for(int j = 0; j < 4 ; j++)
            {
                if( inputnode.fieldarr[i][j] == 0 )//slot was empty...
                {
                    for(int n = 0; n < 2 ; n++) //for each possible drop
                    {
                        if(this->fieldarr[i][j][n][direction].score_2 < worst_score)
                           worst_score = this->fieldarr[i][j][n][direction].score_2;
                    }
                }
            }
        }
    }
    else if (type == 1)
    {
        worst_score = 1.0;

        for(int i = 0; i < 4 ; i++)
        {
            for(int j = 0; j < 4 ; j++)
            {
                if( inputnode.fieldarr[i][j] == 0 )//slot was empty...
                {
                    for(int n = 0; n < 2 ; n++) //for each possible drop
                    {
                        if(this->fieldarr[i][j][n][direction].score_1 < worst_score)
                           worst_score = this->fieldarr[i][j][n][direction].score_1;
                    }
                }
            }
        }
    }

    return worst_score;
}



double Scorefield::get_deathchance( Node &inputnode )
{
    double deathchance = 0.0;
    int empty_slots = 0;

    for(int i = 0; i < 4 ; i++) //count empty slots
    {
        for(int j = 0; j < 4 ; j++)
        {
            if( inputnode.fieldarr[i][j] == 0 )//slot was empty...
            {
                empty_slots++;
            }
        }
    }

    for(int i = 0; i < 4 ; i++) //sum up deathchance. always take best possible direction for each slot (lowest deathchance) and multiply with probability of occurrence.
    {
        for(int j = 0; j < 4 ; j++)
        {
            if( inputnode.fieldarr[i][j] == 0 )//slot was empty...
            {
                deathchance += ( 0.666666 * this->get_best_score(i,j,0,3) / ((double)empty_slots) );//for 2s
                deathchance += ( 0.333333 * this->get_best_score(i,j,1,3) / ((double)empty_slots) );//for 4s
            }
        }
    }

    return deathchance;
}



Scores_struct Scorefield::get_final_scores(Node &inputnode)
{
    Scores_struct best_scores;
    double temp_score;


    //TODO: check if this logic of best worst scores really makes sense!!

    //coose the best of the worst case scores
    best_scores.score_1 = Scorefield::get_worst_score(0, 1, inputnode);//up
    best_scores.score_2 = Scorefield::get_worst_score(0, 2, inputnode);//



    temp_score = Scorefield::get_worst_score(1, 1, inputnode);//right

    if (temp_score > best_scores.score_1)
        best_scores.score_1 = temp_score;

    temp_score = Scorefield::get_worst_score(1, 2, inputnode);//

    if (temp_score > best_scores.score_2)
        best_scores.score_2 = temp_score;



    temp_score = Scorefield::get_worst_score(2, 1, inputnode);//down

    if (temp_score > best_scores.score_1)
        best_scores.score_1 = temp_score;

    temp_score = Scorefield::get_worst_score(2, 2, inputnode);//

    if (temp_score > best_scores.score_2)
        best_scores.score_2 = temp_score;



    temp_score = Scorefield::get_worst_score(3, 1, inputnode);//left

    if (temp_score > best_scores.score_1)
        best_scores.score_1 = temp_score;

    temp_score = Scorefield::get_worst_score(3, 2, inputnode);//

    if (temp_score > best_scores.score_2)
        best_scores.score_2 = temp_score;



    //calc deathchance
    best_scores.deathchance = this->get_deathchance(inputnode);

    return best_scores;
}



AIdata::AIdata()
{
    //Node initNode;
    //Solverstack.resize(5000, Node());

    /*
    for (unsigned int i = 0; i < Solverstack.size(); i++ ) // init AI data stack
    {
        Solverstack[i].lvl = 0;
        Solverstack[i].score_1 = 0;
        Solverstack[i].score_2 = 0;
        Solverstack[i].deathchance = 0.0;
        Solverstack[i].resolve_direction = 0;
        Solverstack[i].parent_id = 0;
        Solverstack[i].child_id = 0;

        Solverstack[i].fieldarr.resize(4);

        for (int j = 0; j < 4; j++ )
        {
            Solverstack[i].fieldarr[j].resize(4);
            for (int k = 0; k < 4; k++ )
            {
                Solverstack[i].fieldarr[j][k] = 0;
            }
        }
    }
    */
}



bool AIdata::move_up(Node &inputnode)
{

    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,0) == true) {
        returnval = true;
        for(int j = 0; j < 4 ; j++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(inputnode.fieldarr[ceiling][j] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[ceiling][j] == inputnode.fieldarr[floor][j]){ //merger possible?
                    inputnode.fieldarr[ceiling][j] = 0;
                    inputnode.fieldarr[floor][j] += 1; // floor value *2
                    floor++;
                    ceiling++;
                    continue;
                }

                if(inputnode.fieldarr[floor][j] == 0){
                    inputnode.fieldarr[floor][j] = inputnode.fieldarr[ceiling][j];
                    inputnode.fieldarr[ceiling][j] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }
    }
    return returnval;
}



bool AIdata::move_down(Node &inputnode)
{

    int floor;
    int ceiling;
    bool returnval = false;


    if (this->check_move(inputnode, 2) == true)
    {
        returnval = true;
        for(int j = 0; j < 4 ; j++)
        {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0))
            {

                if(inputnode.fieldarr[ceiling][j] == 0)
                { //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[ceiling][j] == inputnode.fieldarr[floor][j])
                { //merger possible?
                    inputnode.fieldarr[ceiling][j] = 0;
                    inputnode.fieldarr[floor][j] += 1; // floor value *2
                    floor--;
                    ceiling--;
                    continue;
                }

                if(inputnode.fieldarr[floor][j] == 0)
                {
                    inputnode.fieldarr[floor][j] = inputnode.fieldarr[ceiling][j];
                    inputnode.fieldarr[ceiling][j] = 0;
                    ceiling--;
                }
                else
                {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }
    }
    return returnval;
}



bool AIdata::move_left(Node &inputnode)
{
    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,3) == true) {
        returnval = true;
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 0;
            ceiling = 1;

            while((ceiling <= 3)&&(floor < 3)) {

                if(inputnode.fieldarr [i][ceiling] == 0){ //is ceiling zero?
                    ceiling++; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[i][ceiling] == inputnode.fieldarr[i][floor]){ //merger possible?
                    inputnode.fieldarr[i][ceiling] = 0;
                    inputnode.fieldarr[i][floor] += 1; // floor value *2
                    floor++;
                    ceiling++;
                    continue;
                }

                if(inputnode.fieldarr[i][floor] == 0){
                    inputnode.fieldarr[i][floor] = inputnode.fieldarr[i][ceiling];
                    inputnode.fieldarr[i][ceiling] = 0;
                    ceiling++;
                }
                else {
                    floor++;
                    if(floor == ceiling)
                        ceiling++;
                }
            }
        }

    }
    return returnval;
}



bool AIdata::move_right(Node &inputnode)
{
    int floor;
    int ceiling;
    bool returnval = false;

    if (this->check_move(inputnode,1) == true) {
        returnval = true;
        for(int i = 0; i < 4 ; i++) {//for each colum

            floor = 3;
            ceiling = 2;

            while((ceiling >= 0)&&(floor > 0)) {

                if(inputnode.fieldarr [i][ceiling] == 0){ //is ceiling zero?
                    ceiling--; //then goto next slot
                    continue;
                }

                if(inputnode.fieldarr[i][ceiling] == inputnode.fieldarr[i][floor]){ //merger possible?
                    inputnode.fieldarr[i][ceiling] = 0;
                    inputnode.fieldarr[i][floor] += 1; // floor value *2
                    floor--;
                    ceiling--;
                    continue;
                }

                if(inputnode.fieldarr[i][floor] == 0){
                    inputnode.fieldarr[i][floor] = inputnode.fieldarr[i][ceiling];
                    inputnode.fieldarr[i][ceiling] = 0;
                    ceiling--;
                }
                else {
                    floor--;
                    if(floor == ceiling)
                        ceiling--;
                }
            }
        }
    }
    return returnval;
}



bool AIdata::check_move(Node &inputnode, int direction)
{   // check if legal move in desired direction possible
    // directions 0 = up, 1 = right, 2 = down, 3 = left

    if (direction == 0) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 0; i < 3; i++)//row
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i+1][j] != 0)) //slide possible?
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i+1][j]))) // merger possible?
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 1){//right
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 3; j > 0; j--)// colum
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i][j-1] != 0))
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j-1])))
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 2){//down
        for(int j = 0; j < 4 ; j++) //colum
        {
            for(int i = 3; i > 0; i--)//row
            {
                if ((inputnode.fieldarr[i][j] == 0)&&(inputnode.fieldarr[i-1][j] != 0)) //slide possible?
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i-1][j]))) // merger possible?
                {
                    return true;
                }
            }
        }
    }
    else if (direction == 3){ //left
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if ((inputnode.fieldarr[i][j] == 0) && (inputnode.fieldarr[i][j+1] != 0))
                {
                    return true;
                }
                if ((inputnode.fieldarr[i][j] != 0) && (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j+1])))
                {
                    return true;
                }
            }
        }
    }
    return false;
}



Scores_struct AIdata::check_score (Node inputnode, int depth) //recursive score checker
{
    //TODO:: BUG deathchance calculation ignores impossible move direcions

    Scores_struct scores;
    scores.score_1 = 0.0;
    scores.score_2 = 0.0;
    scores.deathchance = 0.0;

    bool is_done [4][4][4];
    int done_in_row = 0;

    Node passing_node(inputnode);

    // TODO:: how to choose on endgame?
    // TODO:: branching optimization

    //is_done array

    for(int i = 0 ; i < 4 ; i++) // init is_done k = direction
    {
        for (int j = 0; j < 4 ; j++)
        {
            for (int k = 0; k < 4 ; k++)
            {
                is_done[i][j][k] = false;
            }
        }
    }


    if(depth > 0)// further recursion needed?
    {
        int empty_slots;
        Scores_struct scores_temp, worst_scores_up, worst_scores_down, worst_scores_left, worst_scores_right;
        bool move_done = false;

        empty_slots = AIdata::check_empty(inputnode);

        worst_scores_down.score_1 = 16; //init with the best possible values
        worst_scores_down.score_2 = 1.0;
        worst_scores_down.deathchance = 0.0;

        worst_scores_up.score_1 = 16;
        worst_scores_up.score_2 = 1.0;
        worst_scores_up.deathchance = 0.0;

        worst_scores_left.score_1 = 16;
        worst_scores_left.score_2 = 1.0;
        worst_scores_left.deathchance = 0.0;

        worst_scores_right.score_1 = 16;
        worst_scores_right.score_2 = 1.0;
        worst_scores_right.deathchance = 0.0;

        for(int i = 0 ; i < 4 ; i++) // iterate scorefield
        {
            for (int j = 0; j < 4 ; j++)
            {
                if (inputnode.fieldarr[i][j] == 0) // empty? fielddrop possible?
                {
                    // --- drop 2 ---

                    move_done = false;

                    // do for up
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if((move_up(passing_node))&&(is_done[i][j][0] == false))//move possible?
                    {
                        //do stuff for this slot
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_up.score_1) //now check if this has the worst score of all
                            worst_scores_up.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_up.score_2)
                            worst_scores_up.score_2 = scores_temp.score_2;

                        is_done[i][j][0] = true;

                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance
                    }

                    //extrapolate on the other slots where the result will be the same
                    if(is_done[i][j][0] == true)
                    {

                    }



                    //down
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_down(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_down.score_1)
                            worst_scores_down.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_down.score_2)
                            worst_scores_down.score_2 = scores_temp.score_2;

                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance
                    }



                    //left
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_left(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_left.score_1)
                            worst_scores_left.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_left.score_2)
                            worst_scores_left.score_2 = scores_temp.score_2;

                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance
                    }



                    //right
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 1;

                    if(move_right(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_right.score_1)
                            worst_scores_right.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_right.score_2)
                            worst_scores_right.score_2 = scores_temp.score_2;

                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.666666 * scores_temp.deathchance / empty_slots); //aggregate combined deathchance
                    }


                    //check for gameover (no move was possible => full deathchance for this possible drop)
                    if (move_done == false)
                    {
                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.666666 / empty_slots);
                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.666666 / empty_slots);
                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.666666  / empty_slots);
                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.666666 / empty_slots);
                    }





                    // --- drop 4 ---

                    move_done = false;


                    //up
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_up(passing_node))//move possible?
                    {
                        move_done = true;
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_up.score_1)
                            worst_scores_up.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_up.score_2)
                            worst_scores_up.score_2 = scores_temp.score_2;

                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);
                    }



                    // down
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_down(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_down.score_1)
                            worst_scores_down.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_down.score_2)
                            worst_scores_down.score_2 = scores_temp.score_2;

                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);
                    }



                    //  left
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_left(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_left.score_1)
                            worst_scores_left.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_left.score_2)
                            worst_scores_left.score_2 = scores_temp.score_2;

                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);
                    }



                    //right
                    passing_node.copy_node(inputnode);
                    passing_node.fieldarr[i][j] = 2;

                    if(move_right(passing_node))//move possible?
                    {
                        scores_temp = AIdata::check_score(passing_node, depth-1);
                        if(scores_temp.score_1 < worst_scores_right.score_1)
                            worst_scores_right.score_1 = scores_temp.score_1;
                        if(scores_temp.score_2 < worst_scores_right.score_2)
                            worst_scores_right.score_2 = scores_temp.score_2;

                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.333334 * scores_temp.deathchance / empty_slots);
                    }


                    //check for gameover
                    if (move_done == false)
                    {
                        worst_scores_up.deathchance = worst_scores_up.deathchance + (0.333334  / empty_slots);
                        worst_scores_down.deathchance = worst_scores_down.deathchance + (0.333334  / empty_slots);
                        worst_scores_left.deathchance = worst_scores_left.deathchance + (0.333334  / empty_slots);
                        worst_scores_right.deathchance = worst_scores_right.deathchance + (0.333334 / empty_slots);
                    }
                }
            }
        }

        // ----- choose best direction by score -----
        scores.score_1 = worst_scores_up.score_1;
        scores.score_2 = worst_scores_up.score_2;
        scores.deathchance = worst_scores_up.deathchance;

        //check if down better
        if(scores.score_1 < worst_scores_down.score_1) //looking for best score...
            scores.score_1 = worst_scores_down.score_1;

        if(scores.score_2 < worst_scores_down.score_2)
            scores.score_2 = worst_scores_down.score_2;

        if(scores.deathchance > worst_scores_down.deathchance)//... looking for lowest deathchance
            scores.deathchance = worst_scores_down.deathchance;

        //check left
        if(scores.score_1 < worst_scores_left.score_1)
            scores.score_1 = worst_scores_left.score_1;

        if(scores.score_2 < worst_scores_left.score_2)
            scores.score_2 = worst_scores_left.score_2;

        if(scores.deathchance > worst_scores_left.deathchance)
            scores.deathchance = worst_scores_left.deathchance;

        //check right
        if(scores.score_1 < worst_scores_right.score_1)
            scores.score_1 = worst_scores_right.score_1;

        if(scores.score_2 < worst_scores_right.score_2)
            scores.score_2 = worst_scores_right.score_2;

        if(scores.deathchance > worst_scores_right.deathchance)
            scores.deathchance = worst_scores_right.deathchance;

    }
    else //if no recursion needed calculate and return scores
    {
        AIdata::ScoreCalc.get_scores(inputnode);
        scores.score_1 = inputnode.score_1;
        scores.score_2 = inputnode.score_2;
    }

    return scores;
}



Scores_struct AIdata::check_score_deep (Node inputnode, int depth) //recursive score checker
{

    //TODO: implement scorefield evaluation
    //TODO: deathchances are still all wrong ?

    Scorefield myScorefield;
    Scores_struct scores;
    Node passing_node;
    Scores_struct scores_temp;


    scores.score_1 = 0.0;
    scores.score_2 = 0.0;
    scores.deathchance = 0.0;
    scores.isDone = false;



    if (depth > 0)
    {
        //do recursion

        //right + down
        for(int i = 0; i < 4 ; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                if(inputnode.fieldarr[i][j] == 0) //empty slot ?
                {

                    myScorefield.fieldarr[i][j][0][1].isDone = true;
                    myScorefield.fieldarr[i][j][0][2].isDone = true;
                    myScorefield.fieldarr[i][j][1][1].isDone = true;
                    myScorefield.fieldarr[i][j][1][2].isDone = true;


                    //right
                    //check if scores can be copied over
                    // has a valid score been calculated above me?
                    if((j > 0) && ( myScorefield.fieldarr[i][j-1][0][1].isDone == true ) && ( myScorefield.fieldarr[i][j-1][0][1].score_1 >= 0.0 ))
                    {
                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( check_move( passing_node, 1 ) )
                        {
                            myScorefield.fieldarr[i][j][0][1].score_1 = myScorefield.fieldarr[i][j-1][0][1].score_1;
                            myScorefield.fieldarr[i][j][0][1].score_2 = myScorefield.fieldarr[i][j-1][0][1].score_2;
                            myScorefield.fieldarr[i][j][0][1].deathchance = myScorefield.fieldarr[i][j-1][0][1].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][1].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][1].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][1].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( check_move( passing_node, 1 ) )
                        {
                            myScorefield.fieldarr[i][j][1][1].score_1 = myScorefield.fieldarr[i][j-1][1][1].score_1;
                            myScorefield.fieldarr[i][j][1][1].score_2 = myScorefield.fieldarr[i][j-1][1][1].score_2;
                            myScorefield.fieldarr[i][j][1][1].deathchance = myScorefield.fieldarr[i][j-1][1][1].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][1].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][1].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][1].deathchance = -1.0;
                        }
                    }
                    else
                    {
                        //evaluate score the normal way

                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( move_right( passing_node ) )
                        {

                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][0][1].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][0][1].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][0][1].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][1].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][1].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][1].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( move_right( passing_node ) )
                        {
                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][1][1].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][1][1].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][1][1].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][1].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][1].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][1].deathchance = -1.0;
                        }
                    }//for else (right)


                    //copy check, down

                    //down
                    //check if scores can be copied over
                    // has a valid score been calculated above me?
                    if((i > 0)&&( myScorefield.fieldarr[i-1][j][0][2].isDone == true ) && ( myScorefield.fieldarr[i-1][j][0][2].score_1 >= 0.0 ))
                    {
                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( check_move( passing_node, 2 ) )
                        {
                            myScorefield.fieldarr[i][j][0][2].score_1 = myScorefield.fieldarr[i-1][j][0][2].score_1;
                            myScorefield.fieldarr[i][j][0][2].score_2 = myScorefield.fieldarr[i-1][j][0][2].score_2;
                            myScorefield.fieldarr[i][j][0][2].deathchance = myScorefield.fieldarr[i-1][j][0][2].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][2].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][2].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][2].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( check_move( passing_node, 2 ) )
                        {
                            myScorefield.fieldarr[i][j][1][2].score_1 = myScorefield.fieldarr[i-1][j][1][2].score_1;
                            myScorefield.fieldarr[i][j][1][2].score_2 = myScorefield.fieldarr[i-1][j][1][2].score_2;
                            myScorefield.fieldarr[i][j][1][2].deathchance = myScorefield.fieldarr[i-1][j][1][2].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][2].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][2].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][2].deathchance = -1.0;
                        }
                    }
                    else
                    {
                        //evaluate score the normal way

                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( move_down( passing_node) )
                        {

                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][0][2].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][0][2].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][0][2].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][2].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][2].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][2].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( move_down( passing_node ) )
                        {
                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][1][2].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][1][2].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][1][2].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][2].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][2].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][2].deathchance = -1.0;
                        }
                    }//for else (down)

                }
                else //not empty slot (this has only to be done in this one pass)
                {
                    for(int k = 0; k < 4 ; k++) //mark all scores as invalid
                    {
                        myScorefield.fieldarr[i][j][0][k].isDone = true;
                        myScorefield.fieldarr[i][j][0][k].score_1 = -1.0;
                        myScorefield.fieldarr[i][j][0][k].score_2 = -1.0;
                        myScorefield.fieldarr[i][j][0][k].deathchance = -1.0;

                        myScorefield.fieldarr[i][j][1][k].isDone = true;
                        myScorefield.fieldarr[i][j][1][k].score_1 = -1.0;
                        myScorefield.fieldarr[i][j][1][k].score_2 = -1.0;
                        myScorefield.fieldarr[i][j][1][k].deathchance = -1.0;
                    }
                }
            }
        }


        // left and up
        for(int i = 3; i >= 0 ; i--)
        {
            for(int j = 3; j >= 0; j--)
            {
                if(inputnode.fieldarr[i][j] == 0)
                {

                    myScorefield.fieldarr[i][j][0][0].isDone = true;
                    myScorefield.fieldarr[i][j][0][3].isDone = true;
                    myScorefield.fieldarr[i][j][1][0].isDone = true;
                    myScorefield.fieldarr[i][j][1][3].isDone = true;

                    //left
                    //check if scores can be copied over
                    // has a valid score been calculated above me?
                    if((j < 3) && ( myScorefield.fieldarr[i][j+1][0][3].isDone == true ) && ( myScorefield.fieldarr[i][j+1][0][3].score_1 >= 0.0 ))
                    {
                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( check_move( passing_node, 1 ) )
                        {
                            myScorefield.fieldarr[i][j][0][3].score_1 = myScorefield.fieldarr[i][j+1][0][3].score_1;
                            myScorefield.fieldarr[i][j][0][3].score_2 = myScorefield.fieldarr[i][j+1][0][3].score_2;
                            myScorefield.fieldarr[i][j][0][3].deathchance = myScorefield.fieldarr[i][j+1][0][3].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][3].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][3].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][3].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( check_move( passing_node, 1 ) )
                        {
                            myScorefield.fieldarr[i][j][1][3].score_1 = myScorefield.fieldarr[i][j+1][1][3].score_1;
                            myScorefield.fieldarr[i][j][1][3].score_2 = myScorefield.fieldarr[i][j+1][1][3].score_2;
                            myScorefield.fieldarr[i][j][1][3].deathchance = myScorefield.fieldarr[i][j+1][1][3].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][3].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][3].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][3].deathchance = -1.0;
                        }
                    }
                    else
                    {
                        //evaluate score the normal way

                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( move_left( passing_node ) )
                        {

                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][0][3].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][0][3].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][0][3].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][3].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][3].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][3].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( move_left( passing_node ) )
                        {
                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][1][3].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][1][3].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][1][3].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][3].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][3].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][3].deathchance = -1.0;
                        }
                    }//for else (left)


                    //copy check, up

                    //up
                    //check if scores can be copied over
                    // has a valid score been calculated above me?
                    if((i < 3) && ( myScorefield.fieldarr[i+1][j][0][0].isDone == true ) && ( myScorefield.fieldarr[i+1][j][0][0].score_1 >= 0.0 ))
                    {
                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( check_move( passing_node, 2 ) )
                        {
                            myScorefield.fieldarr[i][j][0][0].score_1 = myScorefield.fieldarr[i+1][j][0][0].score_1;
                            myScorefield.fieldarr[i][j][0][0].score_2 = myScorefield.fieldarr[i+1][j][0][0].score_2;
                            myScorefield.fieldarr[i][j][0][0].deathchance = myScorefield.fieldarr[i+1][j][0][0].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][0].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][0].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][0].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( check_move( passing_node, 2 ) )
                        {
                            myScorefield.fieldarr[i][j][1][0].score_1 = myScorefield.fieldarr[i+1][j][1][0].score_1;
                            myScorefield.fieldarr[i][j][1][0].score_2 = myScorefield.fieldarr[i+1][j][1][0].score_2;
                            myScorefield.fieldarr[i][j][1][0].deathchance = myScorefield.fieldarr[i+1][j][1][0].deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][0].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][0].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][0].deathchance = -1.0;
                        }
                    }
                    else
                    {
                        //evaluate score the normal way

                        //drop 2 (exp = 1)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 1;

                        //move possible with this drop?
                        if( move_up( passing_node ) )
                        {

                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][0][0].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][0][0].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][0][0].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][0][0].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][0][0].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][0][0].deathchance = -1.0;
                        }

                        //drop 4 (exp = 2)
                        this->copy_node(inputnode, passing_node);
                        passing_node.fieldarr[i][j] = 2;

                        //move possible with this drop?
                        if( move_up( passing_node ) )
                        {
                            scores_temp = this->check_score_deep(passing_node, depth-1);

                            myScorefield.fieldarr[i][j][1][0].score_1 = scores_temp.score_1;
                            myScorefield.fieldarr[i][j][1][0].score_2 = scores_temp.score_2;
                            myScorefield.fieldarr[i][j][1][0].deathchance = scores_temp.deathchance;
                        }
                        else//move not possible with this drop
                        {
                            myScorefield.fieldarr[i][j][1][0].score_1 = -1.0;
                            myScorefield.fieldarr[i][j][1][0].score_2 = -1.0;
                            myScorefield.fieldarr[i][j][1][0].deathchance = -1.0;
                        }
                    }//for else (up)
                }
            }
        }



        myScorefield.finalize(inputnode);

        //TODO: calculate scores and deathchances from scorefield

        scores = myScorefield.get_final_scores(inputnode);
    }
    else
    {
        //check scores
        AIdata::ScoreCalc.get_scores(inputnode);
        scores.score_1 = inputnode.score_1;
        scores.score_2 = inputnode.score_2;
        scores.deathchance = 0.0;
    }

    return scores;
}



void AIdata::copy_to_stack(const Node& input_node, int stack_position)
{
    //    //this->Solverstack[stack_position].score = input_node.score;
}



bool AIdata::check_gameover(const Node &inputnode)
{   //false -> move possible ; true -> gameover

    bool returnval = true;

    if (returnval == true) {//up
        for(int j = 0; j < 4 ; j++) //colum
        {
            if (inputnode.fieldarr[3][j] == 0) //any free slot? - last row not covered by the for loop
            {
                returnval = false;
                break;
            }

            for(int i = 0; i < 3; i++)//row
            {
                if (inputnode.fieldarr[i][j] == 0) //any free slot?
                {
                    returnval = false;
                    break;
                }
                else if (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i+1][j])) // merger possible?
                {
                    returnval = false;
                    break;
                }
            }
            if (returnval == false)
                break;
        }
    }

    if (returnval == true){ //left - check for mergers in other direction
        for(int i = 0; i < 4 ; i++) //row
        {
            for(int j = 0; j < 3; j++)// colum
            {
                if (inputnode.fieldarr[i][j] == (inputnode.fieldarr[i][j+1]))
                {
                    returnval = false;
                    break;
                }
            }
            if (returnval == false)
                break;
        }
    }

    return returnval;
}



int AIdata::check_empty (const Node &inputnode)
{
    int returnval = 0;
    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            if(inputnode.fieldarr[i][j] == 0)
                returnval++;
        }
    }

    return returnval;
}



int AIdata::think(std::vector< std::vector<int> > &gamefield, int depth)
{

    Node startvalues; //create Nodes to pass along and init with starting values
    Node whenup;
    Node whenright;
    Node whendown;
    Node whenleft;

    int bestmovedirection = 0;
    double bestscore_1 = 0.0;
    double bestscore_2 = 0.0;
    double lowestdeathchance = 1.0;

    Scores_struct temp_scores;
    temp_scores.deathchance = 0.0;
    temp_scores.score_1 = 0;
    temp_scores.score_2 = 0;

    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            startvalues.fieldarr[i][j] = gamefield[i][j];
        }
    }

    AIdata::copy_node(startvalues, whenup);
    AIdata::copy_node(startvalues, whenright);
    AIdata::copy_node(startvalues, whendown);
    AIdata::copy_node(startvalues, whenleft);



    // now execute moves and evaluate the results with the score checker

    if(move_up(whenup))//up
    {
        temp_scores = AIdata::check_score(whenup, depth);

        bestmovedirection = 0;
        bestscore_1 = temp_scores.score_1;
        bestscore_2 = temp_scores.score_2;
        lowestdeathchance = temp_scores.deathchance;
    }



    if(move_right(whenright))//right
    {
        temp_scores = AIdata::check_score(whenright, depth);

        //take this direction if deathchances is (equal or not worse than 2%) and score_2 is better or if deathchance is lower
        if (((temp_scores.score_2 > bestscore_2) && ((temp_scores.deathchance <= lowestdeathchance)||(temp_scores.deathchance <= 0.02)))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 1;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1)) //if all things are equal differenciate through score_1
        {
            bestmovedirection = 1;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }



    if(move_down(whendown))//down
    {
        temp_scores = AIdata::check_score(whendown, depth);

        if (((temp_scores.score_2 > bestscore_2) && ((temp_scores.deathchance <= lowestdeathchance)||(temp_scores.deathchance <= 0.02)))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 2;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1))
        {
            bestmovedirection = 2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }



    if(move_left(whenleft))//left
    {
        temp_scores = AIdata::check_score(whenleft, depth);

        if (((temp_scores.score_2 > bestscore_2) && ((temp_scores.deathchance <= lowestdeathchance)||(temp_scores.deathchance <= 0.02)))||(temp_scores.deathchance < lowestdeathchance))
        {
            bestmovedirection = 3;
            bestscore_2 = temp_scores.score_2;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
        else if ((temp_scores.score_2 >= bestscore_2) && (temp_scores.deathchance <= lowestdeathchance) && (temp_scores.score_1 > bestscore_1))
        {
            bestmovedirection = 3;
            bestscore_1 = temp_scores.score_1;
            lowestdeathchance = temp_scores.deathchance;
        }
    }

    return bestmovedirection;
}


void AIdata::copy_node(Node &inputnode, Node &outputnode)
{


    outputnode.score_1 = inputnode.score_1;
    outputnode.score_2 = inputnode.score_2;


    for(int i = 0; i < 4 ; i++)
    {
        for (int j = 0; j < 4 ; j++)
        {
            outputnode.fieldarr[i][j] = inputnode.fieldarr[i][j];
        }
    }
}
