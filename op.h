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
    virtual std::optional<std::string> eval(it first, it last, int& consumed) = 0; //https://en.cppreference.com/w/cpp/utility/optional

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
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        consumed = 1;
        if(first == last || *first != c){
            return std::nullopt;
        }
        return std::string(1, c); // Convert the character to a string and return it
    }
};
struct text_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        auto result = children[0]->eval(first, last, consumed);

        if(!result){
            return std::nullopt;
        }
        if(children.size() > 1){  // If the text has more characters to match
            auto next_result = children[1]->eval(first + 1, last, consumed); //Match the next character
            if(!next_result){
                return std::nullopt;
            }
            consumed += 1;
            return result.value() + next_result.value();
        }
        return result;
    }
};

struct or_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override{
        // Evaluate the first child
        auto result1 = children[0]->eval(first, last, consumed);
        if(result1){
            return result1; // If the first child succeeds, immediately return the matched string
        }
        // If the first child fails, evaluate the second child
        auto result2 = children[1]->eval(first, last, consumed);
        return result2; // Return the matched string of the second child or std::nullopt if it fails
    }
};

struct many_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override{
        std::string result;
        while(first != last){ // Iterate over the input string
            auto child_result = children[0]->eval(first, last, consumed);
            if(!child_result){ // If the child fails to match, break the loop
                break;
            }
            result += child_result.value(); // Concatenate the matched string
            first ++;
        }
        consumed = result.size();
        return result;
    }
};

struct count_op:op{
    int count;
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        std::string result;
        consumed = 0;
        for(int i = 0; i < count; ++i) {
            auto child_result = children[0]->eval(first, last, consumed);
            if(!child_result) {
                return std::nullopt;
            }
            result += child_result.value();
            first ++;
        }
        consumed = result.size();
        return result;
    }
};

struct any_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        if(first == last)
            return std::nullopt;
        consumed = 1;
        return std::string(1, *first); // Convert the character to a string and return it
    }
};

struct group_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        std::string result;
        // Iterate over all children and evaluate them
        for(auto& child : children){
            auto child_result = child->eval(first, last, consumed);
            if(!child_result){
                return std::nullopt; // Return std::nullopt as soon as a child fails to evaluate
            }
            result += child_result.value(); // Concatenate the result of each child
        }
        return result; // Return the concatenated result
    }
};





struct expr_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override {
        std::string result;
        consumed = 0;
        for(auto& child : children){
            auto child_result = child->eval(first + consumed, last, consumed);
            if(!child_result){
                return std::nullopt; // Return std::nullopt as soon as a child fails to evaluate
            }
            result += child_result.value(); // Concatenate the result of each child
        }
        return result; // Return the concatenated result
    }
};

struct match_op:op{
    std::optional<std::string> eval(it first, it last, int& consumed) override{
        if(first == last)
            return std::nullopt;
        auto result = children[0]->eval(first, last, consumed);
        if(!result){
            return eval(first + consumed, last, consumed);
        }
        return result;
    }
};


#endif //LAB1_OP_H
