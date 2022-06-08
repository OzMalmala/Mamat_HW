#ifndef PORT_H
#define PORT_H

#include "string.h"
#include "field.h"

class Port : public Field {
    int range[2];

public:
    /**
     * @brief initiates the upper bound and the lower
    */
    Port(String pattern);
    /**
     * @brief destroys Port object
    */
    ~Port();
    /**
     * @brief sets 'Field::pattern' to the value in 'val'
     * @param the rule value
     * @return false if String::split fails, or the range is illegal
     * true otherwise
    */
    bool set_value(String val);
    /**
 * @brief compares if the packet matches the rule under this port object
 * @param a packet to compare
 * @returntrue if port value of the packet is in range, false otherwise
 * @note doesnt change the rule
*/
    bool match_value(String packet) const;

};
#endif