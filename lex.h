//
// Created by emilj on 2024-02-08.
//

#ifndef LAB1_LEX_H
#define LAB1_LEX_H

#include <string>
using it = std::string::iterator;

class lex {
public:
    enum{
        CHAR, LPAREN, RPAREN, OR, END
    };
    static int type;
    static char check(it& first, it last){
        if(first == last){
            type = END;
            return '\0';
        }
        switch (*first) {
            case '(':
                type = LPAREN;
                return *first++;
            case ')':
                type = RPAREN;
                return *first++;
            case '+':
                type = OR;
                return *first++;
        }
        type = CHAR;
        return *first++;
    }

};


#endif //LAB1_LEX_H
