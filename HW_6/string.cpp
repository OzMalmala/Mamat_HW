#include <cstring>

#include <stddef.h>
#include <iostream>

#include "string.h"
using namespace std;

const int ZERO =48;
const int NINE = 57;
const int ERROR = 0;
const int SUCCESS =1;

/**
 * @brief Initiates an empty string
 */
String :: String() {
    this->length = 0;
    this->data = new char[0];
}

/**
 * @brief Initiates string from other string
 */
String :: String(const String &cpy_str){
    this->length = cpy_str.length;
    this->data = new char[this->length+1];
    strcpy(this->data,cpy_str.data);
}

/**
 * @brief Initiates a string from char array
 */
String :: String(const char* str){
    this->length = strlen(str);
    this->data = new char[this->length+1];
    strcpy(this->data,str);
}

/**
 * @brief destructor
 */
String :: ~String(){
    delete[] data;
}

/**
 * @brief Changes this from String
 */
 String& String :: operator=(const String &rhs){
     /*make sure we dont get to infinite loop if we construct ourself*/
     if(this != &rhs){
        this->length = rhs.length;
        delete [] this->data;
        this->data = new char[this->length+1];
         strcpy(this->data,rhs.data);
     }
    return *this;
}

/**
  * @brief Changes this from char array
  */
String& String :: operator=(const char *str){
    this->length = strlen(str);
    delete[] this->data;
    this->data = new char[this->length+1];
    strcpy(this->data,str);
    return *this;
}

/**
 * @brief Returns true iff the contents of this equals to the
 * contents of rhs
 */
bool String :: equals(const String &rhs) const {
    if(this->length != rhs.length) {
        return false;
    }
    return !strcmp(this->data,rhs.data);
}

/**
 * @brief Returns true iff the contents of this equals to rhs
 */;
bool String :: equals(const char *rhs) const {
    if (this->length != strlen(rhs)){
        return false;
    }
    return (!(strcmp(this->data,rhs)));
}

/**
 * @brief Splits this to several sub-strings according to delimiters.
 * Allocates memory for output that the user must delete (using delete[]).
 * @note Does not affect this.
 * @note If "output" is set to NULL, do not allocated memory, only
 * compute "size".
 */
void String :: split(const char *delimiters, String **output, size_t *size) const{
    int size_count = 0;
    int i=0;
    char *token;
    char *tmp= new char[length+1];
    strcpy(tmp,data);

    token = strtok(data, delimiters);
    /* find size */
    while(token) {
        size_count ++;
        token = strtok(NULL, delimiters);
    }

    (*size) = size_count ;
    if(!output) {
        delete[] tmp;
        return;
    }

    /*set output*/
    token = strtok(tmp, delimiters);
    i = 0;
    *output = new String[size_count] ;
    while(token) {
        (*output)[i] = String(token);
        i++;
        token = strtok(NULL, delimiters);
    }

    delete[] tmp;

}

/**
 * @brief Try to convert this to an integer. Returns 0 on error.
 */
int String :: to_integer() const{
    if(data == NULL) {
        return 0;
    }
    //make sure string is only digits
    for(int i=0 ; i<(int)length ; i++) {
        if( (data[i] < ZERO) || (data[i] > NINE) ) {
            return ERROR;
        }
    }
    return atoi(data);
}

/**
 * @brief Remove any leading or trailing white-spaces.
 * Does not change this.
 */
String String :: trim() const{
    int start = 0, end = (strlen(data)-1);

    /*search for blamks at the end and begining*/
    while(isspace(data[start])){
        start++;
    }
    while(isspace(data[end])){
        end--;
    }

    /*set the new string after triming*/
    int new_len = end - start ;
    char *tmp = new char[new_len +1];
    int index = 0;
    while(index <= new_len){
        tmp[index] = data[start];
        start++;
        index++;
    }
    tmp[new_len + 1] = '\0';
    String out = String(tmp);

    delete [] tmp;
    return out;


}