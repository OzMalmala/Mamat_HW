#include "field.h"
#include "port.h"
#include "string.h"

const int LOW_RANGE =  0;
const int HIGH_RANGE = 65535;

using namespace std;


/**
 * @brief initiates the upper bound and the lower
*/
Port :: Port(String pattern) :
Field(pattern){
        range[0]=0;
        range[1]=0;
}

/**
 * @brief destroys Port object
*/
Port :: ~Port(){};

/**
 * @brief sets 'Field::pattern' to the value in 'val'
 * @param the rule value
 * @return false if String::split fails, or the range is illegal
 * true otherwise
*/
bool Port :: set_value(String val){
    String *divided_string;
    size_t divs = 0;
    /*split the string and saves the range */
    val.split("-", &divided_string, &divs);
    if(divs != 2){
        delete[] divided_string;
        return false;
    }
    /*cast the range from string to integer*/
                                   range[0] = divided_string[0].to_integer();
    range[1] = divided_string[1].to_integer();

    /*checks the correctness of the range*/
    if((range[0] < LOW_RANGE) || (range[0] > HIGH_RANGE)){
        range[0] = 0;
        delete[] divided_string;
        return false;
    }
    if((range[1] < LOW_RANGE) || (range[1] > HIGH_RANGE)){
        range[1] = 0;
        delete[] divided_string;
        return false;
    }
    if(range[0] > range[1]){
        range[0] = 0;
        range[1] = 0;
        delete[] divided_string;
        return false;
    }
    delete[] divided_string;
    return true;
}

/**
* @brief compares if the packet matches the rule under this port object
* @param a packet to compare
* @returntrue if port value of the packet is in range, false otherwise
* @note doesnt change the rule
*/
bool Port :: match_value(String packet) const{
    /*cast the packet from string to integer*/
    int packet_value = packet.to_integer();
    if((packet_value >= range[0]) && (packet_value <= range[1])){
        return true;
    }
    return false;
}