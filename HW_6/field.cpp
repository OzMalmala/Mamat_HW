#include "field.h"
#include "string.h"
#include <cstring>
#include <iostream>

const int JUMP = 2;
using namespace std;

Field :: Field(String pattern) : pattern(pattern) {}

Field :: ~Field() {}

/* Returns true iff "value" matches the value of this (e.g., if 1.1.1.1
   matches 1.1.*.*) */
bool Field :: match(String packet){

    String *divided_packet;
    size_t divs = 0;
    bool matched = false;

    /*make sure split is valid*/
    packet.split(",=", &divided_packet, &divs);
    if(divs == 0){
        delete[] divided_packet;
        return false;
    }
    /*1.trim to erase blanks
     *2.check if we have port or ip
     *3.check if the rest of the packet matches */
    for(int i = 0; i <(int)divs; i+=JUMP){
        divided_packet[i] = divided_packet[i].trim();
        if(pattern.equals(divided_packet[i])){
            divided_packet[i+1] = divided_packet[i+1].trim();
            matched = match_value(divided_packet[i+1]);
            break;
        }
    }

    delete[] divided_packet;
    return matched;
}