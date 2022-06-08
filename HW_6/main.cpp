#include <iostream>
#include <stddef.h>
#include "string.h"
#include "input.h"
#include "field.h"
#include "ip.h"
#include "port.h"

const int SUCCESS = 0;
const int FAIL = -1;
const int RULE_PARTS_NUM =2;

using namespace std;

int main (int argc, char** argv){
    if (check_args(argc ,argv) != 0){
        return FAIL;
    }

    String *rule;
    String original_rule(argv[1]);
    size_t size =0;
    original_rule.split(",=", &rule,&size);
    /*check for valid size*/
    if(size != RULE_PARTS_NUM){
        delete [] rule;
        return FAIL;
    }

    Field *field;

    /*check if it is an ip or a port*/
    if (rule[0].equals("dst-ip")||rule[0].equals("src-ip")){
        field = new Ip(rule[0]);
    }
    else if (rule[0].equals("dst-port")||rule[0].equals("src-port")){
        field = new Port(rule[0]);
    }
    else {
        delete[] rule;
        return FAIL;
    }
    /* set the field */
    if (field->set_value(rule[1]) == false){
        delete field;
        delete[] rule;
        return FAIL;
    }

    parse_input(*field);
    delete[] rule;
    delete field;

    return SUCCESS;

}
