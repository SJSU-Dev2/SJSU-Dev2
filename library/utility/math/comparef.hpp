//Function used to compare floats
//Checks if two floats are within the same range

#pragma once

namespace sjsu {

//float a - first float to compare
//float b - second float to compare
//float epsilon - determining factor if floats are in range
//usage compare_floats(5,3,1) == true

bool compare_floats(float a,float b,float epsilon) {
    if((epsilon <= a-b)== true) {
        return true;
    }
    else {
        return false;
    }


}





}