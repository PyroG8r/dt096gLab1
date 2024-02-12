//
// Created by emilj on 2024-02-08.
//

#ifndef LAB1_OP_H
#define LAB1_OP_H

#include <vector>
#include <string>


using it = std::string::iterator;

struct op{
    virtual bool eval(it first, it last) = 0;
    void add(op* child){
        if(child) {
            children.push_back(child);
        }
    }
    std::vector<op*> children;
};
struct ch_op:op{
    char c;
    ch_op(char c):c(c){}
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        return *first == c;
    }
};
struct text_op:op{
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);
        if(children.size() > 1){  // If the text has more characters to match
            return result && children[1]->eval(first + 1, last); //Match the next character
        }
        return result;
    }
};
struct group_op:op{
    bool eval(it first, it last) override{
        // Iterate over all children and evaluate them
        for(auto& child : children){
            auto result = child->eval(first, last);
            if(!result){
                return false; // Return false as soon as a child fails to evaluate
            }
        }
        return true; // Return true only if all children evaluated successfully
    }
};


struct or_op:op{
    // Returns true if any of the children returns true
    bool eval(it first, it last) override{
        auto result1 = children[0]->eval(first, last);
        if(result1){
            return true; // If the first child succeeds, immediately return true
        }
        // If the first child fails, evaluate the second child
        auto result2 = children[1]->eval(first, last);
        return result2; // Return the result of the second child
    }
};

struct expr_op:op{
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return true;

    }
};
struct match_op:op{
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return true;
    }
};


#endif //LAB1_OP_H
