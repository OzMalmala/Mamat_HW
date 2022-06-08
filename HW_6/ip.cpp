#include "ip.h"
#include "string.h"
#include <cmath>

const int IP_SIZE= 4;
const int BYTE_SIZE = 8;
using namespace std;

/**
* @brief initiates the rules arr.the last component of rules is the mask.
*/
Ip :: Ip(String pattern): Field(pattern){
    for (int i=0; i<RULES_NUM; i++){
        rules[i]=0;
    }
}

/**
 * @brief destroys Ip object
*/
Ip :: ~Ip(){}

/**
 * @brief set the values to the rules array from a given packet
 * returns false if failed
*/
bool Ip :: set_value(String value){
    String *tmp;
    size_t size=0;
    value.split("./", &tmp, &size);

    /*make sure the size of the packet is valid*/
    if(size != RULES_NUM){
        delete [] tmp;
        return false;
    }

    /*set each part of the rulr with the correct integer*/
    for(int i=0 ; i<(int)size ; i++) {
        rules[i] = (tmp[i]).to_integer();
    }
    delete[] tmp;
    return true;
}

/**
 * @brief checks if there is a match between
 * a given packet and the rulse.
*/
bool Ip :: match_value(String value) const{
    int translated_arr[IP_SIZE] = {0};
    String *tmp;
    size_t size=0;
    value.split(".", &tmp, &size);
    /*translate each part to integer*/
    for(int i=0 ; i<(int)size ; i++) {
        translated_arr[i] = (tmp[i]).to_integer();
    }
    delete [] tmp;

    int rule_count = 0;
    int divide;
    int rule_part, packet_part;
    int power;
    /*run through all bits relevant to the rule */
    for (int i=0 ; i<rules[IP_SIZE] ; i++) {
        /*if we finished with the byte */
        if(i%BYTE_SIZE == 0) {
            rule_part = rules[rule_count];
            packet_part = translated_arr[rule_count];
            rule_count++; /* move to the next part of the ip, after the dot */
            divide = BYTE_SIZE-1; /* start from the MSB */
        }
        power = pow(2,divide);
        if (rule_part/power != packet_part/power) { /*check if bit is same */
            return false;
        }
        else { /* delete MSB */
            rule_part = rule_part%power;
            packet_part = packet_part%power;
        }
        divide--;
    }
    return true;
}