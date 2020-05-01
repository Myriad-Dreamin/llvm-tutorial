//
// Created by kamiyoru on 2020/5/1.
//

#ifndef PASCAL_S_AST_H
#define PASCAL_S_AST_H

#include "token.h"

enum class NodeType {
    ExpNumber,
    ExpIdent,
    ExpFuncCall,
    BiExp,
    UnExp,
};

struct ASTNode {
    NodeType type;

    explicit ASTNode(NodeType type) : type(type) {}
    virtual ~ASTNode() = default;
};

struct Exp : public ASTNode {

    explicit Exp(NodeType type) : ASTNode(type) {}
};

struct ExpNumber : public Exp {
    const Number *num;

    explicit ExpNumber(Number *num) : Exp(NodeType::ExpNumber), num(num) {}
};

struct ExpIdent : public Exp {
    const Identifier *ident;

    explicit ExpIdent(Identifier *ident) : Exp(NodeType::ExpIdent), ident(ident) {}
};

using ParamList = std::vector<Exp*>;

struct ExpFuncCall: public Exp {
    const Identifier *fn;
    const ParamList *params;

    explicit ExpFuncCall(Identifier *fn, const ParamList *params) : Exp(NodeType::ExpFuncCall), fn(fn), params(params) {}
    ~ExpFuncCall() override {
        delete params;
    };
};

struct BiExp : public Exp {
    Exp *lhs, *rhs;
    const Marker *marker;

    explicit BiExp(Exp *lhs, Marker *marker, Exp *rhs) : Exp(NodeType::BiExp), lhs(lhs), rhs(rhs), marker(marker) {}
    ~BiExp() override {
        delete lhs;
        delete rhs;
    };
};

struct UnExp : public Exp {
    Exp *lhs;
    const Marker *marker;

    explicit UnExp(Marker *marker, Exp *lhs) : Exp(NodeType::UnExp), lhs(lhs), marker(marker) {}
    ~UnExp() override {
        delete lhs;
    };
};

#endif //PASCAL_S_AST_H
