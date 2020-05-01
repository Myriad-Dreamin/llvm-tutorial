

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include "token.h"
#include <vector>
#include <cctype>
#include <cassert>


bool maybe_marker(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
           ch == '%' || ch == '(' || ch == ')' || ch == ',';
}

// istream的功能太过繁杂，抽象一个Stream接口
template<typename Stream>
struct Lexer /* implement Lexer */ {
    using lexer_stream = Stream;
    using token_container = std::vector<Token *>;

    explicit Lexer(Stream &s) : s(s), ch(s.next_char()) {}

    ~Lexer() {
        for (auto tok: tokens) {
            delete tok;
        }
    }

    Token *next_token() {
#define just_lex(lexName) do {\
        std::string buf;\
        auto tok = lexName(buf);\
        tokens.push_back(tok);\
        return tok;\
    } while(0)

        // epsilon
        if (ch == EOF) {
            return nullptr;
        }

        // [ \t\r\n\v]等等
        if (isspace(ch)) {
            next_char();
            while (isspace(ch))
                next_char();
        }

        // [0-9]+
        if (isdigit(ch)) {
            just_lex(lexNumber);
        }

        // [_a-zA-Z][_0-9a-zA-Z]*
        if (isalpha(ch) || ch == '_') {
            just_lex(lexIdent);
        }

        // 前探一个字符确定是否是需要解释的符号
        if (maybe_marker(ch)) {
            just_lex(lexMarker);
        }

        assert(false);
        return nullptr;
#undef just_lex
    }

    const token_container &get_all_tokens() {
        while (next_token() != nullptr);

        return tokens;
    }

private:
    Stream &s;
    decltype(std::declval<Stream>().next_char()) ch;

    token_container tokens;

    Token *lexIdent(std::string &buf) {
        forward(buf);
        while (isalnum(ch) || ch == '_') {
            forward(buf);
        }

        // 如果是关键字，那么优先识别为关键字
        if (keyword_type_string_mapping.count(buf)) {
            return new Keyword(keyword_type_string_mapping.at(buf));
        }
        return new Identifier(buf);
    }

    Token *lexNumber(std::string &buf) {
        forward(buf);
        while (isdigit(ch)) {
            forward(buf);
        }
        return new Number(std::stoll(buf));
    }

    Token *lexMarker(std::string &buf) {
        forward(buf);
        // 不捕捉.at not found异常
        return new Marker(marker_type_string_mapping.at(buf));
    }

    void next_char() {
        ch = s.next_char();
    }

    void forward(std::string &buf) {
        buf.push_back(ch);
        next_char();
    }
};


#endif
