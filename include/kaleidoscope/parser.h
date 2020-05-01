//
// Created by kamiyoru on 2020/5/1.
//

#ifndef PASCAL_S_PARSER_H
#define PASCAL_S_PARSER_H

#include "ast.h"
#include "token.h"
#include <cassert>

bool token_equal(const Token *lhs, const Token *rhs) {
    if (lhs == nullptr && rhs == nullptr) {
        return true;
    }
    if (lhs == nullptr || rhs == nullptr) {
        return false;
    }
    if (lhs->type != rhs->type) {
        return false;
    }
    switch (lhs->type) {
        case TokenType::Marker:
            return reinterpret_cast<const Marker *>(lhs)->marker_type ==
                   reinterpret_cast<const Marker *>(rhs)->marker_type;
        default:
            throw std::runtime_error("todo token equal");
    }
}

bool token_equal(const Token *lhs, const std::vector<Token*> *rhs) {
    if (rhs == nullptr) {
        return lhs == nullptr;
    }

    for (auto r: *rhs) {
        if (token_equal(lhs, r)) {
            return true;
        }
    }
    return false;
}

marker_type_underlying_type get_marker_pri(const Marker *marker) {
    return *reinterpret_cast<const marker_type_underlying_type *>(
            &marker->marker_type);
}

template<typename Lexer>
struct Parser {
    explicit Parser(Lexer &lexer) : lexer(lexer), token(lexer.next_token()) {
    }

    ASTNode *parse() {
        if (token == nullptr) {
            return nullptr;
        }
        return parseExp();
    }

protected:
    static const Marker rParen;
    static const Marker dot;
    static const std::vector<Token*> rParenContainer;

    void next_token() {
        token = lexer.next_token();
    }

    Token *token;
    Lexer &lexer;

    Exp *parseExp(const std::vector<Token *> *till = nullptr) {
        auto maybe_lhs = parseFac();
        if (token_equal(token, till)) {
            next_token();
            return maybe_lhs;
        }

        if (token->type == TokenType::Marker) {
            auto marker = reinterpret_cast<Marker *>(token);
            switch (marker->marker_type) {
                case MarkerType::Add:
                case MarkerType::Sub:
                case MarkerType::Mul:
                case MarkerType::Quo:
                case MarkerType::Mod:
                    next_token();
                    return parseBiExp(maybe_lhs, marker, get_marker_pri(marker), till);
                case MarkerType::Dot: case MarkerType::RParen:
                    return maybe_lhs;
                default:
                    assert(false);
                    return nullptr;
            }
        } else {
            throw std::runtime_error("want marker, got ?");
        }

    }

    Exp *parseFac() {
        if (token == nullptr) {
            throw std::runtime_error("expected fac, got nullptr");
        }

        if (token->type == TokenType::Number) {
            auto num = new ExpNumber(
                    reinterpret_cast<Number *>(token));
            next_token();
            return num;
        } else if (token->type == TokenType::Marker) {
            auto marker = reinterpret_cast<Marker *>(token);
            switch (marker->marker_type) {
                case MarkerType::Add:
                case MarkerType::Sub:
                    next_token();
                    return new UnExp(marker, parseExp());
                case MarkerType::LParen:
                    next_token();
                    return parseExp(&rParenContainer);
                default:
                    throw std::runtime_error("expected +/-/(");
            }
        } else if (token->type == TokenType::Identifier) {
            auto ident = reinterpret_cast<Identifier *>(token);
            next_token();
            if (token != nullptr && token->type == TokenType::Marker) {
                auto marker = reinterpret_cast<Marker *>(token);
                if (marker->marker_type == MarkerType::LParen) {
                    next_token();
                    return new ExpFuncCall(
                        ident, parseParamList(new ParamList()));
                }
            }
            return new ExpIdent(ident);
        } else if (token->type == TokenType::Keyword) {
            throw std::runtime_error("todo");
        } else {
            throw std::runtime_error("expected fac");
        }
    }

    ParamList *parseParamList(ParamList *params) {
        for (;;) {
            if (token_equal(&rParen, token)) {
                next_token();
                return params;
            }
            params->push_back(parseExp());
            if (token_equal(&dot, token)) {
                next_token();
            } else if (!token_equal(&rParen, token)) {
                throw std::runtime_error("expected ,/)");
            }
        }
    }

    Exp *parseBiExp(Exp *lhs, Marker *marker, marker_type_underlying_type current_marker_pri,
                    const std::vector<Token *> *till = nullptr) {

        auto rhs = parseFac();
        if (token_equal(token, till)) {
            next_token();
            return new BiExp(lhs, marker, rhs);
        }
        if (token->type != TokenType::Marker) {
            throw std::runtime_error("expected marker");
        }
        auto pri = get_marker_pri(reinterpret_cast<Marker *>(token));
        auto next_marker = reinterpret_cast<Marker *>(token);
        next_token();
        if (current_marker_pri >= pri) {
            return parseBiExp(new BiExp(lhs, marker, rhs),
                              next_marker, pri, till);
        }
        return new BiExp(lhs, marker, parseBiExp(rhs, next_marker, pri, till));
    }
};

template<typename Lexer>
const Marker Parser<Lexer>::rParen = Marker(MarkerType::RParen);
template<typename Lexer>
const Marker Parser<Lexer>::dot = Marker(MarkerType::Dot);
template<typename Lexer>
const std::vector<Token*> Parser<Lexer>::rParenContainer({
     const_cast<Token*>(reinterpret_cast<const Token*>(&Parser<Lexer>::rParen))
});



#endif //PASCAL_S_PARSER_H
