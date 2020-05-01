//
// Created by kamiyoru on 2020/5/1.
//

#ifndef PASCAL_S_AST_H
#define PASCAL_S_AST_H

#include "token.h"

// 目前只有表达式类型
enum class NodeType {
    ExpNumber,
    ExpIdent,
    ExpFuncCall,
    BiExp,
    UnExp,
};

// 基类
struct ASTNode {
    NodeType type;

    explicit ASTNode(NodeType type) : type(type) {}
    virtual ~ASTNode() = default;
};

// 无意义的表达式基类，用于区分ASTNode，标记用
struct Exp : public ASTNode {

    explicit Exp(NodeType type) : ASTNode(type) {}
};

struct ExpNumber : public Exp {
    //非Node指针不需要delete
    const Number *num;

    explicit ExpNumber(const Number *num) : Exp(NodeType::ExpNumber), num(num) {}
};

struct ExpIdent : public Exp {
    const Identifier *ident;

    explicit ExpIdent(const Identifier *ident) : Exp(NodeType::ExpIdent), ident(ident) {}
};

struct UnExp : public Exp {
    Exp *lhs;
    const Marker *marker;

    explicit UnExp(const Marker *marker, Exp *lhs) : Exp(NodeType::UnExp), lhs(lhs), marker(marker) {}
    ~UnExp() override {
        delete lhs;
    };
};

struct BiExp : public Exp {
    Exp *lhs, *rhs;
    const Marker *marker;

    explicit BiExp(Exp *lhs, const Marker *marker, Exp *rhs) : Exp(NodeType::BiExp), lhs(lhs), rhs(rhs), marker(marker) {}
    ~BiExp() override {
        delete lhs;
        delete rhs;
    };
};

using ParamList = std::vector<Exp*>;
struct ExpFuncCall: public Exp {
    const Identifier *fn;
    ParamList *params;

    explicit ExpFuncCall(const Identifier *fn, ParamList *params) : Exp(NodeType::ExpFuncCall), fn(fn), params(params) {}
    ~ExpFuncCall() override {
        for (auto exp: *params) {
            delete exp;
        }
        delete params;
    };
};

#endif //PASCAL_S_AST_H
