

#ifndef KALEIDOSCOPE_TOKEN_H
#define KALEIDOSCOPE_TOKEN_H

#include <cstdint>
#include <map>
#include <string>
#include <utility>

using line_t = uint64_t;
using column_t = uint64_t;

static const uint64_t todo = -1;

// 目前只有四种类型
using token_type_underlying_type = uint16_t;
enum class TokenType : token_type_underlying_type {
    // 标识符
    Identifier,
    // 关键字
    Keyword,
    // 数字
    Number,
    // 符号
    Marker,
};


// 基类
struct Token {
    // 类型信息
    TokenType type;
    line_t line;
    column_t column;

    explicit Token(TokenType t) : type(t), line(todo), column(todo) {}

    virtual ~Token() = default;
};

// 定义关键字类型
using keyword_type_underlying_type = uint16_t;
enum class KeywordType : keyword_type_underlying_type {
    Def,
    Extern,
};

// 定义 string -> Keyword 映射
using keyword_mapping_type = std::map<std::string, KeywordType>;
[[maybe_unused]] keyword_mapping_type keyword_type_string_mapping = {
        keyword_mapping_type::value_type("def", KeywordType::Def),
        keyword_mapping_type::value_type("extern", KeywordType::Extern),
};

struct Keyword : public Token {
    const KeywordType key_type;

    explicit Keyword(KeywordType kt) : Token(TokenType::Keyword), key_type(kt) {}
};


// 定义符号类型，同时定义了标点符号的优先级
// 定义 string -> Keyword 映射
using marker_type_underlying_type = uint16_t;
enum class MarkerType : marker_type_underlying_type {
    Add = 0x00,
    Sub = 0x01,
    Mod = 0x10,
    Mul = 0x20,
    Quo = 0x21,
    LParen = 0x30,
    RParen = 0x31,
    Dot = 0x40,
};

// 定义 string -> Marker 映射
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

struct Marker : public Token {
    const MarkerType marker_type;

    explicit Marker(MarkerType mt) noexcept: Token(TokenType::Marker), marker_type(mt) {}
};

// 获取优先级，对齐到二进制第4 (从0开始) 位
marker_type_underlying_type get_marker_pri(const Marker *marker) {
    return *reinterpret_cast<const marker_type_underlying_type *>(
            &marker->marker_type) >> 4u;
}


struct Identifier : public Token {
    const std::string content;

    explicit Identifier(const char *ident) : Token(TokenType::Identifier), content(ident) {}

    explicit Identifier(std::string ident) : Token(TokenType::Identifier), content(std::move(ident)) {}
};

struct Number : public Token {
    const int64_t value;

    explicit Number(int64_t num) : Token(TokenType::Number), value(num) {}
};


#endif