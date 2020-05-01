

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <vector>
#include <istream>
#include "token.h"
#include <cctype>
#include <cassert>

struct IStream {

    explicit IStream(std::istream &in) : in(in) {}

    [[maybe_unused]] char next_char() {
        return in.get();
    }

protected:
    std::istream &in;
};


bool maybe_marker(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
           ch == '%' || ch == '(' || ch == ')' || ch == ',';
}

template<typename Stream>
struct Lexer {
    using lexer_stream = Stream;
    using token_container = std::vector<Token *>;

    explicit Lexer(Stream &s) : s(s), ch(s.next_char()) {}

    ~Lexer() {
        for (auto tok: tokens) {
            deleteToken(tok);
        }
    }

    Token *next_token() {
#define just_lex(lexName) do {\
        std::string buf;\
        auto tok = lexName(buf);\
        tokens.push_back(tok);\
        return tok;\
    } while(0)
        if (ch == EOF) {
            return nullptr;
        }

        if (isspace(ch)) {
            next_char();
            while (isspace(ch))
                next_char();
        }

        if (isdigit(ch)) {
            just_lex(lexNumber);
        }

        if (isalpha(ch) || ch == '_') {
            just_lex(lexIdent);
        }

        if (maybe_marker(ch)) {
            just_lex(lexMarker);
        }

        assert(false);
        return nullptr;
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
