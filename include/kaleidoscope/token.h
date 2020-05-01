

#ifndef KALEIDOSCOPE_TOKEN_H
#define KALEIDOSCOPE_TOKEN_H

#include <cstdint>
#include <map>

using line_t = uint64_t;
using column_t = uint64_t;

enum class TokenType {
    Identifier,
    Keyword,
    Number,
    Marker,
};

struct Token {
    TokenType type;
    line_t line;
    column_t column;

    explicit Token(TokenType t) : type(t) {}
};

#include <string>
#include <utility>

struct Identifier : public Token {
    const std::string content;
    explicit Identifier(const char *ident) : Token(TokenType::Identifier),content(ident) {}
    explicit Identifier(std::string ident) : Token(TokenType::Identifier),content(std::move(ident)) {}
};

enum class KeywordType {
    Def,
    Extern,
};

using keyword_mapping_type = std::map<std::string, KeywordType>;
[[maybe_unused]] keyword_mapping_type keyword_type_string_mapping = {
        keyword_mapping_type::value_type("def", KeywordType::Def),
        keyword_mapping_type::value_type("extern", KeywordType::Extern),
};


using marker_type_underlying_type = uint16_t;
enum class MarkerType: marker_type_underlying_type {
    Add,
    Sub,
    Mul,
    Quo,
    Mod,
    LParen,
    RParen,
    Dot,
};


using marker_mapping_type = std::map<std::string, MarkerType>;
[[maybe_unused]] marker_mapping_type marker_type_string_mapping = {
        marker_mapping_type::value_type("+", MarkerType::Add),
        marker_mapping_type::value_type("-", MarkerType::Sub),
        marker_mapping_type::value_type("*", MarkerType::Mul),
        marker_mapping_type::value_type("/", MarkerType::Quo),
        marker_mapping_type::value_type("%", MarkerType::Mod),
        marker_mapping_type::value_type("(", MarkerType::LParen),
        marker_mapping_type::value_type(")", MarkerType::RParen),
        marker_mapping_type::value_type(",", MarkerType::Dot),
};

struct Keyword : public Token {
    const KeywordType key_type;
    explicit Keyword(KeywordType kt) : Token(TokenType::Keyword), key_type(kt) {}
};

struct Marker : public Token {
    const MarkerType marker_type;
    explicit Marker(MarkerType mt) noexcept : Token(TokenType::Marker), marker_type(mt) {}
};

struct Number: public Token {
    const int64_t value;
    explicit Number(int64_t num) : Token(TokenType::Number), value(num) {}
};


void deleteToken(Token *tok) {

}

#endif