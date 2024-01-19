//
// Created by emilj on 2024-01-19.
//

#ifndef LAB1_OP_H
#define LAB1_OP_H

#include <iostream>
#include <vector>

using IT = std::string::iterator;

struct iter {
    IT lhs;
    IT rhs;
};

struct object {
    IT lhs;
    IT rhs;
    IT end;
    size_t captured = 0;
    std::vector<iter> cap;
};


struct op{
    virtual object*  evaluate(object * o) = 0;
    virtual std::string id() = 0;
    std::vector<op*> operands;
};


struct






#endif //LAB1_OP_H
