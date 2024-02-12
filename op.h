//
// Created by emilj on 2024-02-08.
//

#ifndef LAB1_OP_H
#define LAB1_OP_H

#include <vector>
#include <string>
#include <optional>


using it = std::string::iterator;

struct op{
    virtual std::optional<std::string> eval(it first, it last) = 0; //https://en.cppreference.com/w/cpp/utility/optional

    void add(op* child){
        if(child) {
            children.push_back(child);
        }
    }
    std::vector<op*> children;
};
struct ch_op:op{
    char c;
    explicit ch_op(char c):c(c){}
    std::optional<std::string> eval(it first, it last) override {
        if(first == last || *first != c)
            return std::nullopt;
        return std::string(1, c); // Convert the character to a string and return it
    }
};
struct text_op:op{
    std::optional<std::string> eval(it first, it last) override {
        auto result = children[0]->eval(first, last);
        if(!result){
            return std::nullopt;
        }
        if(children.size() > 1){  // If the text has more characters to match
            auto next_result = children[1]->eval(first + 1, last); //Match the next character
            if(!next_result){
                return std::nullopt;
            }
            return result.value() + next_result.value();
        }
        return result;
    }
};

struct or_op:op{
    std::optional<std::string> eval(it first, it last) override{
        // Evaluate the first child
        auto result1 = children[0]->eval(first, last);
        if(result1){
            return result1; // If the first child succeeds, immediately return the matched string
        }
        // If the first child fails, evaluate the second child
        auto result2 = children[1]->eval(first, last);
        return result2; // Return the matched string of the second child or std::nullopt if it fails
    }
};

struct many_op:op{
    std::optional<std::string> eval(it first, it last) override{
        std::string result;
        auto current = first; // Create a copy of the iterator
        while(current != last){ // Iterate over the input string
            auto child_result = children[0]->eval(current, last);
            if(!child_result){ // If the child fails to match, break the loop
                break;
            }
            result += child_result.value(); // Concatenate the matched string
            current++;
        }
        return result;
    }
};

struct any_op:op{
    std::optional<std::string> eval(it first, it last) override {
        if(first == last)
            return std::nullopt;
        return std::string(1, *first); // Convert the character to a string and return it
    }
};

struct group_op:op{
    std::optional<std::string> eval(it first, it last) override {
        std::string result;
        // Iterate over all children and evaluate them
        for(auto& child : children){
            auto child_result = child->eval(first, last);
            if(!child_result){
                return std::nullopt; // Return std::nullopt as soon as a child fails to evaluate
            }
            result += child_result.value(); // Concatenate the result of each child
        }
        return result; // Return the concatenated result
    }
};



struct expr_op:op{
    std::optional<std::string> eval(it first, it last) override {
        std::string result;
        for(auto& child : children){
            auto child_result = child->eval(first++, last);
            if(!child_result){
                return std::nullopt; // Return std::nullopt as soon as a child fails to evaluate
            }
            result += child_result.value(); // Concatenate the result of each child
        }
        return result; // Return the concatenated result
    }
};

struct match_op:op{
    std::optional<std::string> eval(it first, it last) override{
        if(first == last)
            return std::nullopt;
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return result;
    }
};


#endif //LAB1_OP_H
