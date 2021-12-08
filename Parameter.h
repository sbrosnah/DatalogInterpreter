#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>


class Parameter {
public:
    Parameter(){};
    ~Parameter(){};
    std::string p;
    std::string ToString();
};

#endif