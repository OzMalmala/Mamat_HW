#ifndef IP_H
#define IP_H

#include "string.h"
#include "field.h"

enum {RULES_NUM =5 };

class Ip : public Field{
    int rules[RULES_NUM];

public:
    /**
    * @brief initiates the rules arr.the last component of rules is the mask.
    */
    Ip(String pattern);

    /**
     * @brief destroys Ip object
    */
    ~Ip();

    /**
     * @brief set the values to the rules array from a given packet
     * returns false if failed
    */
    bool set_value(String value);

    /**
     * @brief checks if there is a match between
     * a given packet and the rulse.
    */
    bool match_value(String value) const;
};

#endif
