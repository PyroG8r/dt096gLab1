#include <iostream>

#include "lex.h"
#include "op.h"


/*
 * "+" passar ett av två alternativ av operander
 * "*" passar ett eller flera upprepningar av en operand
 * "." passar vilken operand som helst
 * "()" grupperar en eller flera operander
 *
 *
 * EBNF för språket
 * <program> := <expr>
 * <expr> := <text>[<expr>] | <group>[<expr>] | <or>[<expr>] | <many>[<expr>] | <any>[<expr>]
 * <or> := <expr>+<expr>
 * <many> := <expr>*
 * <any> := .
 * <group> := (<text>)
 * <text> := <char>[<text>]
 * <char> := a-z|A-Z|0-9
 *
 */
op* parse_many(it& first, it last, op* expr_node);

ch_op* parse_char(it& first, it last){
    auto ch = lex::check(first, last);

    if(lex::type == lex::END) {
        return nullptr;
    }
    if(lex::type != lex::CHAR){
        return nullptr;
    }


    return new ch_op(ch);
}


// <text> := <char>[<text>]
op* parse_text(it& first, it last){
    auto restore = first;
    auto ch_node = parse_char(first, last);
    if(!ch_node){
        first = restore;
        return nullptr;
    }
    auto result = new text_op;
    restore = first;
    auto ch = lex::check(first, last);
    if(lex::type == lex::MANY){
        first = restore;
        result->add(parse_many(first, last, ch_node));
    } else {
        first = restore;
        result->add(ch_node);
        result->add(parse_text(first, last));
    }
    return result;
}
//<or> := <text>+<text>
or_op* parse_or(it& first, it last){
    auto restore = first;
    auto text_node1 = parse_text(first, last);
    if(!text_node1){
        first = restore; // Restore if parsing first text failed
        return nullptr;
    }
    restore = first;
    auto ch = lex::check(first, last);
    if(lex::type != lex::OR){
        first = restore; // Restore if OR is not found
        return nullptr;
    }
    auto text_node2 = parse_text(first, last);
    if(!text_node2){
        first = restore; // Restore if parsing second text failed
        return nullptr;
    }
    auto or_node = new or_op; // Successfully parsed OR, create OR node
    or_node->add(text_node1);
    or_node->add(text_node2); // Add second text to OR node
    return or_node;
}

// <group> : (<text>)
group_op* parse_group(it& first, it last){
    auto restore = first; // Add restore point here
    auto ch = lex::check(first, last);
    if(lex::type == lex::LPAREN){
        auto text_node = parse_text(first, last);
        if(!text_node) {
            first = restore; // Restore if parsing text failed
            return nullptr;
        }
        lex::check(first, last);
        if(lex::type != lex::RPAREN) {
            first = restore; // Restore if closing parenthesis is not found
            return nullptr;
        }
        // Parse of group was successful, create group node
        auto group_node = new group_op;
        group_node->add(text_node);
        return group_node;
    }
    first = restore; // Restore if opening parenthesis is not found
    return nullptr;
}

// <many> := <expr>*
op* parse_many(it& first, it last, op* expr_node){ // Return type is op* to allow for returning many_op* and expr_op*
    auto restore = first;
    auto ch = lex::check(first, last);
    if(lex::type == lex::MANY){
        auto many_node = new many_op;
        many_node->add(expr_node);
        return many_node;
    }
    first = restore;
    return expr_node;
}

// <any> := .
any_op* parse_any(it& first, it last){
    auto restore = first;
    auto ch = lex::check(first, last);
    if(lex::type == lex::ANY){
        return new any_op;
    }
    first = restore;
    return nullptr;
}

//<expr> := <text>[<expr>] | <group>[<expr>] | <or>[<expr>] | <any>[<expr>]
expr_op* parse_expr(it& first, it last){
    auto restore = first;
    auto group_op = parse_group(first, last);
    if(group_op){
        auto expr_node = new expr_op;
        expr_node->add(parse_many(first, last, group_op));
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    first = restore;
    auto or_op = parse_or(first, last);
    if(or_op){
        auto expr_node = new expr_op;
        expr_node->add(parse_many(first, last,or_op));
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    first = restore;
    auto text_node = parse_text(first, last);
    if(text_node){
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    first = restore;
    auto any_node = parse_any(first, last);
    if(any_node){
        auto expr_node = new expr_op;
        expr_node->add(parse_many(first, last,any_node));
        expr_node->add(parse_expr(first, last));
        return expr_node;
    }

    return nullptr;
}

// <match> := <expr>
match_op* parse_match(it& first, it last){
    auto expr_node = parse_expr(first, last);
    if(!expr_node)
        return nullptr;
    auto result = new match_op;
    result->add(expr_node);
    return result;
}


int main(int argc, char* argv[]) {
    std::string program = "lo* I"; // argv[1];
    std::string input = "Waterlooo I was defeated, you won the war Waterloo promise to love you for ever more Waterloo couldn't escape if I wanted to Waterloo knowing my fate is to be with you Waterloo finally facing my Waterloo";
    auto first = program.begin();
    auto last = program.end();
    auto tree = parse_match(first, last);
    auto match = tree->eval(input.begin(), input.end());

    if(match.has_value()){
        std::cout << "Matched string: " << match.value() << std::endl;
    } else {
        std::cout << "No match found" << std::endl;
    }

    
}